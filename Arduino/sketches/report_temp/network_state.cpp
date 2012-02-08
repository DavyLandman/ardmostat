#include "network_state.h"
#include "statemachine.h"
#include "network_init.h"
#include "network.h"
//#define getSchedule
#include <Arduino.h> 
#include <stdint.h>
#include "debuglog.h"


static StateMachineChoice shouldStartCommunication();

static StateMachineChoice wasConnectionCompletedSchedule() {
	recvSendPackets();
	if (hasScheduleBeenRecevied()) {
		printlnDebug("Receiving succeeded");
		fillSharedScheduleState();

		sleepEthernet();
		initNextCommunicationRound();
		return Choice(shouldStartCommunication);
	}
	else if (hasScheduleReceivingTimedOut()) {
		printlnDebug("Receiving timeout?");

		sleepEthernet();
		initNextCommunicationRound();
		return Choice(shouldStartCommunication);
	}
	else {
		return Choice(wasConnectionCompletedSchedule);
	}
}
static StateMachineChoice wasConnectionCompletedTemperature() {
	recvSendPackets();
	if (hasTemperatureBeenSend()) {
#ifdef getSchedule
		printlnDebug("Sending succeeded, now retrieving schedule");
		initiateConnectionSchedule();
		return Choice(wasConnectionCompletedSchedule);
#else
		sleepEthernet();
		initNextCommunicationRound();
		return Choice(shouldStartCommunication);
#endif
	}
	else if (hasTemperatureReceivingTimedOut()) {
		printlnDebug("Sending timeout?");
		sleepEthernet();
		initNextCommunicationRound();
		return Choice(shouldStartCommunication);
	}
	else {
		return Choice(wasConnectionCompletedTemperature);
	}
}


static StateMachineChoice shouldStartCommunication() {
	if (hasWaitRoundPassed()) {
		wakeUpEthernet();
		initiateConnectionTemperature();
		return Choice(wasConnectionCompletedTemperature);
	}
	return Choice(shouldStartCommunication); 
}

StateMachineChoice initializeStateMachineNetwork(NetworkInitInformation* initData) {
	initializeNetwork(initData);
	return Choice(shouldStartCommunication);
}
