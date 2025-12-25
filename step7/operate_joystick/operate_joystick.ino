/* Sources:
 * https://randomnerdtutorials.com/esp32-bluetooth-classic-arduino-ide/
 */

#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;
String device_name = "ESP32-BT-Slave";
bool o = true;

void setup()
{
  Serial.begin(115200);           // esp32 (2): reads the potentiometer
  SerialBT.begin(device_name);    // esp32 (1): controls servers

  pinMode(26, INPUT);
  pinMode(25, INPUT);
  pinMode(33, INPUT);
}

void loop()
{
  if (Serial.available())
  {
    SerialBT.println(analogRead(25));
    SerialBT.println(analogRead(26));
    SerialBT.println(analogRead(33));

    while (Serial.available() > 0) Serial.read(); // Serial flush to stop reading buffer
  }

  delay(20);
}
