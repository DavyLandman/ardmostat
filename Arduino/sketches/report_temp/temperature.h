#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include <stdint.h>
#include "statemachine.h"
#include "sharedstate.h"

StateMachineChoice initializeTemperature(uint32_t measureEvery, SharedState* sharedState);
#endif
