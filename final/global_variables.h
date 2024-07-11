#ifndef GLOBALS_H
#define GLOBALS_H
#include <time.h>

//for timer
extern uint16_t cnt;
extern uint16_t m_second;
extern char centi_second;
extern char half_second;
extern char second;

//for run
extern int currentLap;
extern time_t raceStartTime;
extern int isPaused;
extern int isSessionActive;
extern int isTurning;
extern int isCompleted;

#endif
