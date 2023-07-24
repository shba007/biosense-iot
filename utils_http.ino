#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* API = "https://api.chucknorris.io";

void fetch() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on

  if (WiFi.status() == WL_CONNECTED) {
    String timestamp = getTime();
    HTTPClient http;
    http.begin(String(API) + String("/jokes/random"));

    int httpResponseCode = http.GET();
    if (httpResponseCode == HTTP_CODE_OK) {
      String response = http.getString();
      // Parse JSON response
      DynamicJsonDocument jsonBuffer(1024);
      deserializeJson(jsonBuffer, response);
      String joke = jsonBuffer["value"].as<String>();

      String output = timestamp + ": " + joke;
      Serial.println(output);
    } else {
      Serial.print("Error HTTP status code: " + String(httpResponseCode));
    }

    http.end();
  }

  digitalWrite(LED_BUILTIN, LOW);  // turn the LED off
}