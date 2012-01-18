//#define printDebugStuff
//#define powerDown
#include <math.h>
#include <EtherCard.h>
#define StateMachineAction void*
#define Action(a) reinterpret_cast<void*>(a)
#define CallAction(a) reinterpret_cast<action>(a)()
typedef void* (*action)();

#ifdef printDebugStuff
#define printlnDebug(__s1) Serial.println((__p1));
#define printlnDebug2(__p1, __p2) Serial.println((__p1),(__p2));
#else
#define printlnDebug(__s1) 
#define printlnDebug2(__p1, __p2) 
#endif



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

static StateMachineAction sendingTemperature();

static StateMachineAction waitingForNextRound() {
	if (roundTime < millis()) {
		roundTime += roundLoopTime;
		return Action(sendingTemperature);
	}
	return Action(waitingForNextRound);
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
	printlnDebug("Strange reply from server?");
	printlnDebug(int(statuscode));
	dataSend = 1;
	return 0;
}

static StateMachineAction sendingTemperature() {
	if (sendingStarted == 0) {
		printlnDebug("Starting up ethernet controller");
		sendingStarted = millis();
		dataSend = 0;
#ifdef powerDown
		ether.powerUp();
		initEther();
#endif
		ether.clientTcpReq(resultFromTemperatureStream, sendTemperatureFillRequest, 5555); 
	} 
	else if (dataSend) {
		printlnDebug("data was send");
		sendingStarted = 0;
#ifdef powerDown		
		ether.powerDown();
#endif
		return Action(waitingForNextRound);
	} 
	else if ((millis() - sendingStarted) > 4000) {
		// something went wrong with sending.. lets consider this one failed
		printlnDebug("Sending failure assumed");
		sendingStarted = 0;
#ifdef powerDown		
		ether.powerDown();
#endif
		return Action(waitingForNextRound);
	}
	longDelay = 0;
	ether.packetLoop(ether.packetReceive());
	return Action(sendingTemperature); 
}

static void initEther() {
	if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
		Serial.println( "Failed to access Ethernet controller");

	if (!ether.dhcpSetup())
		Serial.println("DHCP failed");
#ifdef powerDown
	if (ether.dhcpExpired())
		ether.dhcpSetup();
#endif
}

void setup() {
	Serial.begin(57600);
	Serial.println("\n[Starting temp logger]");

	initEther();

	ether.printIp("IP: ", ether.myip);
	if (!ether.dnsLookup(servername))
		Serial.println("DNS failed");
	ether.printIp("Server: ", ether.hisip);
	ether.hisport = 5555;
	roundTime = millis();
}

static StateMachineAction currentState = Action(waitingForNextRound);


void loop() {
	if (ether.dhcpExpired() && !ether.dhcpSetup())
		Serial.println("DHCP failed");		
#ifndef powerDown
	if (ether.dhcpExpired())
		ether.dhcpSetup();
#endif
	longDelay = 1;
	currentState = CallAction(currentState);
	if (longDelay) {
		delay(1000); 
	}
}
