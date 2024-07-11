#ifndef TRACK_h
#define TRACK_h


#include <avr/io.h>
#include "iesusart.h"
#include "iesmotors.h"
#include "iesadc.h"
#include "global_variables.h"

// Konstanten für die PB- und PD-Pins
#define PIN_IN1_FORWARD_LEFT PD7
#define PIN_IN2_BACKWARD_LEFT PB0
#define PIN_IN3_BACKWARD_RIGHT PB1
#define PIN_IN4_FORWARD_RIGHT PB3

void turn_right();
void turn_left();
void drive_right();
void drive_left();
void gerade();
void init_run();
void stop();
void rotate_clockwise();
void handleDrivingLogic(int *left, int *middle, int *right, int *last_right, int *start, char * field_count);


#endif



