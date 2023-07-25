#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// #include "./config.h"

WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);

char MQTTpayload[16];

void setupMQTT()
{
    wifiClient.setCACert(RootCACert.c_str());
    mqttClient.setServer(BrokerURL.c_str(), BrokerPort);
    mqttClient.setCallback(callbackMQTT);
}

void callbackMQTT(char *topic, byte *payload, unsigned int length)
{
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on

    Serial.print("Message arrived [" + String(topic) + "]: ");
    if ((char)payload[0] != NULL)
    {
        for (int i = 0; i < length; i++)
            Serial.print((char)payload[i]);

        Serial.println();
        delay(500);
    }
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED off
}

void loopMQTT()
{
    if (!mqttClient.connected())
        reconnectMQTT();

    mqttClient.loop();
}

void reconnectMQTT()
{
    int attemptCounter = 0;

    Serial.print("Connecting to MQTT Broker " + BrokerURL + " as " + Hostname);

    while (!mqttClient.connected() && attemptCounter < 10)
    {
        if (mqttClient.connect(Hostname.c_str(), ClientUsername.c_str(), ClientPassword.c_str()))
        {
            Serial.println("\nConnected to " + String(BrokerURL));

            // ALL Subscriptions
            mqttClient.subscribe("pump/mode");
            mqttClient.subscribe("pump/state");
        }
        else
        {
            Serial.print(".");

            int state = mqttClient.state();
            switch (state)
            {
            case -4:
                Serial.println("Connection timeout");
                break;
            case -3:
                Serial.println("Connection lost");
                break;
            case -2:
                Serial.println("Connection failed");
                break;
            case -1:
                Serial.println("Connection disconnected");
                break;
            case 1:
                Serial.println("Connection refused, unacceptable protocol version");
                break;
            case 2:
                Serial.println("Connection refused, identifier rejected");
                break;
            case 3:
                Serial.println("Connection refused, server unavailable");
                break;
            case 4:
                Serial.println("Connection refused, bad username or password");
                break;
            case 5:
                Serial.println("Connection refused, not authorized");
                break;
            default:
                Serial.println("Connection failed, unknown reason");
                break;
            }

            attemptCounter++;
            delay(5000);
        }
    }

    Serial.println();
}

void publish(const char *topic, const int data, bool retained = false)
{
    itoa(data, MQTTpayload, 10);
    mqttClient.publish(topic, MQTTpayload, retained);
}