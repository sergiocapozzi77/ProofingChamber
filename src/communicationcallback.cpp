#include "communicationcallback.hpp"
#include "Arduino.h"

CommunicationCallbacks::CommunicationCallbacks()
{
}

void CommunicationCallbacks::onWrite(BLECharacteristic *characteristic)
{
    // retorna ponteiro para o registrador contendo o valor atual da caracteristica
    std::string rxValue = characteristic->getValue();
    // verifica se existe dados (tamanho maior que zero)
    if (rxValue.length() > 0)
    {
        String command = String(rxValue.c_str());
        Serial.println("*********");
        Serial.print("Received Value: ");
        Serial.println(command);

        // Do stuff based on the command received
        if (command == "ON")
        {
            Serial.print("Turning Oven On");
            // this->oven->SwitchOn();
        }

        if (command == "OFF")
        {
            Serial.print("Turning Oven Off");
            // this->oven->SwitchOff();
        }


        Serial.println();
        Serial.println("*********");
    }
}