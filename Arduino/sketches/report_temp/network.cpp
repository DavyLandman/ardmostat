#include "network.h"
#define POWERDOWN 
//#define getSchedule
#include <Arduino.h> 
#include <stdint.h>
#include <EtherCard.h>
#include <AESLib.h>
#include "debuglog.h"
#include "statemachine.h"
#include "sharedstate.h"

static uint8_t mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x32 };
static uint8_t tempserverip[] = { 192, 168, 1, 12 };
char servername[] PROGMEM = "server-download2";
static const uint8_t encKey[16] = 
	{ 0xEE, 0xCC, 0x3C, 0x4D,
	  0x31, 0xF9, 0xB2, 0xDA,
	  0xBE, 0xEF, 0x37, 0xA2,
	  0x43, 0x42, 0x55, 0x76
	};


byte Ethernet::buffer[700];

static SharedState* sharedState;
static uint_fast8_t temperatureSend;

static word serverRequestTemperature(byte fd) {
	// filling state
	uint8_t data[16] = {0};
	data[0] = uint8_t(uint32_t(sharedState->currentTemperature)); // whole part 
	data[1] = uint8_t(uint32_t(sharedState->currentTemperature * 100) % 100); // fraction part
	data[2] = sharedState->currentState;
	*(uint32_t*)(data + (16 - sizeof(uint32_t))) = sharedState->lastNonce;
	
	// encrypting the state
	aes128_enc_single(encKey, data);
	printlnDebug("Sending encrypted state");

	BufferFiller bfill = ether.tcpOffset();
	bfill.emit_raw((char*)data, sizeof data); 
	temperatureSend = 1;
	return bfill.position();
}

static byte serverReplyTemperature(byte fd, byte statuscode, word datapos, word len_of_data) {
	// this method should not be called when sending temperature, if it happens,
	// something strange happend..
	printlnError("Strange reply from server?");
	printlnError(int(statuscode));
	temperatureSend = 1;
	return 0;
}

static uint_fast8_t scheduleReceived;
static word serverRequestSchedule(byte fd) {
	// we do not send anything to get the schedule
	BufferFiller bfill = ether.tcpOffset();
	return bfill.position();
}

static byte serverReplySchedule(byte fd, byte statuscode, word datapos, word len_of_data) {
	if (statuscode) {
		printlnError("Strange reply from server?");
		printlnError(int(statuscode));
	}
	else {
		// do something with data (ether.buffer + datapos)
	}
	scheduleReceived = 1;
	return 0;
}
static void fillSharedScheduleState() {
	// perhaps mark the schedule as ready?
}

static void initEther() {
	if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
		printlnError("Failed to access Ethernet controller");

	if (!ether.dhcpSetup())
		printlnError("DHCP failed");
}

static void wakeUpEthernet() {
#ifdef POWERDOWN
	printlnDebug("Starting up ethernet controller");
	ether.powerUp();
	initEther();
	printlnDebug("Ethernet controller is awake");
#endif
}

static void sleepEthernet() { 
#ifdef POWERDOWN		
	printlnDebug("Powering down ethernet controller");
	ether.powerDown();
#endif
}

static const uint32_t sendingTimeout = 4 * 1000UL;
static uint32_t temperatureSendingStop;

static void initiateConnectionTemperature() {
	printlnDebug("Starting sending of Temperature");
	if (ether.dhcpExpired() && !ether.dhcpSetup())
		printlnError("DHCP failed");		

	temperatureSendingStop = millis() + sendingTimeout;
	temperatureSend = 0;
	ether.clientTcpReq(serverReplyTemperature, serverRequestTemperature, 5555); 
}

static const uint32_t receivingTimeout = 8 * 1000UL;
static uint32_t scheduleReceivingStop;

static void initiateConnectionSchedule() {
	printlnDebug("Starting receiving of Schedule");
	if (ether.dhcpExpired() && !ether.dhcpSetup())
		printlnError("DHCP failed");		

	scheduleReceivingStop = millis() + receivingTimeout;
	scheduleReceived = 0;
	ether.clientTcpReq(serverReplySchedule, serverRequestSchedule, 6666); 
}

static void recvSendPackets() {
	ether.packetLoop(ether.packetReceive());
}

static StateMachineChoice shouldStartCommunication();
static void initNextCommunicationRound();

static StateMachineChoice wasConnectionCompletedSchedule() {
	recvSendPackets();
	if (scheduleReceived) {
		printlnDebug("Receiving succeeded");
		fillSharedScheduleState();

		sleepEthernet();
		initNextCommunicationRound();
		return Choice(shouldStartCommunication);
	}
	else if (millis() > scheduleReceivingStop) {
		printlnDebug("Receiving timeout?");

		sleepEthernet();
		initNextCommunicationRound();
		return Choice(shouldStartCommunication);
	}
	else {
		return Choice(wasConnectionCompletedSchedule);
	}
}
static StateMachineChoice wasConnectionCompletedTemperature() {
	recvSendPackets();
	if (temperatureSend) {
#ifdef getSchedule
		printlnDebug("Sending succeeded, now retrieving schedule");
		initiateConnectionSchedule();
		return Choice(wasConnectionCompletedSchedule);
#else
		sleepEthernet();
		initNextCommunicationRound();
		return Choice(shouldStartCommunication);
#endif
	}
	else if (millis() > temperatureSendingStop) {
		printlnDebug("Sending timeout?");
		sleepEthernet();
		initNextCommunicationRound();
		return Choice(shouldStartCommunication);
	}
	else {
		return Choice(wasConnectionCompletedTemperature);
	}
}

static uint32_t sendStep;
static uint32_t nextTime;

static void initNextCommunicationRound() {
	nextTime += sendStep;
}

static StateMachineChoice shouldStartCommunication() {
	if (millis() > 	nextTime) {
		wakeUpEthernet();
		initiateConnectionTemperature();
		return Choice(wasConnectionCompletedTemperature);
	}
	return Choice(shouldStartCommunication); 
}


StateMachineChoice initializeNetwork(uint32_t sendEvery, SharedState* psharedState) {
	sharedState = psharedState;	
	sendStep = sendEvery;

	initEther();

#ifdef printInfoStuff
	ether.printIp("IP: ", ether.myip);
#endif
	if (!ether.dnsLookup(servername))
		printlnError("DNS failed");
#ifdef printInfoStuff
	ether.printIp("Server: ", ether.hisip);
#endif
	nextTime = millis() + sendEvery;

	return Choice(shouldStartCommunication);
}

