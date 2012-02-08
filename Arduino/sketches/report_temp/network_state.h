#ifndef NETWORKSTATE_H
#define NETWORKSTATE_H
#include <stdint.h>
#include "network_init.h"
#include "statemachine.h"

StateMachineChoice initializeStateMachineNetwork(NetworkInitInformation* initData);
#endif
