#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include "statemachine.h"
#include "sharedstate.h"

StateMachineChoice initializeTemperature(unsigned long measureEvery, SharedState* sharedState);
#endif
