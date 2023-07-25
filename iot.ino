/* #include "./utils/file.h"
#include "./utils/config.h"
#include "./utils/wifi.h"
#include "./utils/time.h"
#include "./utils/http.h"
#include "./utils/mqtt.h"
 */
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
}

void loop()
{
    // loopHTTP();
    loopMQTT();
}