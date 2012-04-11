#include "SharedState.h"
#include "TemperatureLog.h"
#include <EtherCard.h>
#include <AESLib.h>

SharedStateInfo state;

void setup() {
	state.lastNonce = 0;
	TemperatureLog_initialize(&state);
}

void loop() {
	TemperatureLog_performStep();
}

