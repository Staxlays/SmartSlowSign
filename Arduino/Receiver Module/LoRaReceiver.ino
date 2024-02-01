/*********
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
*********/

#include <SPI.h>
#include <LoRa.h>
#include <led.h>

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

//Create the LED object and assign the connected pin value to it
LED LED(32);

int ledTimer;
bool enableLED = false;

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0XD9) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xD9);
  Serial.println("LoRa Initializing OK!");

  LED.initializeLED();
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData);
      //check if the message contains the LED keyphrase
      if(LoRaData.indexOf("LED" >= 0)){
        enableLED = true;
      } 
    }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
  //Enable LED if the message has the included keyphrase
  if(enableLED){
    //make the LED flash for three seconds
    LED.flashingLED();
    enableLED = false;
  }

}