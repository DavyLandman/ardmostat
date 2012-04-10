#ifndef _SMETHERNET_H
#define _SMETHERNET_H
/***************************************
** This file is generated, do not edit! 
** You can edit SharedState.h or Ethernet.cpp
****************************************/
#include "SharedState.h"
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C"{
#endif	
void* SM_Ethernet_initialize(SharedState state );
void SM_Ethernet_takeStep(void* sm );
uint8_t SM_Ethernet_isSleepableStep(const void* sm);
#ifdef __cplusplus
}
#endif
#endif
	