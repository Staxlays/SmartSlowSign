/*
This program is an experiment to see if I can get one sensor to activate another one.
The goal is to have a PIR sensor be tripped which leads to a dolphin sensor to take a distance measurement.
If the new distance recorded varies from the previous one, calculate the speed.
*/

//Experimenting with modular development. Testing .h and .cpp files for HCSR04 and PIR Sensor
#include <pirSensor.h>
#include <hcsr04.h>
//Include LoRa Library
#include <SPI.h>
#include <LoRa.h>
//Include Library for OLED screen and I2C
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//LoRa: define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

//OLED Screen: Define the screen dimensions
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Declarations for other connected sensors
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
  
  //Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setRotation(10);  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  //display.setCursor(0, 10);
  display.println("Display Initialized");
  display.display();
  delay(1000);
  //display.clearDisplay();  
  display.println("LoRa Sender");
  display.display();
  delay(1000);
  //Initialize PIR and Sonars
  pir.initializePir();
  dolphin.initializeSonar();
  bat.initializeSonar();
  //Initialize LoRa
  Serial.println("LoRa Sender");
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

  display.clearDisplay();
  display.display();
}

void loop() {
  //Display a stand-by screen on the OLED
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Waiting");
  display.setCursor(0,20);
  display.println("For");
  display.setCursor(0, 40);
  display.println("Motion");
  display.display();
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
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("Motion Detected!");
    display.display();
    //Trigger the first sonar
    dolphin.triggerSonar();
    
    //Adding a delay of 100ms between measurements for accuracy
    delay(100);

    //LIGHT THE BAT SIGNAL AKA trigger the second sonar
    bat.triggerSonar();
    
    //Display the calculated speed on the OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.println("Speed:");
    display.setCursor(0,20);
    display.println(String(bat.calculateSpeed(dolphin, bat)));
    display.setCursor(0,40);
    display.println("KMH");
    display.display();

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
    if(bat.calculateSpeed(dolphin, bat) >= 2){
      LoRa.beginPacket();
      LoRa.print("Speed: " + String(bat.calculateSpeed(dolphin, bat)) + "kmh LED");
      //LoRa.print("LED ON");
      LoRa.endPacket();
      
      //display.fillTriangle(0, 0, 128, 0, 64, 64, 1);
      //display.set
    }
    display.clearDisplay();
    //wait for the hardware delay in the PIR sensor to pass
    while(pir.currentStatus()){};
  }
  
}