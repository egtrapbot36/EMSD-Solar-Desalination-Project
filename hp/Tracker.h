#ifndef TRACKER_H
#define TRACKER_H

#include <Arduino.h>

#define PIN_LDR_1 A3 // ldr at n-th quadrant 
#define PIN_LDR_2 A4
#define PIN_LDR_3 A5
#define PIN_LDR_4 A6

void setup_tracker();
double* get_lux();

#endif
