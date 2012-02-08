#include "debuglog.h"
#include "statemachine.h"
#include "sharedstate.h"
#include "network_state.h"
#include "network_init.h"
#include "temperature.h"

#include <AESLib.h>
#include <EtherCard.h> // stupid Arduino (and SCons) need to see we use the library here..


static SharedState sharedState;
static StateMachineChoice temperatureLoop; 
static StateMachineChoice ethernetLoop; 

void setup() {
	Serial.begin(57600);
	printlnInfo("\n[Starting temp logger]");
	temperatureLoop = initializeTemperature(60*1000UL, &sharedState);
	NetworkInitInformation networkInit;
	networkInit.sendEvery = 60*1000UL;
	networkInit.sharedState = &sharedState;
	ethernetLoop = initializeStateMachineNetwork(&networkInit);
}

void loop() {
	temperatureLoop = CallChoice(temperatureLoop);
	ethernetLoop = CallChoice(ethernetLoop);
}
