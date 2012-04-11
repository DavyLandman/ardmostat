#include "SharedState.h"
#include "TemperatureLog.h"
#include <EtherCard.h>
#include <AESLib.h>

SharedStateInfo state;

void setup() {
	TemperatureLog_initialize(&state);
}

void loop() {
	TemperatureLog_performStep();
}

