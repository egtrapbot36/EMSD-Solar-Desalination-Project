#include "Tracker.h"

// initialize reference luminance and resistance data
double l1 = 0.0001;
double l2 = 10.0;
double r1 = 3000000.0;
double r2 = 40000.0;
double A, B; // constant for model RL = A*LUX^B, LUX = (RL/A)^(1/B)
double R = 10000.0; // resistor in series with ldr

void setup_tracker() {
  // ldr
  pinMode(PIN_LDR_1, INPUT);
  pinMode(PIN_LDR_2, INPUT);
  pinMode(PIN_LDR_3, INPUT);
  pinMode(PIN_LDR_4, INPUT);
  A = pow(10, (log10(r2)*log10(l1)-log10(r1)*log10(l2))/(log10(l1)-log10(l2)));
  B = (log10(r1)-log10(r2))/(log10(l1)-log10(l2));
}

// returns the address of the array
double* get_lux() {
  double luxes[4];
  int pins[4] = { PIN_LDR_1, PIN_LDR_2, PIN_LDR_3, PIN_LDR_4 };
  for (int i=0; i<4; i++) {
    int reading = analogRead(pins[i]);
    double v = (double)reading/1023.0*5.0;
    double r = (5.0-v)/v*R;
    luxes[i] = pow(r/A, 1.0/B);
  }
  return &luxes[0];
}
