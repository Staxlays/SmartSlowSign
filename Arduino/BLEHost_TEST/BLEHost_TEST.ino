#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;

void setup() {
  BLEDevice::init("ESP32-Server");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(BLEUUID("180D"));
  pCharacteristic = pService->createCharacteristic(
      BLEUUID("2A37"),
      BLECharacteristic::PROPERTY_READ |
      BLECharacteristic::PROPERTY_WRITE |
      BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->start();

  //pServer->setCallbacks(new MyServerCallbacks());
}

void loop() {
  // Update characteristic value
  // In this example, we'll leave it empty for simplicity
  delay(1000);
}

class MyServerCallbacks : public BLEServerCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        Serial.print("Received from client: ");
        for (int i = 0; i < value.length(); i++)
            Serial.print(value[i]);
        Serial.println();
    }
};