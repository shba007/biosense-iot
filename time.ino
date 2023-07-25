#include <time.h>
// #include <TZ.h>
#define TZ_Asia_Kolkata 19800
#define DS_Asia_Kolkata 0

long timezone = TZ_Asia_Kolkata;
byte daysavetime = DS_Asia_Kolkata;

// Configuring time using NTP
void setupTime()
{
    Serial.print("Syncing Time Server");
    configTime(timezone, daysavetime, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");

    // Wait for NTP time synchronization
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2)
    {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }

    Serial.println("\nTime Synced: " + String(getTime()));
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
