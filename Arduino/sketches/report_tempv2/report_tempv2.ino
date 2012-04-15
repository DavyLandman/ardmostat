#include "SharedState.h"
#include "TemperatureLog.h"
#include <EtherCard.h>
#include <AESLib.h>
#include <OneWire.h>

SharedStateInfo state;

void setup() {
	Serial.begin(57600);
	state.lastNonce = 0;
	TemperatureLog_initialize(&state);
}

void loop() {
	TemperatureLog_performStep();
}

