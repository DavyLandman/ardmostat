#include "Ethernet.h"
#include <Arduino.h> 
#include <stdint.h>
#include <EtherCard.h>
#include <AESLib.h>
#include "debuglog.h"
#define INTERVAL 60000UL
#define TIMEOUT 4000UL
#define SENDINGEASE 500UL

static uint8_t mymac[] = { 0x74,0x69,0x69, 0xA2, 0xD4, 0x10 };
char servername[] PROGMEM = "server-download2";
static const uint8_t encKey[16] = 
	{ 0xEE, 0xCC, 0x3C, 0x4D,
	  0x31, 0xF9, 0xB2, 0xDA,
	  0xBE, 0xEF, 0x37, 0xA2,
	  0x43, 0x42, 0x55, 0x76
	};

static void initEther();

static const uint8_t hexLookup[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
// an inplace conversion to hexadecimal. this makes the assumption the toConvert
// pointer contains originalSize * 2 space
static void toHex_i(uint8_t* toConvert, uint_fast16_t originalSize) {
	uint8_t* endOri = (toConvert + originalSize) - 1;
	uint8_t* endHex = endOri + originalSize;
	while (endOri >= toConvert) {
		*endHex-- = hexLookup[(*endOri) & 0x0F];
		*endHex-- = hexLookup[(*endOri) >> 4];
		endOri--;
	}
}

byte Ethernet::buffer[700];


static uint32_t nextSendTime;
static SharedState sharedState;


void Ethernet_DoPowerDownEthernet() {
	printlnDebug("Powering down ethernet controller");
	ether.powerDown();
}

void Ethernet_WakeUpEthernet() {
	printlnDebug("Starting up ethernet controller");
	ether.powerUp();
	initEther();
	printlnDebug("Ethernet controller is awake");
}

void Ethernet_RecvAndSendPackets() {
	ether.packetLoop(ether.packetReceive());
}

static uint8_t temperatureSend;
static uint32_t sendEaseTime;

static word serverRequestTemperature(byte fd) {
	// filling state
	uint8_t data[32] = {0}; // 16 bytes block for encryption, which have to be send in hex
	data[0] = uint8_t(uint32_t(sharedState->currentTemperature)); // whole part 
	data[1] = uint8_t(uint32_t(sharedState->currentTemperature * 100) % 100); // fraction part
	*(uint32_t*)(data + (16 - sizeof(uint32_t))) = sharedState->lastNonce;

	sharedState->lastNonce += 1;
	
	// encrypting the state
	aes128_enc_single(encKey, data);

	// encoding it as hex for posting to http server
	toHex_i(data, 16);


	printlnDebug("Sending encrypted state");
	BufferFiller bfill = ether.tcpOffset();
	bfill.emit_raw((char*)data, sizeof data); 
	temperatureSend = 1;
	sendEaseTime = millis() + SENDINGEASE;
	return bfill.position();
}

static byte serverReplyTemperature(byte fd, byte statuscode, word datapos, word len_of_data) {
	// this method should not be called when sending temperature, if it happens,
	// something strange happend..
	printlnError("Strange reply from server?");
	printlnError(int(statuscode));
	temperatureSend = 1;
	sendEaseTime = millis() + SENDINGEASE;
	return 0;
}

static uint32_t temperatureSendingStop;


void Ethernet_InitializeTemperatureSending() {
	temperatureSendingStop = millis() + TIMEOUT;
	temperatureSend = 0;
	ether.clientTcpReq(serverReplyTemperature, serverRequestTemperature, 5555); 
}

void Ethernet_MarkTimeout() {
	printlnDebug("Timeout occured!");
}


uint8_t _Ethernet_con_WasTemperatureSend() {
	return temperatureSend && millis() >= sendEaseTime;
}

uint8_t _Ethernet_con_HasTemperatureSendingTimeoutPassed() {
	return !temperatureSend && millis() > temperatureSendingStop;
}


static void initEther() {
	if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
		printlnError("Failed to access Ethernet controller");

	if (!ether.dhcpSetup())
		printlnError("DHCP failed");
}

void Ethernet_initialize(SharedState state ) {
	sharedState = state;
	initEther();

#ifdef printInfoStuff
	ether.printIp("IP: ", ether.myip);
#endif
	if (!ether.dnsLookup(servername))
		printlnError("DNS failed");
#ifdef printInfoStuff
	ether.printIp("Server: ", ether.hisip);
#endif
	nextSendTime = millis() + INTERVAL;
}

void Ethernet_InitializeNextCommunicationRound() {
	nextSendTime += INTERVAL;
}

uint8_t _Ethernet_con_ShouldStartCommunication() {
	return millis() >= nextSendTime;
}
