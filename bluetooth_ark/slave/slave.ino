#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic* pCharacteristic;
//const int PIN = 34;

#define NAME "ESP32-SLAVE"

void setup()
{
  Serial.begin(115200);
  BLEDevice::init(NAME);
  
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
      BLECharacteristic::PROPERTY_NOTIFY
      );

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
  Serial.println("Slave is advertising...");
}

void loop()
{
  //int sensorValue = analogRead(PIN);
  //String data = String(sensorValue);
  String data = "it works";

  pCharacteristic->setValue(data.c_str());
  pCharacteristic->notify();

  delay(100);
}
