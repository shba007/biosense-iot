// #include "./mqtt.ino"
#include <DHT.h>

#define BatteryLevelPin 18
#define WaterLevelPin 20

#define LuminosityPin 26
#define TempHumidityPin 25
#define AirQualityPin 33
#define MoisturePin 32

#define LightPin 19
#define SprayPin 21

DHT dht(TempHumidityPin, DHT22);

// Define the enumeration
enum SprayMode
{
    MANU,
    AUTO,
};

enum SprayState
{
    OFF,
    ON,
};

// Global Variables
float batteryLevel = 0.0; // 0.0 to 100.0
float waterLevel = 0.0;   // 0.0 to 100.0

float luminosity = 0.0;  // 0.0 to 1000.0
float temperature = 0.0; // -40.0 tp 125.0
float humidity = 0.0;    // 0.0 to 100.0
int airQuality = 0;      // 0 to 10000
float moisture = 0.0;    // 0.0 to 100.0

bool lightMode = false;  // false -> Manual, true -> Auto
bool lightState = false; // false -> off, true -> on
bool sprayMode = false;  // false -> Manual, true -> Auto
int sprayAmount = 0;     // false -> off, true -> on

float min_max[2] = {9999, 0};

unsigned long lastSensorUpdateTime = 0;
const unsigned long sensorUpdateInterval = 2.5 * 1000;
unsigned long lastControllerUpdateTime = 0;
const unsigned long ControllerUpdateInterval = 5 * 1000;

// Exponential function model
double exponentialMap(double x, double a, double b, double base)
{
    if (base * b * x > 20000)
        return -1;

    return a * pow(base, b * x);
}
// Linear function model
double linearMap(double x, double a, double b)
{
    return a * x + b;
}

float getLuminosity(int pin)
{
    int sensorValue = analogRead(pin);
    // Coefficients
    double a = 1567.854301007328;
    double b = 4.012950225958843;
    double base = 0.9995934601575271;

    float luminosity = exponentialMap(sensorValue, a, b, base);
    Serial.printf("Luminosity: %d ->  %.1f lux\n", sensorValue, luminosity);

    return luminosity;
}
float getTemperature(int pin)
{
    float temperature = dht.readTemperature();
    Serial.printf("Temperature: %.1f °C\n", temperature);

    return temperature;
}
float getHumidity(int pin)
{
    float humidity = dht.readHumidity();
    Serial.printf("Humidity: %.1f %%\n", humidity);

    return humidity;
}
int getAirQuality(int pin)
{
    int sensorValue = analogRead(pin);
    // Coefficients
    double a = 2.0;
    double b = 3.0;

    int airQuality = linearMap(sensorValue, a, b);
    Serial.printf("Air Quality: %d -> %d PPM\n", sensorValue, airQuality);
    return airQuality;
}
float getMoisture(int pin)
{
    int sensorValue = analogRead(pin);
    // Coefficients
    double a = -0.02442002442002442;
    double b = 100.0;

    double moisture = linearMap(sensorValue, a, b);
    Serial.printf("Moisture: %d -> %.1f %%\n", sensorValue, moisture);
    return moisture;
}

void lightControl(bool newSprayState)
{
    lightState = newSprayState;
    digitalWrite(LightPin, lightState ? HIGH : LOW);
}
void sprayControl(int newSprayAmount)
{
    sprayAmount = newSprayAmount;
    digitalWrite(SprayPin, HIGH);
    delay(600 * newSprayAmount);
    digitalWrite(SprayPin, LOW);
}

void setupIO()
{
    dht.begin();

    // pinMode(LuminosityPin, INPUT);
    // pinMode(TempHumidityPin, INPUT);
    // pinMode(AirQualityPin, INPUT);
    // pinMode(MoisturePin, INPUT);

    pinMode(LightPin, OUTPUT);
    pinMode(SprayPin, OUTPUT);
}

void inputIO()
{
    if (millis() - lastSensorUpdateTime >= sensorUpdateInterval)
    {
        lastSensorUpdateTime = millis();

        Serial.println();
        luminosity = getLuminosity(LuminosityPin);
        publish("sensor/luminosity", luminosity, true);
        temperature = getTemperature(TempHumidityPin);
        publish("sensor/temperature", temperature, true);
        humidity = getHumidity(TempHumidityPin);
        publish("sensor/humidity", humidity, true);
        airQuality = getAirQuality(AirQualityPin);
        publish("sensor/airquality", airQuality, true);
        moisture = getMoisture(MoisturePin);
        publish("sensor/moisture", moisture, true);
    }
}

void outputIO(String topic, String payload)
{
    if (topic == "light/mode")
    {
        if (payload == "MANU")
            Serial.println("Light mode changed to Manual");
        else if (payload == "AUTO")
            Serial.println("Light mode changed to Auto");
        else
            Serial.println("Light mode not changed");
    }
    else if (topic == "light/state")
    {
        if (payload == "OFF")
        {
            Serial.println("Light switched to OFF");
            lightControl(false);
        }

        else if (payload == "ON")
        {
            Serial.println("Light switched to ON");
            lightControl(true);
        }
        else
        {
            Serial.println("Light not switched");
            lightControl(false);
        }
    }
    else if (topic == "spray/mode")
    {
        if (payload == "MANU")
            Serial.println("Spray mode changed to Manual");
        else if (payload == "AUTO")
            Serial.println("Spray mode changed to Auto");
        else
            Serial.println("Spray mode not changed");
    }
    else if (topic == "spray/amount")
    {
        if (payload == "1")
        {
            Serial.println("Spray amount 1");
            sprayControl(1);
        }

        else if (payload == "2")
        {
            Serial.println("Spray amount 2");
            sprayControl(2);
        }
        else
            Serial.println("Spray not switched");
    }
    else
    {
        Serial.println("Unknown Topic");
    }
}