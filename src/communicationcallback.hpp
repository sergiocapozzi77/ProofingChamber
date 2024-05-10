#ifndef COMMUNICATIONCALLBACK_H
#define COMMUNICATIONCALLBACK_H

#include <BLEDevice.h>

// callback  para envendos das características
class CommunicationCallbacks : public BLECharacteristicCallbacks
{

public:
    CommunicationCallbacks();

    void onWrite(BLECharacteristic *characteristic);
};

#endif