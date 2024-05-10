#include "blemanager.hpp"
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "communicationcallback.hpp"

uint32_t FIRMWARE_VERSION = 6;

BLEManager::BLEManager()
{
    this->serverCallbaks = NULL;
}

bool BLEManager::IsDeviceConnected()
{
    if (this->serverCallbaks)
    {
        return false;
    }

    return this->serverCallbaks->IsDeviceConnected();
}

void BLEManager::SetupBLE()
{
    Serial.println("Setup BLE Comm");
    // Create the BLE Device
    BLEDevice::init("ProofingChamber");
    // Create the BLE Server
    BLEServer *server = BLEDevice::createServer(); // cria um BLE server
    this->serverCallbaks = new ServerCallbacks();
    server->setCallbacks(this->serverCallbaks); // seta o callback do server

    Serial.println("Setup BLE");
    // Create the BLE Service
    BLEService *service = server->createService(SERVICE_UUID);
    BLEService *otaService = server->createService(OTA_SERVICE_UUID);

    Serial.println("Create communication channel");
    this->CreateCommunicationCharactersitic(service);

    Serial.println("Create OTA characteristic");
    this->CreateOTACharacteristic(otaService);

    Serial.println("Starting service");
    // Start the service
    service->start();
    otaService->start();
    // Start advertising (descoberta do ESP32)
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->addServiceUUID(OTA_SERVICE_UUID);

    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    Serial.println("Start advertising");
    BLEDevice::startAdvertising();

    Serial.println("Waiting a client connection to notify...");
}

void BLEManager::SendNotification()
{
}

// for the communication with the app
void BLEManager::CreateOTACharacteristic(BLEService *service)
{
    BLECharacteristic *characteristic = service->createCharacteristic(
        CHARACTERISTIC_UUID_OTA,
        // BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE);
        BLECharacteristic::PROPERTY_WRITE);

    BLECharacteristic *characteristicOtaFinish = service->createCharacteristic(
        CHARACTERISTIC_UUID_OTA_FINISH,
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE);
    characteristicOtaFinish->addDescriptor(new BLE2902());

    // characteristic->setCallbacks(new OtaCallbacks(characteristicOtaFinish));

    BLECharacteristic *pVersionCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_UUID_VERSION,
        BLECharacteristic::PROPERTY_READ);

    char str[100];
    snprintf(str, 100, "{ firmware: %d}", FIRMWARE_VERSION);
    Serial.print("Firmware version: ");
    Serial.println(FIRMWARE_VERSION);
    pVersionCharacteristic->setValue(str);
}

// for the communication with the app
void BLEManager::CreateCommunicationCharactersitic(BLEService *service)
{
    BLECharacteristic *characteristic = service->createCharacteristic(
        CHARACTERISTIC_UUID_COMMANDS,
        BLECharacteristic::PROPERTY_WRITE);

    characteristic->setCallbacks(new CommunicationCallbacks());
}
