#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// #include "./config.ino"
// #include "./io.ino"

WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);

char MQTTpayload[16];

void setupMQTT()
{
    wifiClient.setCACert(RootCACert.c_str());
    mqttClient.setServer(BrokerURL.c_str(), BrokerPort);
    mqttClient.setCallback(subscribe);
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
    Hostname += " 2";

    Serial.print("Connecting to MQTT Broker " + BrokerURL + " as " + Hostname);

    while (!mqttClient.connected() && attemptCounter < 10)
    {
        if (mqttClient.connect(Hostname.c_str(), ClientUsername.c_str(), ClientPassword.c_str()))
        {
            Serial.println("\nConnected to MQTT Broker" + String(BrokerURL));

            // ALL Subscriptions
            mqttClient.subscribe("light/mode");
            mqttClient.subscribe("light/state");
            mqttClient.subscribe("spray/mode");
            mqttClient.subscribe("spray/amount");
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

void subscribe(char *_topic, byte *_payload, unsigned int length)
{
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on

    char payloadArray[length + 1];
    memcpy(payloadArray, _payload, length);
    payloadArray[length] = '\0';

    String topic(_topic);
    String payload(payloadArray);

    Serial.println("Received [" + topic + "]: " + payload);

    outputIO(topic, payload);

    digitalWrite(LED_BUILTIN, LOW); // Turn the LED off
}

// Overloaded version for int data type
void publish(const char *topic, const int data, bool retained = false)
{
    char buffer[16]; // Adjust the size as needed
    itoa(data, buffer, 10);
    mqttClient.publish(topic, buffer, retained);
}

// Overloaded version for float data type
void publish(const char *topic, const float data, bool retained = false)
{
    char buffer[16];             // Adjust the size as needed
    dtostrf(data, 0, 2, buffer); // Convert float to string with 2 decimal places
    mqttClient.publish(topic, buffer, retained);
}

// Overloaded version for bool data type
void publish(const char *topic, const bool data, bool retained = false)
{
    const char *payload = data ? "true" : "false";
    mqttClient.publish(topic, payload, retained);
}

// Overloaded version for String data type
void publish(const char *topic, const String &data, bool retained = false)
{
    mqttClient.publish(topic, data.c_str(), retained);
}