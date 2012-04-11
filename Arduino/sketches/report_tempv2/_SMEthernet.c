#include "_SMEthernet.h"
/***************************************
** This file is generated, do not edit! 
** You can edit SharedState.h or Ethernet.cpp
****************************************/
#include "Ethernet.h"
struct State;
typedef void (*StatePointer)(struct State* );
struct State {
	StatePointer nextState;
	uint8_t sleepable;
};
#ifdef __cplusplus
#define GETSMSTATE(pt) (reinterpret_cast<struct State*>(pt))
#else
#define GETSMSTATE(pt) ((struct State*)pt)
#endif
static void _WaitForCommunicationTimeOutPassed(struct State* sm );
static void _WaitForTemperatureSend(struct State* sm );
void* SM_Ethernet_initialize(SharedState state ) {
	Ethernet_initialize(state );
	struct State* result = GETSMSTATE(malloc(sizeof(struct State)));
	result->sleepable = 1;
	result->nextState = _WaitForCommunicationTimeOutPassed;
	return result; 
}

void SM_Ethernet_takeStep(void* sm ) {
	GETSMSTATE(sm)->nextState(GETSMSTATE(sm) );
}

uint8_t SM_Ethernet_isSleepableStep(const void* sm) {
	return GETSMSTATE(sm)->sleepable;
}

static void _WaitForCommunicationTimeOutPassed(struct State* sm ) {
	//pre actions
	if (_Ethernet_con_ShouldStartCommunication()) {
		Ethernet_WakeUpEthernet();
		Ethernet_InitializeTemperatureSending();
		sm->sleepable = 0;
		sm->nextState = _WaitForTemperatureSend;
		return;
	}

	sm->sleepable = 1;
	sm->nextState = _WaitForCommunicationTimeOutPassed;
	return;
}
		
static void _WaitForTemperatureSend(struct State* sm ) {
	//pre actions
	Ethernet_RecvAndSendPackets();
	if (_Ethernet_con_WasTemperatureSend()) {
		Ethernet_DoPowerDownEthernet();
		Ethernet_InitializeNextCommunicationRound();
		sm->sleepable = 1;
		sm->nextState = _WaitForCommunicationTimeOutPassed;
		return;
	}
	
	if (_Ethernet_con_HasTemperatureSendingTimeoutPassed()) {
		Ethernet_MarkTimeout();
		Ethernet_DoPowerDownEthernet();
		Ethernet_InitializeNextCommunicationRound();
		sm->sleepable = 1;
		sm->nextState = _WaitForCommunicationTimeOutPassed;
		return;
	}

	sm->sleepable = 0;
	sm->nextState = _WaitForTemperatureSend;
	return;
}
		
	
