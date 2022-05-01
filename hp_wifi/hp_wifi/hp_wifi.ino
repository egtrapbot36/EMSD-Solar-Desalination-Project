
/***************************************************
  Modified from Adafruit MQTT Library ESP8266 Example by Tony DiCola for Adafruit Industries.
****************************************************/

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point - From Adaruit example *********************************/

#define WLAN_SSID       "Amen's hotspot"
#define WLAN_PASS       "manicbird197"

/************************* Adafruit.io Setup - From Adaruit Example *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "eguevara360"
#define AIO_KEY         "aio_EQAm30OgDyg7N3hw94PSknvUeCqz"

/************ Global State - From Adafruit example ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds - Modified from Adafruit Example ***************************************/

// Setup a feed for subscribing to changes to relay states
Adafruit_MQTT_Subscribe RelayAcid  = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay_acid");
Adafruit_MQTT_Subscribe RelayBase  = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay_base");
Adafruit_MQTT_Subscribe RelayNA    = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay_nA");
Adafruit_MQTT_Subscribe RelayNB    = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay_nB");
Adafruit_MQTT_Subscribe RelaySalt  = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay_salt");
Adafruit_MQTT_Subscribe RelayFresh = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay_fresh");

Adafruit_MQTT_Publish SensorBattery       = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Battery");
Adafruit_MQTT_Publish SensorConductivity  = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Conductivity");
Adafruit_MQTT_Publish SensorAcid          = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Acid");
Adafruit_MQTT_Publish SensorTemperature   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");

/*** System states used across files ***/

//double v_cur; // [mL]
double batt_cur; 
double pH_cur;
double ppm_cur;
double temp_cur; 

String relay_acid   = "0";
String relay_base   = "0";
String relay_nA     = "0";
String relay_nB     = "0";
String relay_salt   = "0";
String relay_fresh  = "0";

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

//  WiFi.begin(WLAN_SSID, WLAN_PASS);
  WiFi.begin(WLAN_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
}

uint32_t x = 0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();
  
  parseData();    //Read the serial data and store it
  
//  print_data();   //print out data for debugging
  publish_data(); //publish data to website
  
  mqtt.processPackets(10);

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
//    if (! mqtt.ping()) {
//      mqtt.disconnect();
//    }
    
  delay(5000);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}

void print_data() {
  
  Serial.println("-----------------");
  Serial.println("RelayAcid: " + String(relay_acid));
  Serial.println("RelayBase: " + String(relay_base));
  Serial.println("RelayNA: " + String(relay_nA));
  Serial.println("RelayNB: " + String(relay_nB));
  Serial.println("RelaySalt: " + String(relay_salt));
  Serial.println("RelayFresh: " + String(relay_fresh));

  Serial.println();
  Serial.println("Battery: " + String(batt_cur) + " V");
  Serial.println("pH: " + String(pH_cur));
  Serial.println("TDS: " + String(ppm_cur) + " ppm");
  Serial.println("Temperature: " + String(temp_cur) + " C");
}

void publish_data() {
  SensorBattery.publish(batt_cur);
  SensorConductivity.publish(pH_cur);
  SensorAcid.publish(ppm_cur);
  SensorTemperature.publish(temp_cur);
}

void parseData() {
//  data = Serial.read(); 
//  Serial.println(data);

  relay_acid   = Serial.parseInt(SKIP_ALL, '\n');
  relay_base   = Serial.parseInt(SKIP_ALL, '\n');
  relay_nA     = Serial.parseInt(SKIP_ALL, '\n');
  relay_nB     = Serial.parseInt(SKIP_ALL, '\n');
  relay_salt   = Serial.parseInt(SKIP_ALL, '\n');
  relay_fresh  = Serial.parseInt(SKIP_ALL, '\n');
  
  batt_cur  = Serial.parseFloat(SKIP_ALL, '\n');
  pH_cur    = Serial.parseFloat(SKIP_ALL, '\n');
  ppm_cur   = Serial.parseFloat(SKIP_ALL, '\n');
  temp_cur  = Serial.parseFloat(SKIP_ALL, '\n');
}
