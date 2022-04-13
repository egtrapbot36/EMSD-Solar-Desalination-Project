/*
Spring 2022 Electromechanical Systems Design Capstone Project
Team 1 -- Solar-Desalination-Hydroponics Project
Team members: Amen Assefa, Elio Bourcart, Emilio Guevara, Shitong (Michael) Pang
*/

#include "Hydroponics.h"
#include "Sensors.h"
#include "Relay.h"

#define PIN_RELAY_ACID 4
#define PIN_RELAY_BASE 5
#define PIN_RELAY_NA 6
#define PIN_RELAY_NB 7
#define PIN_RELAY_SALT 8

// System states used across files
double v_cur; // [mL]
double pH_cur;
double ppm_cur;
double temp_cur; 

Relay relay_acid(PIN_RELAY_ACID);
Relay relay_base(PIN_RELAY_BASE);
Relay relay_nA(PIN_RELAY_NA);
Relay relay_nB(PIN_RELAY_NB);
Relay relay_salt(PIN_RELAY_SALT);

// interval and timing setup
unsigned long cur_mark = 0; // current time

unsigned long sensor_interval = 30000; // sense data every 30 seconds
unsigned long sensor_prev_mark = 0;
unsigned long hp_interval = 120000; // 2 minutes
unsigned long hp_prev_mark = 0;
unsigned long salt_interval = 86400000; // 24 hours 
unsigned long salt_prev_mark = 0; // salt water intake

// motor run time in milliseconds
unsigned long acid_motor_interval = 0; 
unsigned long base_motor_interval = 0; 
unsigned long nA_motor_interval = 0; 
unsigned long nB_motor_interval = 0; 


bool testing = true;


void setup() {
  setup_sensors();
  Serial.begin(9600);
}



void loop() {
  if (testing) {
    test();
    return;
  }
  
  cur_mark = millis(); // update timer
  // update sensors
  if (sensor_prev_mark == 0 || (unsigned long)(cur_mark-sensor_prev_mark) >= sensor_interval) { 
    Serial.println("Updating Sensors ...");
    update_sensors();
    Serial.println("Done.\n");
    print_sensor_readings();
    
    cur_mark = millis();
    sensor_prev_mark = cur_mark; 
  }

  cur_mark = millis();
  // start motors 
  if (hp_prev_mark == 0 || (unsigned long)(cur_mark-hp_prev_mark) >= hp_interval) {
    Serial.println("Calculating Actuation ...");
    set_motor_intervals();
    Serial.println("Done.\n");
    print_motor_intervals();
    
    // run pumps (test/manual)
//    relay_acid.turn_on_for(2000);
//    relay_base.turn_on_for(4000);
//    relay_nA.turn_on_for(6000);
//    relay_nB.turn_on_for(8000);
    // run pumps (with algorithm)
    relay_acid.turn_on_for(acid_motor_interval);
    relay_base.turn_on_for(base_motor_interval);
    relay_nA.turn_on_for(nA_motor_interval);
    relay_nB.turn_on_for(nB_motor_interval);

    cur_mark = millis();
    hp_prev_mark = cur_mark;
  }

  cur_mark = millis();
  // refill desalination tank once a day
  if ((cur_mark-salt_prev_mark) >= salt_interval) {
    Serial.println("Adding Salt Water to Desalination Unit");
    relay_salt.turn_on_for(10000); // max amount ~46 mL, roughly the daily usage

    cur_mark = millis();
    salt_prev_mark = cur_mark;
  }

  // check if motors need to be stopped
  relay_acid.update_relay();
  relay_base.update_relay();
  relay_nA.update_relay();
  relay_nB.update_relay();
  relay_salt.update_relay();
}

void test() {
  temp_cur = 22.0;
  Serial.println(get_ppm());
}

void print_sensor_readings() {
  Serial.println("Sensor Readings (t = " + String(cur_mark) + " ms): ");
  Serial.println("Temperature: " + String(temp_cur) + " C");
  Serial.println("Volume: " + String(v_cur) + " mL");
  Serial.println("pH: " + String(pH_cur));
  Serial.println("TDS: " + String(ppm_cur) + " ppm");
  Serial.println();
}

void print_motor_intervals() {
  Serial.println("Running Pumps from t = " + String(cur_mark) + " ms: ");
  Serial.println("Acid: " + String(acid_motor_interval) + " ms: ");
  Serial.println("Base: " + String(base_motor_interval) + " ms: ");
  Serial.println("Nutrient A: " + String(nA_motor_interval) + " ms: ");
  Serial.println("Nutrient B: " + String(nB_motor_interval) + " ms: ");
//  Serial.println("Salt Water: " + String(salt_motor_interval) + " ms: ");
//  Serial.println("Fresh Water: " + String(fresh_motor_interval) + " ms: ");
  Serial.println();
}
