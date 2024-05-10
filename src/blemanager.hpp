#ifndef BLEMANAGER_H
#define BLEMANAGER_H
#include <BLEServer.h>

#include "servercallbacks.hpp"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "ae4a7ea7-c717-4e71-9b89-26a5f9a3511b"                 // UART service UUID
#define CHARACTERISTIC_UUID_COMMANDS "6d60f0ce-af2e-4e58-8d24-66b4573807f2" // commands
#define CHARACTERISTIC_UUID_NOTIFY "5c3b815e-e5da-4486-8f18-2d2fadf43046"

#define OTA_SERVICE_UUID "d5d6fe08-1973-446d-8de2-f3ad8d99d7cb"               // OTA service UUID
#define CHARACTERISTIC_UUID_OTA "b181a940-0127-4bbc-88f8-101b643e7b7a"        // OTA update
#define CHARACTERISTIC_UUID_VERSION "36c6361e-882e-4f0e-ba3a-ee6f6bc35d60"    // device firmware version
#define CHARACTERISTIC_UUID_OTA_FINISH "ae1a7b5c-31ba-49f1-bbb4-869afed9e4b5" // OTA update

extern uint32_t FIRMWARE_VERSION; // device firmware version

class BLEManager
{
    ServerCallbacks *serverCallbaks;

    void CreateCommunicationCharactersitic(BLEService *service);
    void CreateOTACharacteristic(BLEService *service);

public:
    BLEManager();
    void SetupBLE();
    void SendNotification();
    bool IsDeviceConnected();
};

#endif