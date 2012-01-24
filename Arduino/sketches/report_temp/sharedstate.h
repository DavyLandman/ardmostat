#ifndef SHAREDSTATE_H
#define SHAREDSTATE_H
#include <stdint.h>
typedef struct {
	double currentTemperature;
	uint8_t newTemperature;
} SharedState;
#endif
