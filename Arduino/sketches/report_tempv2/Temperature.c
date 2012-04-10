#include "Temperature.h"
void Temperature_initialize(SharedState state ) {
}

void Temperature_FillSharedState() {
}

void Temperature_InitializeSmoothing() {
}

void Temperature_PrepareNextMeasurementRound() {
}

void Temperature_PrepareNextSmoothStep() {
}

void Temperature_SampleTemperature() {
}

uint8_t _Temperature_con_EnoughSamplesCollected() {
	return 1;
}

uint8_t _Temperature_con_ShouldSmooth() {
	return 1;
}

uint8_t _Temperature_con_ShouldStartMeasurement() {
	return 1;
}

	