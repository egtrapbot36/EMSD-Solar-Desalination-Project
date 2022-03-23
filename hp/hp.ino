/*
Spring 2022 Electromechanical Systems Design Capstone Project
Team 1 -- Solar-Desalination-Hydroponics Project
Team members: Amen Assefa, Elio Bourcart, Emilio Guevara, Shitong (Michael) Pang
*/

#include "Hydroponics.h"
#include "Sensors.h"
#include "Tracker.h"
#include "Relay.h"

#define PIN_RELAY_ACID 4
#define PIN_RELAY_BASE 5
#define PIN_RELAY_NA 6
#define PIN_RELAY_NB 7

// System states used across files
double v_cur; // [mL]
double pH_cur;
double ppm_cur;
double temp_cur; 

Relay relay_acid(PIN_RELAY_ACID);
Relay relay_base(PIN_RELAY_BASE);
Relay relay_nA(PIN_RELAY_NA);
Relay relay_nB(PIN_RELAY_NB);

// interval and motor timing setup
unsigned long hp_interval = 300000; // 5 minutes
unsigned long hp_prev_mark = 0;
unsigned long cur_mark = 0; 

// motor run time in milliseconds {acid, base, nutrient A, nutrient B}
unsigned long motor_intervals[4] = { 0, 0, 0, 0 }; 

bool first_itr = true; // runs control loop once right after launch



void setup() {
  setup_sensors();
  setup_tracker();
  
  Serial.begin(9600);
}



void loop() {
  cur_mark = millis(); // update timer

  // get sensor readings and start motors every interval (and when the program starts)
  if (first_itr || (unsigned long)(cur_mark-hp_prev_mark) >= hp_interval) { 
    if (first_itr) first_itr = false;
    
    Serial.println("Updating Sensors ...");
    update_sensors();
    Serial.println("Done.\n");

    Serial.println("Calculating Actuation ...");
    set_motor_intervals();
    Serial.println("Done.\n");

    // run pumps
    relay_acid.turn_on_for(motor_intervals[0]);
    relay_base.turn_on_for(motor_intervals[1]);
    relay_nA.turn_on_for(motor_intervals[2]);
    relay_nB.turn_on_for(motor_intervals[3]);
    
    cur_mark = millis();
    hp_prev_mark = cur_mark; 
  }
  
  relay_acid.update_relay();
  relay_base.update_relay();
  relay_nA.update_relay();
  relay_nB.update_relay();

}
