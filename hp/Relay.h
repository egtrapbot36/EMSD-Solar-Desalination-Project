#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

#define MIN_RUN_TIME 500
#define MAX_RUN_TIME 10000

class Relay {
  
  private: 
    byte pin;
    bool is_on;
    unsigned long run_time;
    unsigned long start_time;

  public:
    Relay(byte pin);
    void setup_relay();
    bool is_running();
    void turn_on_for(unsigned long run_time);
    void turn_off();
    void update_relay();
};

#endif
