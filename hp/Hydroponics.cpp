#include "Hydroponics.h"

extern double v_cur;
extern double pH_cur;
extern double ppm_cur;

extern unsigned long acid_motor_interval; 
extern unsigned long base_motor_interval; 
extern unsigned long nA_motor_interval; 
extern unsigned long nB_motor_interval; 
//extern unsigned long salt_motor_interval; 
//extern unsigned long fresh_motor_interval; 

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
  // acid and base
  double pH_add;
  double v_add_ab; // added volume for acid / base
  if (pH_cur >= PH_TAR+PH_ALLOW) {
    pH_add = PH_ACID; // add acid
    v_add_ab = (pH_to_H(PH_TAR)-pH_to_H(pH_cur))/(pH_to_H(pH_add)-pH_to_H(PH_TAR))*v_cur;
    acid_motor_interval = v_to_t(v_add_ab);
    base_motor_interval = 0; // no base added
  } else if(pH_cur <= PH_TAR-PH_ALLOW){
    pH_add = PH_BASE; // add base
    v_add_ab = (pH_to_H(PH_TAR)-pH_to_H(pH_cur))/(pH_to_H(pH_add)-pH_to_H(PH_TAR))*v_cur;
    acid_motor_interval = 0; // no acid added
    base_motor_interval = v_to_t(v_add_ab);
  } else {
    acid_motor_interval = 0; 
    base_motor_interval = 0; // don't do anything
  }

  // nutrients
  double ppm_add = RATIO_NA*PPM_NA + RATIO_NB*PPM_NB;
  double v_add_n = 0; // added volume for nutrients
  if (ppm_cur < PPM_FLOOR) {
    v_add_n = v_cur*(PPM_TAR-ppm_cur)/(ppm_add-PPM_TAR);
  }
  nA_motor_interval = v_to_t(v_add_n*RATIO_NA);
  nB_motor_interval = v_to_t(v_add_n*RATIO_NB);
  
  // ignore water intake for now
//  salt_motor_interval = 0;
//  fresh_motor_interval = 0;
}
