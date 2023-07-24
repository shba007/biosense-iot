#include <WiFiClientSecure.h>

const char *Hostname = "ESP32";

const char *SSID = "Neuralink";
const char *Password = "Neuron@2718";

IPAddress connectToWiFi()
{
  int attemptCounter = 0;
  // Set the custom hostname
  WiFi.setHostname(Hostname);
  WiFi.mode(WIFI_STA);

  Serial.print("Connecting to WiFi " + String(SSID) + " as " + String(Hostname));
  while (WiFi.status() != WL_CONNECTED && attemptCounter < 10)
  {
    WiFi.begin(SSID, Password);
    Serial.print(".");

    attemptCounter++;
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nConnected to " + String(SSID));
    Serial.println("Local IP: " + WiFi.localIP().toString());
  }
  else
  {
    Serial.println("Unable to connect to " + String(SSID));
    Serial.println("Error Code: " + String(WiFi.status()));
  }

  return WiFi.localIP();
}