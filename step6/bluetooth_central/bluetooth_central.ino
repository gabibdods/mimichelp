#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// --- Configuration ---
#define MAX_DEVICES 3  // Maximum number of peripherals to connect to
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");
const String DEVICE_NAME = "ESP32-CENTRAL";

// --- Global Variables ---
// A list to hold pointers to the devices we want to connect to
std::vector<BLEAdvertisedDevice*> devicesFound; 
// A list of addresses we are already connected (or connecting) to, to prevent duplicates
std::vector<String> connectedAddresses; 

bool isScanning = false;

// --- Communicate with Laptop or Phone ---
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

// --- Callback for Data Reception ---
static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    String value = "";
    for (int i = 0; i < length; i++) {
        value += (char)pData[i];
    }
    
    // Identify which device sent the data
    String senderAddress = pBLERemoteCharacteristic->getRemoteService()->getClient()->getPeerAddress().toString().c_str();
    
    SerialBT.print(senderAddress);
    SerialBT.print(": ");
    SerialBT.println(value);
}

// --- Client Callback (Handles Disconnects) ---
class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    SerialBT.println("Connected to a new client!");
  }

  void onDisconnect(BLEClient* pclient) {
    SerialBT.println("Disconnected from a client.");
    // Note: To make this robust, you would need to remove the address 
    // from 'connectedAddresses' here so it can be re-discovered.
  }
};

// --- Connection Logic ---
bool connectToServer(BLEAdvertisedDevice* myDevice) {
    SerialBT.print("Forming a connection to ");
    SerialBT.println(myDevice->getAddress().toString().c_str());

    BLEClient* pClient = BLEDevice::createClient();
    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remote BLE Server.
    if (!pClient->connect(myDevice)) {
        SerialBT.println(" - Failed to connect to server");
        return false;
    }
    SerialBT.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
        SerialBT.print(" - Failed to find our service UUID: ");
        SerialBT.println(serviceUUID.toString().c_str());
        pClient->disconnect();
        return false;
    }

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
        SerialBT.print(" - Failed to find our characteristic UUID: ");
        SerialBT.println(charUUID.toString().c_str());
        pClient->disconnect();
        return false;
    }

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
        String value = pRemoteCharacteristic->readValue();
        SerialBT.print(" - Initial Value: ");
        SerialBT.println(value.c_str());
    }

    // Register for notifications
    if(pRemoteCharacteristic->canNotify()) {
        pRemoteCharacteristic->registerForNotify(notifyCallback);
    }

    return true;
}

// --- Scanning Callback ---
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        // If we found a device with our UUID
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
            
            String tempAddr = advertisedDevice.getAddress().toString().c_str();
            
            // Check if we already know this device
            bool known = false;
            for(int i=0; i < connectedAddresses.size(); i++) {
                if (connectedAddresses[i] == tempAddr) {
                    known = true;
                    break;
                }
            }

            // If it's new and we have room, save it
            if (!known && connectedAddresses.size() < MAX_DEVICES) {
                SerialBT.print("New Device Found: ");
                SerialBT.println(tempAddr);
                
                // Save the address so we don't pick it up again immediately
                connectedAddresses.push_back(tempAddr);
                
                // Save the device object to connect to it in the loop
                // We make a copy using 'new' because the advertisedDevice object is temporary
                devicesFound.push_back(new BLEAdvertisedDevice(advertisedDevice));
            }
        }
    }
};

void setup() {
    SerialBT.begin(DEVICE_NAME);

    BLEDevice::init(DEVICE_NAME);

    // Setup Scanning
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
}

void loop() {
    // 1. Logic to connect to any newly found devices
    // We do this in the loop (not the callback) to avoid blocking the scan
    while (!devicesFound.empty()) {
        BLEAdvertisedDevice* device = devicesFound.back();
        devicesFound.pop_back(); // Remove from queue
        
        if (connectToServer(device)) {
            SerialBT.println("Success! Listening for data...");
        } else {
            SerialBT.println("Failed to connect.");
        }
        
        // Delete the object we created with 'new' to free memory
        // (The BLEClient makes its own internal copy if connected)
        delete device; 
    }

    // 2. Logic to keep scanning if we haven't reached our limit
    if (connectedAddresses.size() < MAX_DEVICES) {
        if (!isScanning) {
            BLEDevice::getScan()->start(5, false); // Scan for 5 seconds
            isScanning = true;
        } else {
             // Check if scan is done? 
             // The library blocks for the duration of start(), so we just reset the flag here
             isScanning = false;
             // Optional: Add a small delay so we aren't scanning 100% of the time
             delay(1000); 
        }
    } else {
        // We are full, just wait and process data
        delay(1000); 
    }
}
