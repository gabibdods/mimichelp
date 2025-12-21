#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

bool doConnect = false;
BLEAdvertisedDevice* myDevice;
BLERemoteCharacteristic* pRemoteCharacteristic;

#define NAME "ESP32-MASTER"

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
    {
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
    }
  }
};

void setup()
{
  Serial.begin(115200);
  BLEDevice::init(NAME);
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
}

void loop()
{
  if (doConnect)
  {
    BLEClient* pClient = BLEDevice::createClient();
    if (pClient->connect(myDevice))
    {
      BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
      if (pRemoteService != nullptr)
      {
        pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
        if (pRemoteCharacteristic != nullptr)
        {
          //int localData = analogRead(34);
          Serial.print("Master_Data: ");
          Serial.println("Collected Data from master's joystick");

          String value = pRemoteCharacteristic->readValue();
          Serial.print("Slave_Data: ");
          Serial.println(value.c_str());
        }
      }
      //pClient->disconnect();
    }
    //doConnect = false;
    //BLEDevice::getScan()->start(5, false);
  }
  delay(1000);
}
