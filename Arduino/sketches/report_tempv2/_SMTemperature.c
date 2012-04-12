#include "_SMTemperature.h"
/***************************************
** This file is generated, do not edit! 
** You can edit SharedState.h or Temperature.cpp
****************************************/
#include "Temperature.h"
struct State;
typedef void (*StatePointer)(struct State* );
struct State {
	StatePointer nextState;
	uint8_t sleepable;
};
#ifdef __cplusplus
#define GETSMSTATE(pt) (reinterpret_cast<struct State*>(pt))
#else
#define GETSMSTATE(pt) ((struct State*)pt)
#endif
static void _WaitForTemperatureMeasurement(struct State* sm );
static void _WaitForSmoothingFinished(struct State* sm );
static void _WaitForTemperatureReceived(struct State* sm );
void* SM_Temperature_initialize(SharedState state ) {
	Temperature_initialize(state );
	struct State* result = GETSMSTATE(malloc(sizeof(struct State)));
	result->sleepable = 1;
	result->nextState = _WaitForTemperatureMeasurement;
	return result; 
}

void SM_Temperature_takeStep(void* sm ) {
	GETSMSTATE(sm)->nextState(GETSMSTATE(sm) );
}

uint8_t SM_Temperature_isSleepableStep(const void* sm) {
	return GETSMSTATE(sm)->sleepable;
}

static void _WaitForTemperatureMeasurement(struct State* sm ) {
	//pre actions
	if (_Temperature_con_ShouldStartMeasurement()) {
		Temperature_InitializeSmoothing();
		sm->sleepable = 0;
		sm->nextState = _WaitForSmoothingFinished;
		return;
	}

	sm->sleepable = 1;
	sm->nextState = _WaitForTemperatureMeasurement;
	return;
}
		
static void _WaitForSmoothingFinished(struct State* sm ) {
	//pre actions
	if (_Temperature_con_EnoughSamplesCollected()) {
		Temperature_FillSharedState();
		Temperature_PrepareNextMeasurementRound();
		sm->sleepable = 1;
		sm->nextState = _WaitForTemperatureMeasurement;
		return;
	}
	
	if (_Temperature_con_ShouldSmooth()) {
		Temperature_RequestTemperature();
		sm->sleepable = 0;
		sm->nextState = _WaitForTemperatureReceived;
		return;
	}

	sm->sleepable = 0;
	sm->nextState = _WaitForSmoothingFinished;
	return;
}
		
static void _WaitForTemperatureReceived(struct State* sm ) {
	//pre actions
	if (_Temperature_con_TemperatureReady()) {
		Temperature_SampleTemperature();
		Temperature_PrepareNextSmoothStep();
		sm->sleepable = 0;
		sm->nextState = _WaitForSmoothingFinished;
		return;
	}

	sm->sleepable = 0;
	sm->nextState = _WaitForTemperatureReceived;
	return;
}
		
	