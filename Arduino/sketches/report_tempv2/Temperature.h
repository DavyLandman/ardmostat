#ifndef TEMPERATURE_H
#define TEMPERATURE_H
/***************************************
** This file is generated, do not edit! 
** You can edit SharedState.h or Temperature.cpp
****************************************/
#include "SharedState.h"
#include <stdint.h>
#ifdef __cplusplus
extern "C"{
#endif	
void Temperature_initialize(SharedState state );
void Temperature_FillSharedState();
void Temperature_InitializeSmoothing();
void Temperature_PrepareNextMeasurementRound();
void Temperature_PrepareNextSmoothStep();
void Temperature_RequestTemperature();
void Temperature_SampleTemperature();
uint8_t _Temperature_con_EnoughSamplesCollected();
uint8_t _Temperature_con_ShouldSmooth();
uint8_t _Temperature_con_ShouldStartMeasurement();
uint8_t _Temperature_con_TemperatureReady();
#ifdef __cplusplus
}
#endif
#endif
	