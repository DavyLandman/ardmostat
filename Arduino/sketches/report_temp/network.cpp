#include "network.h"
//#define powerDown
#include <Arduino.h> 
#include <stdint.h>
#include <EtherCard.h>
#include "debuglog.h"
#include "statemachine.h"
#include "sharedstate.h"

static uint8_t mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x32 };
static uint8_t tempserverip[] = { 192, 168, 1, 12 };
char servername[] PROGMEM = "server-download2";

uint8_t Ethernet::buffer[700];

static SharedState* temperatureInfo;
static uint8_t dataSend = 0;
static uint32_t sendingStarted = 0;
static uint32_t roundLoopTime;
static uint32_t roundTime = 0;

static StateMachineChoice sendingTemperature();

static StateMachineChoice waitingForNextRound() {
	if (ether.dhcpExpired() && !ether.dhcpSetup())
		printlnError("DHCP failed");		

	if (roundTime < millis()) {
		roundTime += roundLoopTime;
		return Choice(sendingTemperature);
	}
	return Choice(waitingForNextRound);
}
static word sendTemperatureFillRequest(byte fd) {
	dataSend = 1;
	BufferFiller bfill = ether.tcpOffset();
	double temp = temperatureInfo->currentTemperature; 
	temperatureInfo->newTemperature = 0;
	printlnDebug("Sending temp");
	printlnDebug(temp);
	bfill.emit_raw(reinterpret_cast<char*>(&temp), sizeof temp); 
	return bfill.position();
}

static byte resultFromTemperatureStream(byte fd, byte statuscode, word datapos, word len_of_data) {
	printlnError("Strange reply from server?");
	printlnError(int(statuscode));
	dataSend = 1;
	return 0;
}

static StateMachineChoice sendingTemperature() {
	if (sendingStarted == 0 && temperatureInfo->newTemperature) {
		printlnDebug("Starting send to server");
		sendingStarted = millis();
		dataSend = 0;
#ifdef powerDown
		printlnDebug("Starting up ethernet controller");
		ether.powerUp();
		initEther();
		printlnDebug("Ethernet controller is awake");
#endif
		if (ether.dhcpExpired() && !ether.dhcpSetup())
			printlnError("DHCP failed");		
		ether.clientTcpReq(resultFromTemperatureStream, sendTemperatureFillRequest, 5555); 
	} 
	else if (dataSend) {
		printlnDebug("data was send");
		sendingStarted = 0;
#ifdef powerDown		
		printlnDebug("Powering down ethernet controller");
		ether.powerDown();
#endif
		return Choice(waitingForNextRound);
	} 
	else if ((millis() - sendingStarted) > 4000) {
		// something went wrong with sending.. lets consider this one failed
		printlnError("Sending timeout reached, therefor we assume a sending failure");
		sendingStarted = 0;
#ifdef powerDown		
		printlnDebug("Powering down ethernet controller");
		ether.powerDown();
#endif
		return Choice(waitingForNextRound);
	}
	ether.packetLoop(ether.packetReceive());
	return Choice(sendingTemperature); 
}

static void initEther() {
	if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
		printlnError("Failed to access Ethernet controller");

	if (!ether.dhcpSetup())
		printlnError("DHCP failed");
}


StateMachineChoice initializeNetwork(uint32_t sendEvery, SharedState* sharedState) {
	temperatureInfo = sharedState;	
	roundLoopTime = sendEvery;

	initEther();

#ifdef printInfoStuff
	ether.printIp("IP: ", ether.myip);
#endif
	if (!ether.dnsLookup(servername))
		printlnError("DNS failed");
#ifdef printInfoStuff
	ether.printIp("Server: ", ether.hisip);
#endif
	roundTime = millis();

	return Choice(waitingForNextRound);
}

