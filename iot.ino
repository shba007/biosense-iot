#define LED_BUILTIN 13

unsigned long lastFetchTime = 0;
const unsigned long fetchInterval = 10 * 1000; // Fetch joke every 10 seconds

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  delay(1000);

  connectToWiFi();
  setTime();
  setupMQTTClient();
}

void loop()
{
  // connectToBroker();

  if (millis() - lastFetchTime >= fetchInterval)
  {
    fetch();
    lastFetchTime = millis();
  }
}