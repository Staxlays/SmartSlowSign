/*
This program is an experiment to see if I can get one sensor to activate another one.
The goal is to have a PIR sensor be tripped which leads to a sonar sensor to take a distance measurement.
If the new distance recorded varies from the previous one, calculate the speed.
*/

//Experimenting with modular development. Testing .h and .cpp files for PIR Sensor
#include <pirSensor.h>
#include <hcsr04.h>
//Include LoRa Library
#include <SPI.h>
#include <LoRa.h>

//LoRa: define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

pirSensor pir(27);
hcsr04 sonar(13, 12);

/*Want to add a timer to check how long it has been since the last measurement recorded.
If it has been long enough, forget the previous recording.
Instead of using a new timer variable, thought I may be able to implement this using the existing 
timeEcho and timeEchoPrev variables.

TRY TO IMPLEMENT IN YOUR IF ELSE STATEMENT BY COMPARING TIMEECHOPREV AND TIMEECHO AS YOUR CONDITIONALS
*/

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");
  pir.initializePir();
  sonar.initializeSonar();
  //LoRa: initialize LoRa pins
  LoRa.setPins(ss, rst, dio0);
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while(!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
  
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xD9);
  Serial.println("Lora Initializing OK!");
}

void loop() {
  if(pir.currentStatus() == 1){
    sonar.triggerSonar();
    if(sonar.currentStatus() == 1){
      while(sonar.currentStatus() == 1){
        //Serial.println("Waiting for the FALL"); //DEBUG STATEMENT
      }
      sonar.readDistance();
      sonar.calculateSpeed(sonar.distance, sonar.prevDistance, sonar.echoTime, sonar.prevEchoTime);

      //LoRa: Begin sending wireless message to receiver
      LoRa.beginPacket();
      LoRa.print("Current distance is: " + String(sonar.distance) + "\n");
      LoRa.print("Speed: " + String(sonar.speed));
      LoRa.endPacket();
      /*Serial.println("Current distance is: " + String(sonar.distance));
      sonar.calculateSpeed(sonar.distance, sonar.prevDistance, sonar.echoTime, sonar.prevEchoTime);
      Serial.println("Speed: " + String(sonar.speed));*/

    }
  }
}