#ifndef SHAREDSTATE_H
#define SHAREDSTATE_H
#include <stdint.h>
#ifdef __cplusplus
extern "C"{
#endif	
//add your own fields to the struct 
typedef struct {
	float currentTemperature;
	uint32_t lastNonce;
} SharedStateInfo;
typedef SharedStateInfo* SharedState;
#ifdef __cplusplus
}
#endif	
#endif
		
