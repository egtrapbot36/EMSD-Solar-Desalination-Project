#include "Sensors.h"

extern double v_cur;
extern double pH_cur;
extern double ppm_cur;
extern double temp_cur;

double buffer_array[AVG_COUNT]; //temperary array for averaging

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
GravityTDS gravityTds;


void setup_sensors()
{
  // temperature sensor
  sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement

  // tds [ppm] (conductivity)
  gravityTds.setPin(PIN_TDS);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization

  // pH
  pinMode(PIN_PH, INPUT);

  // water level
  pinMode(PIN_WATER_LEVEL, INPUT);
}

// Celcius
double get_temp() {
  for (int i=0; i<AVG_COUNT; i++) {
    sensors.requestTemperatures(); // Send the command to get temperatures
    buffer_array[i] = sensors.getTempCByIndex(0);
  }
  return smooth_avg();
}

// mL
double get_volume() {
  // height[cm] = -0.136*r+113 // TUNE
  double r; // resistance of strip
  double height; // [cm]
  for (int i=0; i<AVG_COUNT; i++) {
    r = SERIESRESISTOR/(1023.0/(double)analogRead(PIN_WATER_LEVEL)-1); // voltage divider
//    height = -0.136*r+113; // [cm]
    height = -r/60+26.39;
    if (height < 0) height = 0;
    buffer_array[i] = height*TANK_AREA; // [mL]
//    buffer_array[i] = r; // for testing and calibrating
  }
  return 470; // wrong
//  return smooth_avg();
}

double get_pH() {
  double voltage;
  for (int i=0; i<AVG_COUNT; i++) {
    voltage = (double)analogRead(PIN_PH)/1023.0*5.0;
    buffer_array[i] = 3.0*voltage+1.5;
  }
  return smooth_avg();
}

double get_ppm() {
  gravityTds.setTemperature(temp_cur);
  for (int i=0; i<AVG_COUNT; i++) {
    gravityTds.update();
    buffer_array[i] = gravityTds.getTdsValue();
  }
  return smooth_avg();
}

void update_sensors() {
  v_cur = get_volume();
  temp_cur = get_temp();
  ppm_cur = get_ppm();
  pH_cur = get_pH();
}

// put stuff in buffer array first, than call this function to get avg (eg 6 out of 10)
double smooth_avg() {
  double total = 0;
  for (int i=0; i<AVG_COUNT-1; i++) { // selection sort
    for (int j=i+1; j<AVG_COUNT; j++) {
      if (buffer_array[i]>buffer_array[j]) {
        double temp = buffer_array[i];
        buffer_array[i] = buffer_array[j];
        buffer_array[j] = temp;
      }
    }
  }
  for (int k=DISCARD_COUNT; k<AVG_COUNT-DISCARD_COUNT; k++) {
    total += buffer_array[k];
  }
  return total/(AVG_COUNT-2.0*DISCARD_COUNT);
}
