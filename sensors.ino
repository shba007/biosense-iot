// #include "./mqtt.ino"

#define MoisturePin 34
#define PumpPin 22

// Define the enumeration
enum PumpMode
{
    MANU,
    AUTO,
};

enum PumpState
{
    OFF,
    ON,
};

unsigned long lastMoistureUpdateTime = 0;
const unsigned long moistureUpdateInterval = 1 * 1000;
unsigned long lastLuminosityUpdateTime = 0;
const unsigned long luminosityUpdateInterval = 1 * 1000;

// Global Variables
float moisture = 0.0;   // 0.0 to 100.0
float luminosity = 0.0; // 0.0 to 100.0
bool pumpMode = false;  // false -> Manual, true -> Auto
bool pumpState = false; // false -> off, true -> on

float getLuminosity(int pin)
{
    float sensorValue = (analogRead(pin) / 4095.0) * 3.3;

    //   float luminosity = mapLuminosity(sensorValue);
    float luminosity = sensorValue;

    Serial.printf("Luminosity: %.2f lux\n", luminosity);
    return luminosity;
}
float getMoisture(int pin)
{
    float sensorValue = 100.0 - ((analogRead(pin) / 4095.0) * 100.0);

    //   float moisture = mapMoisture(sensorValue);
    float moisture = sensorValue;

    Serial.printf("Moisture: %.2f %\n", sensorValue);
    return moisture;
}

void pumpControl(bool newPumpState)
{
    pumpState = newPumpState;
    digitalWrite(PumpPin, pumpState ? HIGH : LOW);
}

void setupIO()
{
    pinMode(PumpPin, OUTPUT);
}

void inputIO()
{
    if (millis() - lastLuminosityUpdateTime >= luminosityUpdateInterval)
    {
        lastLuminosityUpdateTime = millis();
        luminosity = getMoisture(MoisturePin);
        publish("sensor/luminosity", luminosity, true);
    }
    if (millis() - lastMoistureUpdateTime >= moistureUpdateInterval)
    {
        lastMoistureUpdateTime = millis();
        moisture = getMoisture(MoisturePin);
        publish("sensor/moisture", moisture, true);
    }
}

void outputIO(String topic, String payload)
{
    if (topic == "pump/mode")
    {
        if (payload == "MANU")
            Serial.println("Pump mode changed to Manual");
        else if (payload == "AUTO")
            Serial.println("Pump mode changed to Auto");
        else
            Serial.println("Pump mode not changed");
    }
    else if (topic == "pump/state")
    {
        if (payload == "OFF")
        {
            Serial.println("Pump switched to OFF");
            pumpControl(false);
        }

        else if (payload == "ON")
        {
            Serial.println("Pump switched to ON");
            pumpControl(true);
        }
        else
        {
            Serial.println("Pump not switched");
            pumpControl(false);
        }
    }
    else
    {
        Serial.println("Unknown Topic");
    }
}