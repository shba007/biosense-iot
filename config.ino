#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

// ENV Variables
String Hostname = "";
String SSID = "";
String Password = "";
PROGMEM String RootCACert = "";
String BrokerURL = "";
int BrokerPort = 0;
String ClientUsername = "";
String ClientPassword = "";

void setupVariables()
{
    Serial.println("Opening config.json file\n");
    File configFile = LittleFS.open("/config.json", "r");
    if (!configFile)
    {
        Serial.println("Failed to open config.json file");
        return;
    }

    // Allocate a buffer for the JSON data
    size_t size = configFile.size();
    std::unique_ptr<char[]> buffer(new char[size]);

    // Read the contents of the config.json file into the buffer
    configFile.readBytes(buffer.get(), size);
    // Close the config file
    configFile.close();

    // Parse the JSON buffer into the DynamicJsonDocument
    DynamicJsonDocument doc(2048); // Adjust the size as needed
    DeserializationError error = deserializeJson(doc, buffer.get());

    // Check for parsing errors
    if (error)
    {
        Serial.println("Failed to parse config.json!");
        return;
    }

    // Use the data as needed
    // Retrieve values from the JSON document and assign them to global variables
    Hostname = doc["Hostname"].as<String>();
    SSID = doc["SSID"].as<String>();
    Password = doc["Password"].as<String>();
    RootCACert = doc["RootCACert"].as<String>();
    BrokerURL = doc["BrokerURL"].as<String>();
    BrokerPort = doc["BrokerPort"].as<int>();
    ClientUsername = doc["ClientUsername"].as<String>();
    ClientPassword = doc["ClientPassword"].as<String>();

    Serial.println("Hostname: " + Hostname);
    Serial.println("SSID: " + SSID);
    Serial.println("Password: " + Password);
    Serial.println("BrokerURL: " + BrokerURL);
    Serial.println("BrokerPort: " + String(BrokerPort));
    Serial.println("ClientUsername: " + ClientUsername);
    Serial.println("ClientPassword: " + ClientPassword);
    Serial.println("RootCACert: " + RootCACert);
    Serial.println();
}
