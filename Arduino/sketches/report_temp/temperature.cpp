#include "temperature.h"
#include <Arduino.h>
#include <stdint.h>
#include <math.h>
#include "statemachine.h"
#include "sharedstate.h"
#define THERMISTERPIN 0

static double calculateTemperature(int RawADC) {
	double t;
	t = log(((10240000/RawADC) - 10000));
	t = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * t * t ))* t );
	t = t - 273.15;								// Convert Kelvin to Celcius
	return t;
}


static SharedState* target;
static const uint8_t smoothSteps = 10;
static const uint8_t smoothDelay = 20; //ms
static uint32_t measureStep;
static uint32_t nextTime;

static StateMachineChoice shouldMeasure();

static uint_fast8_t smoothCount;
static double tempTotal;
static void initSmoothing() {
	smoothCount = 0;
	tempTotal = 0.0;
}

static void sampleTemperature() {
	smoothCount++;
	tempTotal += calculateTemperature(analogRead(THERMISTERPIN));
}

static void fillTarget() {
	target->currentTemperature = tempTotal / smoothSteps;
}

static void prepareNextSmoothStep() {
	nextTime += smoothDelay;
}

static void initWaitNextRound() {
	nextTime += measureStep;
}

static StateMachineChoice shouldSmooth() {
	if (smoothCount < smoothSteps && millis() > nextTime) {
		sampleTemperature();
		prepareNextSmoothStep();
		return Choice(shouldSmooth);
	}
	else if (smoothCount == smoothSteps) {
		fillTarget();
		initWaitNextRound();
		return Choice(shouldMeasure);
	}
	return Choice(shouldSmooth);
}

static StateMachineChoice shouldMeasure() {
	if (millis() > nextTime) {
		initSmoothing();
		return Choice(shouldSmooth);
	}
	return Choice(shouldMeasure);
}

StateMachineChoice initializeTemperature(uint32_t measureEvery, SharedState* sharedState) {
	target = sharedState;
	measureStep = measureEvery - (smoothSteps * smoothDelay);
	nextTime = millis();
	return Choice(shouldMeasure);
}
