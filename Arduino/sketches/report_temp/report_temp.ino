//#define powerDown
#include <math.h>
#include <EtherCard.h>
#include "debuglog.h"
#include "statemachine.h"

static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x32 };
static byte tempserverip[] = { 192, 168, 1, 12 };
char servername[] PROGMEM = "server-download2";

static double Thermister(int RawADC) {
	double Temp;
	Temp = log(((10240000/RawADC) - 10000));
	Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
	Temp = Temp - 273.15;								// Convert Kelvin to Celcius
	return Temp;
}


byte Ethernet::buffer[700];


static byte longDelay = 1;
static byte dataSend = 0;
static unsigned long sendingStarted = 0;
static const unsigned long roundLoopTime = 60*1000UL;
static unsigned long roundTime = 0;

static StateMachineChoice sendingTemperature();

static StateMachineChoice waitingForNextRound() {
	if (roundTime < millis()) {
		roundTime += roundLoopTime;
		return Choice(sendingTemperature);
	}
	return Choice(waitingForNextRound);
}


static double collectTemperature() {
	double tempTotal = 0.0;
	for (int i = 0; i < 10; i++ ) {
		double temp = Thermister(analogRead(0));
		tempTotal += temp;
		delay(10);
	}
	return tempTotal / 10;
}

static word sendTemperatureFillRequest(byte fd) {
	dataSend = 1;
	BufferFiller bfill = ether.tcpOffset();
	double temp = collectTemperature();
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
	if (sendingStarted == 0) {
		printlnDebug("Starting send to server");
		sendingStarted = millis();
		dataSend = 0;
#ifdef powerDown
		printlnDebug("Starting up ethernet controller");
		ether.powerUp();
		initEther();
		printlnDebug("Ethernet controller is awake");
#endif
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
	longDelay = 0;
	ether.packetLoop(ether.packetReceive());
	return Choice(sendingTemperature); 
}

static void initEther() {
	if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
		printlnError("Failed to access Ethernet controller");

	if (!ether.dhcpSetup())
		printlnError("DHCP failed");
}

void setup() {
	Serial.begin(57600);
	printlnInfo("\n[Starting temp logger]");
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
}

static StateMachineChoice currentState = Choice(waitingForNextRound);


void loop() {
	if (ether.dhcpExpired() && !ether.dhcpSetup())
		printlnError("DHCP failed");		
#ifndef powerDown
	if (ether.dhcpExpired())
		ether.dhcpSetup();
#endif
	longDelay = 1;
	currentState = CallChoice(currentState);
	if (longDelay) {
		delay(1000); 
	}
}
