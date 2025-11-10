/* Resources
https://github.com/matthieuvigne/STS_servos/blob/main/examples/SimpleMotion/SimpleMotion.ino
https://github.com/matthieuvigne/STS_servos/blob/main/src/STSServoDriver.h
https://github.com/matthieuvigne/STS_servos/blob/main/src/STSServoDriver.cpp
*/
#include <STSServoDriver.h>

// FE-URT-1 TXD Pin -> ESP32 TXD Pin
// FE-URT-1 RXD Pin -> ESP32 RXD Pin
// FE-URT-1 GND Pin -> ESP32 GND Pin
// FE-URT-1 5V  Pin -> ESP32 5V  Pin
// FE-URT-1 DTR Pin -> None

STSServoDriver servos;

void setup() {
  pinMode(13, OUTPUT); digitalWrite(13, LOW); if (!servos.init(2)) digitalWrite(13, HIGH);

  servos.setMode(0xFE, STSMode::POSITION);
}

void loop()
{
  servos.setPositionOffset(1, 2000);

  servos.setTargetPosition(1, 600);
  delay(100); // Wait for servo to start moving, then wait for end of motion
  while (servos.isMoving(1)) delay(50); // Wait a bit more to see it stop
  delay(500);

  servos.setTargetPosition(1, 2000);
  delay(100);
  while (servos.isMoving(1)) delay(50);
  delay(500);
}
