#include "SharedState.h"
#include "TemperatureLog.h"

SharedStateInfo state;

void setup() {
	TemperatureLog_initialize(&state);
}

void loop() {
	TemperatureLog_performStep();
}

