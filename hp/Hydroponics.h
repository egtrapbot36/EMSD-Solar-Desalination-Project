#ifndef HYDROPONICS_H
#define HYDROPONICS_H

#include <Arduino.h>
#include <Math.h>

#define RATIO_NA 0.5 // ratio of nutrient A
#define RATIO_NB 0.5 // ratio of nutrient B
#define PH_ACID 2.64
#define PH_BASE 10.55
//#define PH_N 7.0 // pH of nutrients
#define PH_TAR 6.5 // target pH
#define PH_ALLOW 0.3 // allowed one-side error range for pH
//#define PPM_ACID 1240.0
//#define PPM_BASE 1270.0
#define PPM_NA 107500.0 // PURPLE
#define PPM_NB 135000.0 // GREEN
#define PPM_TAR 1000.0 // cutting/seeding: 384-512; vegetative: 1024-1280; flowering/fruiting: 1024-1472
#define PPM_FLOOR 800.0 // add nutrients when PPM drop below this

// purple ppm: ~430000, green ppm: ~270000

#define FLOW_RATE 4.6 // volumetric flow rate of pump [mL/s]

double pH_to_H(double pH);
unsigned long v_to_t(double volume);
void set_motor_intervals();

#endif
