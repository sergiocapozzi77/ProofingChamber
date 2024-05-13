#include "BLEDevice.h"

#ifndef BLECLIENT_H
#define BLECLIENT_H

class MyBLEClient
{

 
public:
    // The remote service we wish to connect to.
    BLEUUID serviceUUID;
    // The characteristic of the remote service we are interested in.
    BLEUUID charUUID;
   bool doConnect = false;
    bool doScan = false;
    BLERemoteCharacteristic *pRemoteCharacteristic;
    BLEAdvertisedDevice *myDevice;
    bool connected = false;
    bool ConnectToServer();
    void Setup();
    void Loop();
};

#endif