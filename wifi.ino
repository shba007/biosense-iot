#include <WiFiClientSecure.h>

// #include "./config.ino"

IPAddress setupWiFi()
{
    int attemptCounter = 0;

    WiFi.setHostname(Hostname.c_str());
    // Set Wi-Fi mode to Station (Client) mode
    WiFi.mode(WIFI_STA);

    Serial.print("Connecting to WiFi " + SSID + " as " + Hostname);
    while (WiFi.status() != WL_CONNECTED && attemptCounter < 10)
    {
        WiFi.begin(SSID.c_str(), Password.c_str());
        Serial.print(".");

        attemptCounter++;
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.print("\nConnected to WiFi " + String(SSID));
        Serial.println(" [Local IP: " + WiFi.localIP().toString() + "] ");
    }
    else
    {
        Serial.print("\nUnable to connect to " + String(SSID));
        Serial.println(" [Error Code: " + String(WiFi.status()) + "] ");
    }

    return WiFi.localIP();
}