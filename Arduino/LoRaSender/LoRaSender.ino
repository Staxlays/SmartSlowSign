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
    //Trigger the first sonar
    dolphin.triggerSonar();
    
    //Adding a delay of 100ms between measurements for accuracy
    delay(100);

    //LIGHT THE BAT SIGNAL AKA trigger the second sonar
    bat.triggerSonar();
    
    //DEBUG for speed calculation
    Serial.println("Speed is: " + String(bat.calculateSpeed(dolphin, bat)) + "kmh");
    
    //DEBUG STATEMENTS
    Serial.println("Dolphin distance in cm: " + String(dolphin.distance));
    //Serial.println("Dolphin pulseTime in mS: " + String(dolphin.pulseTime));
    Serial.println("Dolphin echoTime in mS: " + String(dolphin.echoTime));
    Serial.println("Bat distance in cm: " + String(bat.distance));
    //Serial.println("Bat pulseTime in mS: " + String(bat.pulseTime));
    Serial.println("Bat echoTime in mS: " + String(bat.echoTime));

    //if speed is greater than .67cm/ms, 15mph, or 24.14kmh send a message to the receiver
    //sending a packet that contains the string 'LED' will activate the LED(s) connected to the receiver
    if(bat.calculateSpeed(dolphin, bat) >= 5){
      LoRa.beginPacket();
      LoRa.print("Speed: " + String(bat.calculateSpeed(dolphin, bat)) + "kmh LED ON");
      //LoRa.print("LED ON");
      LoRa.endPacket();
    }

    //wait for the hardware delay in the PIR sensor to pass
    while(pir.currentStatus()){};
  }
  
}