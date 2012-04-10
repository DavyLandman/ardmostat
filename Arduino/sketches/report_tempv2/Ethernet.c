#include "Ethernet.h"
void Ethernet_initialize(SharedState state ) {
}

void Ethernet_DoPowerDownEthernet() {
}

void Ethernet_InitializeNextCommunicationRound() {
}

void Ethernet_InitializeTemperatureSending() {
}

void Ethernet_MarkTimeout() {
}

void Ethernet_RecvAndSendPackets() {
}

void Ethernet_WakeUpEthernet() {
}

uint8_t _Ethernet_con_HasTemperatureSendingTimeoutPassed() {
	return 1;
}

uint8_t _Ethernet_con_ShouldStartCommunication() {
	return 1;
}

uint8_t _Ethernet_con_WasTemperatureSend() {
	return 1;
}

	