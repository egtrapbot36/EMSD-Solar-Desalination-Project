#include "Relay.h"

extern unsigned long cur_mark;

Relay::Relay(byte pin) {
  this->pin = pin;
  this->setup_relay();
}

void Relay::setup_relay() {
  pinMode(this->pin, OUTPUT);
  this->start_time = 0;
  this->turn_off();
}

bool Relay::is_running() {
  return this->is_on;
}

// turn motor on for ____ amount of time
void Relay::turn_on_for(unsigned long run_time) { 
  cur_mark = millis();
  if (run_time <= MIN_RUN_TIME) {
    run_time = MIN_RUN_TIME;
  } else if (run_time >= MAX_RUN_TIME) {
    run_time = MAX_RUN_TIME;
  }
  if (run_time == 0) return;
  this->run_time = run_time;
  this->start_time = cur_mark;
  digitalWrite(this->pin, LOW);
  this->is_on = true;
}

void Relay::turn_off() {
  digitalWrite(this->pin, HIGH);
  this->is_on = false;
}

void Relay::update_relay() {
  cur_mark = millis();
  if (this->is_on && (unsigned long)(cur_mark-this->start_time) >= this->run_time) {
    this->turn_off();
  }
}
