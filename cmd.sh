# ESP-WROOM-32

ac compile --fqbn esp32:esp32:esp32-poe-iso write_servo && ac upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32-poe-iso write_servo

ac compile --fqbn esp32:esp32:esp32-poe-iso read_servo && ac upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32-poe-iso read_servo && ac monitor -p /dev/ttyUSB0 -c baudrate=115200

ac compile --fqbn esp32:esp32:esp32-poe-iso null && ac upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32-poe-iso null && ac monitor -p /dev/ttyUSB0 -c baudrate=9600
