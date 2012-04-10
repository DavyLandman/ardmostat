#ifndef TEMPERATURELOG_H
#define TEMPERATURELOG_H
/***************************************
** This file is generated, do not edit! 
** You can edit SharedState.h
****************************************/
#include "SharedState.h"
#include "_SMEthernet.h"
#include "_SMTemperature.h"
#include <stdint.h>
#ifdef __cplusplus
extern "C"{
#endif	
void TemperatureLog_initialize(SharedState state);
void TemperatureLog_performStep();
int8_t TemperatureLog_canSleep();
#ifdef __cplusplus
}
#endif
#endif
	