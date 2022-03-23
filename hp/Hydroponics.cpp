#include "Hydroponics.h"

extern double v_cur;
extern double pH_cur;
extern double ppm_cur;
extern unsigned long motor_intervals[]; // {acid, base, nutrient A, nutrient B}

// calculate net concentration of H+; negative value denotes net OH-
// pH 7 returns 0, patched with smooth linear in the middle
double pH_to_H(double pH) {
  if (pH < 7.0-log10(M_E)) { //M_E = e
    return pow(10, -pH);
  } else if (pH <= 7+log10(M_E)) {
    return M_E*log(10)*pow(10, -7)*(7.0-pH);
  } else {
    return -pow(10, pH-14.0);
  }
}

// takes in volume of fluid in [mL] and return motor time in [ms]
unsigned long v_to_t(double volume) {
  return (unsigned long)(volume/FLOW_RATE*1000.0+0.5);
}

// calculate motor run time for 4 motors and store in global array
void set_motor_intervals() {
  double pH_add;
  double v_add;
  if (pH_cur >= PH_TAR) {
    pH_add = PH_ACID; // add acid
    v_add = (pH_to_H(PH_TAR)-pH_to_H(pH_cur))/(pH_to_H(pH_add)-pH_to_H(PH_TAR))*v_cur;
    motor_intervals[0] = v_to_t(v_add);
    motor_intervals[1] = 0; // no base added
  } else {
    pH_add = PH_BASE; // add base
    v_add = (pH_to_H(PH_TAR)-pH_to_H(pH_cur))/(pH_to_H(pH_add)-pH_to_H(PH_TAR))*v_cur;
    motor_intervals[0] = 0; // no acid added
    motor_intervals[1] = v_to_t(v_add);
  }
  
  // ignore nutrients for now
  motor_intervals[2] = 0;
  motor_intervals[3] = 0;
}
