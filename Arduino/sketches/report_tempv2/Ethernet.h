#ifndef ETHERNET_H
#define ETHERNET_H
/***************************************
** This file is generated, do not edit! 
** You can edit SharedState.h or Ethernet.cpp
****************************************/
#include "SharedState.h"
#include <stdint.h>
#ifdef __cplusplus
extern "C"{
#endif	
void Ethernet_initialize(SharedState state );
void Ethernet_DoPowerDownEthernet();
void Ethernet_InitializeNextCommunicationRound();
void Ethernet_InitializeTemperatureSending();
void Ethernet_MarkTimeout();
void Ethernet_RecvAndSendPackets();
void Ethernet_WakeUpEthernet();
uint8_t _Ethernet_con_HasTemperatureSendingTimeoutPassed();
uint8_t _Ethernet_con_ShouldStartCommunication();
uint8_t _Ethernet_con_WasTemperatureSend();
#ifdef __cplusplus
}
#endif
#endif
	