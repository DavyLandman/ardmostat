#ifndef _SMTEMPERATURE_H
#define _SMTEMPERATURE_H
/***************************************
** This file is generated, do not edit! 
** You can edit SharedState.h or Temperature.cpp
****************************************/
#include "SharedState.h"
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C"{
#endif	
void* SM_Temperature_initialize(SharedState state );
void SM_Temperature_takeStep(void* sm );
uint8_t SM_Temperature_isSleepableStep(const void* sm);
#ifdef __cplusplus
}
#endif
#endif
	