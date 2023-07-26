// #include "./file.ino"
// #include "./config.ino"
// #include "./wifi.ino"
// #include "./time.ino"
// #include "./utils/http.h"
// #include "./mqtt.ino"

#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
    delay(1000);

    setupFiles();
    setupVariables();
    delay(1000);

    setupWiFi();
    setupTime();
    setupMQTT();
    setupIO();
}

void loop()
{
    // loopHTTP();
    loopMQTT();
    inputIO();
}