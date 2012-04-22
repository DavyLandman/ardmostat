#include "Temperature.h"
#include "debuglog.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#define SMOOTHAMOUNT 10
#define SMOOTHINTERVAL 1000UL
#define INTERVAL (60000UL - (SMOOTHAMOUNT * SMOOTHINTERVAL))

OneWire ds(2);
DallasTemperature sensors(&ds);
DeviceAddress tempDeviceAddress;

static SharedState sharedState;
static double measuredTemperature;
static uint32_t nextMeasureTime;

void Temperature_initialize(SharedState state ) {
	sharedState = state;
	nextMeasureTime = millis() + INTERVAL;

	sensors.begin();
	if (sensors.getAddress(tempDeviceAddress, 0)) {
		printlnDebug("Found temperature sensor.");
		sensors.setResolution(tempDeviceAddress, 12);
		sensors.setWaitForConversion(false); 
	}
	else {
		printlnError("Could not find temperature sensor");
	}
}

void Temperature_FillSharedState() {
	sharedState->currentTemperature = measuredTemperature / SMOOTHAMOUNT;
	printlnDebug("Temperature: ");
	printlnDebug(sharedState->currentTemperature);
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
	sensors.requestTemperatures();  
	sensorReady = millis() + 750UL;
}

void Temperature_SampleTemperature() {
	measuredTemperature += sensors.getTempCByIndex(0);
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
	
