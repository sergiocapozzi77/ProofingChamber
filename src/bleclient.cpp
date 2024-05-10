#include "bleclient.hpp"
#include <Arduino.h>
/**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 */

#include "BLEDevice.h"
// #include "BLEScan.h"

MyBLEClient::MyBLEClient()
{

    serviceUUID = BLEUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
    charUUID = BLEUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");
}

static void notifyCallback(
    BLERemoteCharacteristic *pBLERemoteCharacteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify)
{
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    Serial.println((char *)pData);
}

class MyClientCallback : public BLEClientCallbacks
{
    MyBLEClient *myBLEClient;

    void onConnect(BLEClient *pclient)
    {
    }

    void onDisconnect(BLEClient *pclient)
    {
        myBLEClient->connected = false;
        Serial.println("onDisconnect");
    }

public:
    MyClientCallback::MyClientCallback(MyBLEClient *myBLEClient)
    {
        this->myBLEClient = myBLEClient;
    }
};

bool MyBLEClient::ConnectToServer()
{
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());

    BLEClient *pClient = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback(this));

    // Connect to the remove BLE Server.
    pClient->connect(myDevice); // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr)
    {
        Serial.print("Failed to find our service UUID: ");
        Serial.println(serviceUUID.toString().c_str());
        pClient->disconnect();
        return false;
    }
    Serial.println(" - Found our service");

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr)
    {
        Serial.print("Failed to find our characteristic UUID: ");
        Serial.println(charUUID.toString().c_str());
        pClient->disconnect();
        return false;
    }
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    if (pRemoteCharacteristic->canRead())
    {
        std::string value = pRemoteCharacteristic->readValue();
        Serial.print("The characteristic value was: ");
        Serial.println(value.c_str());
    }

    if (pRemoteCharacteristic->canNotify())
        pRemoteCharacteristic->registerForNotify(notifyCallback);

    connected = true;
    return true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    MyBLEClient *myBLEClient;
    /**
     * Called for each advertising BLE server.
     */
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        Serial.print("BLE Advertised Device found: ");
        Serial.println(advertisedDevice.toString().c_str());

        // We have found a device, let us now see if it contains the service we are looking for.
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(myBLEClient->serviceUUID))
        {
            BLEDevice::getScan()->stop();
            myBLEClient->myDevice = new BLEAdvertisedDevice(advertisedDevice);
            myBLEClient->doConnect = true;
            myBLEClient->doScan = true;

        } // Found our server
    } // onResult

public:
    MyAdvertisedDeviceCallbacks::MyAdvertisedDeviceCallbacks(MyBLEClient *myBLEClient)
    {
        this->myBLEClient = myBLEClient;
    }
}; // MyAdvertisedDeviceCallbacks

void MyBLEClient::Setup()
{
    BLEDevice::init("ProofingChamber");

    // Retrieve a Scanner and set the callback we want to use to be informed when we
    // have detected a new device.  Specify that we want active scanning and start the
    // scan to run for 5 seconds.
    BLEScan *pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(this));
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
} // End of setup.

// This is the Arduino main loop function.
void MyBLEClient::Loop()
{

    // If the flag "doConnect" is true then we have scanned for and found the desired
    // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
    // connected we set the connected flag to be true.
    if (doConnect == true)
    {
        if (ConnectToServer())
        {
            Serial.println("We are now connected to the BLE Server.");
        }
        else
        {
            Serial.println("We have failed to connect to the server; there is nothin more we will do.");
        }
        doConnect = false;
    }

    // If we are connected to a peer BLE Server, update the characteristic each time we are reached
    // with the current time since boot.
    // if (connected)
    // {
    //     String newValue = "Time since boot: " + String(millis() / 1000);
    //     Serial.println("Setting new characteristic value to \"" + newValue + "\"");

    //     // Set the characteristic's value to be the array of bytes that is actually a string.
    //     pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
    // }
    if (!connected && doScan)
    {
        BLEDevice::getScan()->start(0); // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
    }

} // End of loop