#ifndef NETWORK_H
#define NETWORK_H
#include <stdint.h>
#include "network_init.h"

void initializeNetwork(NetworkInitInformation* initData);

// state machine actions
void recvSendPackets();
void sleepEthernet();
void wakeUpEthernet();
void fillSharedScheduleState();
void initiateConnectionTemperature();
void initiateConnectionSchedule();

// conditions
uint_fast8_t hasScheduleBeenRecevied();
uint_fast8_t hasScheduleReceivingTimedOut();

uint_fast8_t hasTemperatureBeenSend();
uint_fast8_t hasTemperatureReceivingTimedOut();

// timing stuff
void initNextCommunicationRound();
uint_fast8_t hasWaitRoundPassed();
#endif
