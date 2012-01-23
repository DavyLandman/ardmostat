#ifndef NETWORK_H
#define NETWORK_H
#include "statemachine.h"
#include "sharedstate.h"

StateMachineChoice initializeNetwork(unsigned long sendEvery, SharedState* sharedState);
#endif
