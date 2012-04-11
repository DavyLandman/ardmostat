#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H
/* 
 * We have 3 information levels
 * - Info, for general information about the program
 * - Error, for important failures
 * - Debug, debug statements
 *
 * And they can be globally enabled/disabled from this file
 **/
#define printInfoStuff
#define printErrorStuff
//#define printDebugStuff

#ifdef printInfoStuff
	#define printlnInfo(__p1) {Serial.print("info:   "); Serial.println((__p1));}
	#define printlnInfo2(__p1, __p2) {Serial.print("info:   "); Serial.println((__p1),(__p2));}
#else
	#define printlnInfo(__p1) 
	#define printlnInfo2(__p1, __p2) 
#endif

#ifdef printErrorStuff
	#define printlnError(__p1) {Serial.print("error: "); Serial.println((__p1));}
	#define printlnError2(__p1, __p2) {Serial.print("error: "); Serial.println((__p1),(__p2));}
#else
	#define printlnError(__p1) 
	#define printlnError2(__p1, __p2) 
#endif

#ifdef printDebugStuff
	#define printlnDebug(__p1) {Serial.print("debug: "); Serial.println((__p1));}
	#define printlnDebug2(__p1, __p2) {Serial.print("debug: "); Serial.println((__p1),(__p2));}
#else
	#define printlnDebug(__p1) 
	#define printlnDebug2(__p1, __p2) 
#endif
#endif
