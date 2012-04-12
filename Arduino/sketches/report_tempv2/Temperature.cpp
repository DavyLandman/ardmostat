#include "Temperature.h"
#include "debuglog.h"
#include <OneWire.h>
#define SMOOTHAMOUNT 10
#define SMOOTHINTERVAL 500UL
#define INTERVAL (60000UL - (SMOOTHAMOUNT * SMOOTHINTERVAL))

OneWire ds(1);
static uint8_t sensorAddress[8];

static SharedState sharedState;
static double measuredTemperature;
static uint32_t nextMeasureTime;

void Temperature_initialize(SharedState state ) {
	sharedState = state;
	nextMeasureTime = millis() + INTERVAL;
	if (!ds.search(sensorAddress)) {
		printlnError("Cannot find temperature sensor");
		ds.reset_search();
	}
	else if (OneWire::crc8(sensorAddress, 7) != sensorAddress[7]) {
		printlnError("Invalid temperature sensor address found");
	}
	else if (sensorAddress[0] != 0x10) {
		printlnError("Device is not a DS18S20 family device");
	}
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

static uint32_t sensorReady;
void Temperature_RequestTemperature() {
	//ds.skip();
	ds.reset();
	ds.select(sensorAddress);
	ds.write(0x44, 0);
	sensorReady = millis() + 1000UL;
}

static double lastValidTemp = 20.0;
void Temperature_SampleTemperature() {
	ds.reset();
	ds.select(sensorAddress);
	ds.write(0xBE, 0);

	uint8_t data[8];
	int i;
	for (i = 0; i < 8; i++) {
		data[i] = ds.read();
	}
	uint8_t crc = ds.read();
	if (OneWire::crc8(data, 8) == crc) {
		lastValidTemp = *((double*)data);
	}
	measuredTemperature += lastValidTemp;
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

uint8_t _Temperature_con_TemperatureReady() {
	return millis() >= sensorReady;
}
	
