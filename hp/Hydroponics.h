#ifndef HYDROPONICS_H
#define HYDROPONICS_H

#include <Arduino.h>
#include <Math.h>

#define RATIO_A 0.5 // ratio of nutrient A
#define RATIO_B 0.5 // ratio of nutrient B
#define PH_ACID 2.0
#define PH_BASE 12.0
#define PH_N 7.0 // pH of nutrients
#define PH_TAR 6.5
#define PPM_ACID 1240.0
#define PPM_BASE 1270.0
#define PPM_N 289.0
#define PPM_TAR 500 // cutting/seeding: 384-512; vegetative: 1024-1280; flowering/fruiting: 1024-1472

#define FLOW_RATE 4.6 // volumetric flow rate of pump [mL/s]

double pH_to_H(double pH);
unsigned long v_to_t(double volume);
void set_motor_intervals();

#endif
