#include <SPI.h>
#include <LoRa.h>
#include <led.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Create the LED object and assign the connected pin value to it
LED LED(32);

int ledTimer;
bool enableLED = false;

void setup() {
  //initialize Serial Monitor
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
  display.println("LoRa Receiver");
  display.display();
  delay(1000);

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
  
  display.println("LoRa Initializing OK!");
  display.display();
  delay(2000);

  LED.initializeLED();
  
  display.clearDisplay();
  display.display();
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    //display.println("Received packet '");
    //display.display();

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData);
      
      display.println(LoRaData);
      display.display();
      
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
  display.clearDisplay();
  display.fillTriangle(64, 64, 128, 0, 0, 0, WHITE);
  display.setTextColor(BLACK);
  display.display();
}