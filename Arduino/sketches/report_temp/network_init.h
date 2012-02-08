#ifndef NETWORKINIT_H
#define NETWORKINIT_H
#include "sharedstate.h"
#include <stdint.h>
typedef struct {
	uint32_t sendEvery;
	SharedState* sharedState;
} NetworkInitInformation;
#endif
