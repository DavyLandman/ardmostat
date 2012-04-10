#include "TemperatureLog.h"
/***************************************
** This file is generated, do not edit! 
** You can edit SharedState.h
****************************************/
#include <stdlib.h>
#include "SharedState.h"
#include "_SMEthernet.h"
#include "_SMTemperature.h"
static void* Temperature = NULL;
static void* Ethernet = NULL;
void TemperatureLog_initialize(SharedState state) {
	 Temperature = SM_Temperature_initialize(state );
	 Ethernet = SM_Ethernet_initialize(state );
}

void TemperatureLog_performStep() {
	 SM_Temperature_takeStep(Temperature );
	 SM_Ethernet_takeStep(Ethernet );
}

int8_t TemperatureLog_canSleep() {
	return SM_Temperature_isSleepableStep(Temperature)
	       && SM_Ethernet_isSleepableStep(Ethernet);
}
	