//Import BLE files
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEClient.h>

BLECharacteristic *pRemoteCharacteristic;

//set timer length to 3 seconds
#define timeSeconds 3
//set an alias for the PIR sensor pin
const int pir = 27;
//define timer variables
long now = millis();
long lastTrigger = 0;
bool startTimer = false;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(BLEUUID("180D"))) {
      BLEDevice::getScan()->stop();
      BLEClient *pClient = BLEDevice::createClient();
      pClient->connect(&advertisedDevice);
      BLERemoteService *pRemoteService = pClient->getServiceByUUID(BLEUUID("180D"));
      if (pRemoteService != nullptr) {
        pRemoteCharacteristic = pRemoteService->getCharacteristic(BLEUUID("2A37"));
        if (pRemoteCharacteristic != nullptr) {
          // Send a message to the server
          pRemoteCharacteristic->writeValue("Hello, server!", true);
        }
      }
    }
  }
};


void setup() {
  // open serial port communication
  Serial.begin(115200);
  //BLE Connection
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(10);
  //pin for PIR sensor set to pullup
  pinMode(pir, INPUT_PULLUP);
  //set PIR pin as a digital interrupt
  attachInterrupt(digitalPinToInterrupt(pir), detectsMovement, RISING);
}

void loop() {
  //set the current time
  now = millis();
  //check to see if the timer has run out
  if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
    //reset the timer and print a message
    startTimer = false;
    Serial.println("Start Timer Has Been Reset");
    pRemoteCharacteristic->writeValue("TO HOST: Start Timer Has Been Reset");
  }
}

//if motion is detected print a message, take not of when it was triggered, and enable the timer
void detectsMovement() {
  Serial.println("Motion Detected\n");
  startTimer = true;
  lastTrigger = millis();
}