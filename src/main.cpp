#include <Arduino.h>
#include "blemanager.hpp"

unsigned long previousMillis = 0; // will store last time LED was updated
const long interval = 1000;
const uint8_t RELAY_PIN = 1;
const float desiredTemperature = 18.0;
const long minTimeBetweenOn = 60000;
unsigned long previousOn = -minTimeBetweenOn;

BLEManager bleManager;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("Proofing Chamber starting"));
  bleManager.SetupBLE();

  pinMode(RELAY_PIN, OUTPUT);
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval)
  {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    float currentTemperature = 11;

    if (currentTemperature > desiredTemperature && currentMillis - previousOn >= minTimeBetweenOn)
    {
      digitalWrite(RELAY_PIN, HIGH);
      previousOn = currentMillis;
    }
    else
    {
      digitalWrite(RELAY_PIN, LOW);
    }
  }
}
