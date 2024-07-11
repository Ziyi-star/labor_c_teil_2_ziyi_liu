#include "global_variables.h"

//for timer
uint16_t cnt = 0;
uint16_t m_second = 0;
char centi_second = 0;
char half_second = 0;
char second = 0;

//for run
int currentLap = 0;
time_t raceStartTime;
int isPaused = 0;
int isSessionActive = 0;
int isTurning = 0;
int isCompleted = 0;

