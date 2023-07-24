#include <time.h>

long timezone = 1;
byte daysavetime = 1;

// Configuring time using NTP
void setTime()
{
  Serial.println("Contacting Time Server");
  configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");

  time_t now;
  // To avoid false timestamps
  while (now < 8 * 3600)
  {
    delay(500);
    now = time(nullptr);
    Serial.print(".");
  }

  Serial.println("Time Synced: " + String(getTime()));
}

String getTime()
{
  struct tm tmstruct;
  tmstruct.tm_year = 0;
  getLocalTime(&tmstruct, 5000);
  char timestamp[30]; // Buffer to store the formatted timestamp

  snprintf(timestamp, sizeof(timestamp), "%d-%02d-%02d %02d:%02d:%02d",
           (tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1, tmstruct.tm_mday,
           tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);

  return timestamp;
}
