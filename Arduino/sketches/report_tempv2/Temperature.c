#include "Temperature.h"
#include "debuglog.h"
#define SMOOTHAMOUNT 10
#define SMOOTHINTERVAL 500UL
#define INTERVAL (60000UL - (SMOOTHAMOUNT * SMOOTHINTERVAL))

static SharedState sharedState;
static double measuredTemperature;
static uint32_t nextMeasureTime;

void Temperature_initialize(SharedState state ) {
	sharedState = state;
	nextMeasureTime = millis() + INTERVAL;
}

void Temperature_FillSharedState() {
	sharedState->currentTemperature = measuredTemperature / SMOOTHAMOUNT;
}

static int smoothCount;
static uint32_t nextSampleTime;
void Temperature_InitializeSmoothing() {
	smoothCount = 0;
	nextSampleTime = nextMeasureTime + SMOOTHINTERVAL;
	measuredTemperature = 0.0;
}

void Temperature_PrepareNextMeasurementRound() {
	nextMeasureTime += INTERVAL;
}

void Temperature_PrepareNextSmoothStep() {
	nextSampleTime += SMOOTHINTERVAL;
}

void Temperature_SampleTemperature() {
	smoothCount++;
}

uint8_t _Temperature_con_EnoughSamplesCollected() {
	return smoothCount >= SMOOTHAMOUNT;
}

uint8_t _Temperature_con_ShouldSmooth() {
	return millis() >= nextSampleTime;
}

uint8_t _Temperature_con_ShouldStartMeasurement() {
	return millis() >= nextMeasureTime;
}

	
