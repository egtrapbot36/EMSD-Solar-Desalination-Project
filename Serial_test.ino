
//CODE FOR RECEIVING DATA
//int data = 0; //Initialized variable to store recieved data
//
//void setup() {
//  //Serial Begin at 9600 Baud 
//  Serial.begin(9600);
//}
//
//void loop() {
//  data = Serial.read(); //Read the serial data and store it
//  Serial.println(data);
//  delay(1000);
//}

//CODE FOR SENDING DATA
#include<SoftwareSerial.h> //Included SoftwareSerial Library
//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
SoftwareSerial s(3,1);

void setup() {
  //Serial S Begin at 9600 Baud
  s.begin(9600);
}

void loop() {
  //Write '123' to Serial
  s.write(123);
  delay(1000);
}
