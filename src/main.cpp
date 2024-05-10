#include <Arduino.h>
#include "blemanager.hpp"

BLEManager bleManager;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("Proofing Chamber starting"));
  bleManager.SetupBLE();
}

void loop()
{
  // put your main code here, to run repeatedly:
}
