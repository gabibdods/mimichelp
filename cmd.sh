# ESP-WROOM-32

ac compile --fqbn esp32:esp32:esp32-poe-iso write_servo && ac upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32-poe-iso write_servo
ac compile --fqbn esp32:esp32:nodemcu-32s write_servo && ac upload -p /dev/ttyUSB0 --fqbn esp32:esp32:nodemcu-32s write_servo

ac compile --fqbn esp32:esp32:esp32-poe-iso read_servo && ac upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32-poe-iso read_servo && ac monitor -p /dev/ttyUSB0 -c baudrate=115200
ac compile --fqbn esp32:esp32:nodemcu-32s read_servo && ac upload -p /dev/ttyUSB0 --fqbn esp32:esp32:nodemcu-32s read_servo && ac monitor -p /dev/ttyUSB0 -c baudrate=115200

ac compile --fqbn esp32:esp32:nodemcu-32s read_potentio && ac upload -p /dev/ttyUSB0 --fqbn esp32:esp32:nodemcu-32s read_potentio && ac monitor -p /dev/ttyUSB0 -c baudrate=9600


# ESP32-DEVKIT

ac compile --fqbn esp32:esp32:nodemcu-32s ping && ac upload -p /dev/ttyUSB0 --fqbn esp32:esp32:nodemcu-32s ping
