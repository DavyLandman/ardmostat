#ifndef SHAREDSTATE_H
#define SHAREDSTATE_H
#include <stdint.h>
typedef struct {
	double currentTemperature;
	double currentTargetTemperature;
	uint8_t currentState;
	uint32_t lastNonce;
} SharedState;
#endif
