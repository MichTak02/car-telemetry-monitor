#include "GPS.h"

bool GPS::init()
{
    Serial2.begin(9600);
    return true;
}

void GPS::readData()
{
    static size_t ptr = 0;
    bool printed = false;

    while (Serial2.available() > 0)
    {
        char read = Serial2.read();
        buff[ptr++] = read;
        // Serial.println(read);
        if (_gps.encode(read)) {
            printed = true;

            buff[ptr] = '\0';
            // Serial.print(buff);
            ptr = 0;
        }
    }

    // TODO smazat výpisy
    if (_gps.satellites.isValid() && _gps.satellites.isUpdated()) {
        Serial.print("Sattelites: ");
        Serial.println(_gps.satellites.value());
    }

    if (_gps.location.isValid() && _gps.location.isUpdated()) {
        Serial.println("Position:");
        Serial.println(_gps.location.lat());
        Serial.println(_gps.location.lng());
    }
}
