# Biosense - IoT
IoT section of [Biosense](https://github.com/shba007/biosense-web)

arduino-cli monitor -p COM5 --baud 115200

### Compile the sketch
arduino-cli compile -b esp32:esp32:esp32doit-devkit-v1 --build-path ./build .
### Upload the sketch
arduino-cli upload -b esp32:esp32:esp32doit-devkit-v1 -p COM5 --input-dir ./build .
### Create the data binary
mkspiffs.exe -p 256 -b 4096 -s 1441792 -d 5 -c data build/files.bin
### Flash the LittleFS partition
esptool --chip esp32 -b 115200 -p COM5 write_flash 0x290000 build/files.bin

esptool --chip esp32 --port COM5 erase_flash

