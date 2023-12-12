/*
This program is an experiment to see if I can get one sensor to activate another one.
The goal is to have a PIR sensor be tripped which leads to a dolphin sensor to take a distance measurement.
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
hcsr04 dolphin(13, 12);
hcsr04 bat(25,26);


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
  dolphin.initializeSonar();
  bat.initializeSonar();
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
  /*
  Okay so, I wanted to change the way I took speed by introducing a second hcsr04.
  Now, when the PIR is triggered, I try to take back to back distance measurements from
  each sensor with respect to time.
  Order of Events:
  1) PIR is tripped
  2) Dolphin activates to get a distance measurement
  3) Bat activates to get a distance measurements
  4) Their distances are compared with respect to time (tracked by millis at the moment)
    in order to obtain a speed value
  */
  if(pir.currentStatus() == 1){
    dolphin.triggerSonar();
    delay(60);
    if(dolphin.currentStatus() == 1){
      while(dolphin.currentStatus()){
        Serial.println("DOLPHIN Waiting for the FALL"); //DEBUG STATEMENT
        //delay(1);
      }
      dolphin.readDistance();
      //Serial.println("Dolphin distance in inches: " + String(dolphin.distance));
      //Serial.println("Dolphin triggerTime in mS: " + String(dolphin.triggerTime));
      //Serial.println("Dolphin echoTime in mS: " + String(dolphin.echoTime));
    }
    /*
    dolphin.readDistance();
    Serial.println("Dolphin distance in inches: " + String(dolphin.distance));
    Serial.println("Dolphin triggerTime in mS: " + String(dolphin.triggerTime));
    Serial.println("Dolphin echoTime in mS: " + String(dolphin.echoTime));
    */
      
    //LIGHT THE BAT SIGNAL
    bat.triggerSonar();
    delay(60);
    if(bat.currentStatus() == 1){
      while(bat.currentStatus()){
        Serial.println("BAT Waiting for the fall");
        //delay(1);
      }
      bat.readDistance();
      //Serial.println("Bat distance in inches: " + String(bat.distance));
      //Serial.println("Bat triggerTime in mS: " + String(bat.triggerTime));
      //Serial.println("Bat echoTime in mS: " + String(bat.echoTime));
    }
    
    /*
    bat.readDistance();
    Serial.println("Bat distance in inches: " + String(bat.distance));
    Serial.println("Bat triggerTime in mS: " + String(bat.triggerTime));
    Serial.println("Bat echoTime in mS: " + String(bat.echoTime));
    */
    Serial.println(bat.calculateSpeed(dolphin, bat));
    Serial.println("Dolphin distance in inches: " + String(dolphin.distance));
    Serial.println("Dolphin triggerTime in mS: " + String(dolphin.triggerTime));
    Serial.println("Dolphin echoTime in mS: " + String(dolphin.echoTime));
    Serial.println("Bat distance in inches: " + String(bat.distance));
    Serial.println("Bat triggerTime in mS: " + String(bat.triggerTime));
    Serial.println("Bat echoTime in mS: " + String(bat.echoTime));
    if(bat.calculateSpeed(dolphin, bat) >= 15){
      LoRa.beginPacket();
      LoRa.print("Speed: " + String(bat.calculateSpeed(dolphin, bat)));
      LoRa.print("LED ON");
      LoRa.endPacket();
    }

    //delay(2000);
      /*
      dolphin.calculateSpeed(dolphin.distance, dolphin.prevDistance, dolphin.echoTime, dolphin.prevEchoTime);

      //LoRa: Begin sending wireless message to receiver
      LoRa.beginPacket();
      LoRa.print("Current distance is: " + String(dolphin.distance) + "\n");
      LoRa.print("Speed: " + String(dolphin.speed));
      LoRa.endPacket();
      Serial.println("Current distance is: " + String(dolphin.distance));
      Serial.println("Previous distance is: " + String(dolphin.prevDistance));
      dolphin.calculateSpeed(dolphin.distance, dolphin.prevDistance, dolphin.echoTime, dolphin.prevEchoTime);
      Serial.println("Speed: " + String(dolphin.speed));
      */

  }
  
}