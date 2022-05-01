
/***************************************************
  Modified from Adafruit MQTT Library ESP8266 Example by Tony DiCola for Adafruit Industries.
****************************************************/

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point - From Adaruit example *********************************/

#define WLAN_SSID       "CMU-DEVICE"
#define WLAN_PASS       "manicbird197"

/************************* Adafruit.io Setup - From Adaruit Example *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "eguevara360"
#define AIO_KEY         "aio_XnDo05tXNZ0wk4eDRiN0GPmGg3sy"

/************ Global State - From Adafruit example ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds - Modified from Adafruit Example ***************************************/

// Setup a feed for subscribing to changes to relay states
Adafruit_MQTT_Subscribe Relay1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Motor1");
Adafruit_MQTT_Subscribe Relay2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Motor2");
Adafruit_MQTT_Subscribe Relay3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Motor3");
Adafruit_MQTT_Subscribe Relay4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Motor4");

Adafruit_MQTT_Publish ConductivitySensor = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Conductivity");

/**** LED Strip Setup - Modified from Fast.LED library Example***/

#define RELAY1_PIN D0
#define RELAY2_PIN D1
#define RELAY3_PIN D2
#define RELAY4_PIN D3

String relay1 = "0";
String relay2 = "0";
String relay3 = "0";
String relay4 = "0";

/************************* Conductivity Sensor Setup *********************************/

float temp = 25;
float tdsValue = 0;

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

//updates Relay 1 State based on Adafruit.io Motor1 feed
void activatecallback1(char *data, uint16_t len) {
  relay1 = String(data);

  if (relay1 == "1") {
    digitalWrite(RELAY1_PIN, LOW);
  }

  else digitalWrite(RELAY1_PIN, HIGH);
}

//updates Relay 2 State based on Adafruit.io Motor2 feed
void activatecallback2(char *data, uint16_t len) {
  relay2 = String(data);

  if (relay2 == "1") {
    digitalWrite(RELAY2_PIN, LOW);
  }

  else digitalWrite(RELAY2_PIN, HIGH);
}

//updates Relay 3 State based on Adafruit.io Motor3 feed
void activatecallback3(char *data, uint16_t len) {
  relay3 = String(data);

  if (relay3 == "1") {
    digitalWrite(RELAY3_PIN, LOW);
  }

  else digitalWrite(RELAY3_PIN, HIGH);
}

//updates Relay 4 State based on Adafruit.io Motor4 feed
void activatecallback4(char *data, uint16_t len) {
  relay4 = String(data);

  if (relay4 == "1") {
    digitalWrite(RELAY4_PIN, LOW);
  }

  else digitalWrite(RELAY4_PIN, HIGH);
}

void setup() {
  Serial.begin(115200);
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
  
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  digitalWrite(RELAY3_PIN, HIGH);
  digitalWrite(RELAY4_PIN, HIGH);

  Relay1.setCallback(activatecallback1);
  Relay2.setCallback(activatecallback2);
  Relay3.setCallback(activatecallback3);
  Relay4.setCallback(activatecallback4);

  // Setup MQTT subscription for motor feeds.
  mqtt.subscribe(&Relay1);
  mqtt.subscribe(&Relay2);
  mqtt.subscribe(&Relay3);
  mqtt.subscribe(&Relay4);

}

uint32_t x = 0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();
  
  // this is our 'wait for incoming subscription packets and callback em' busy subloop
  // try to spend your time here:
  
  Serial.print("Relay1: ");
  Serial.println(relay1);
  Serial.print("Relay2: ");
  Serial.println(relay2);
  Serial.print("Relay3: ");
  Serial.println(relay3);
  Serial.print("Relay4: ");
  Serial.println(relay4);

  Serial.print("Conductivity: ");
  Serial.print(tdsValue,0);
  Serial.println("ppm");
  Serial.println();
  ConductivitySensor.publish(tdsValue);

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
