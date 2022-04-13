#ifndef Sensors_H
#define Sensors_H

#include <Arduino.h>
#include <Math.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "GravityTDS.h" //https://github.com/DFRobot/GravityTDS/blob/master/GravityTDS.cpp
// https://how2electronics.com/tds-sensor-arduino-interfacing-water-quality-monitoring/

#define ONE_WIRE_BUS 2
#define PIN_WATER_LEVEL A0
#define PIN_TDS A1
#define PIN_PH A2

#define SERIESRESISTOR 550 // for water level sensor
#define TANK_AREA 70.9 // square centimeter, multiply by water level [cm] to get volume [mL]
#define AVG_COUNT 10 // number of measurements to take to average
#define DISCARD_COUNT 2 // number of measurements to discard (2 each for too big/small)

void setup_sensors();

double get_temp();
double get_volume();
double get_pH();
double get_ppm();
double smooth_avg();

void update_sensors();

#endif
