#ifndef NETWORK_H
#define NETWORK_H
#include <stdint.h>
#include "statemachine.h"
#include "sharedstate.h"

StateMachineChoice initializeNetwork(uint32_t sendEvery, SharedState* sharedState);
#endif
