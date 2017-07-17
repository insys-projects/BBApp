#ifndef DEVICE_CPP
#define DEVICE_CPP

#include "device.h"
#include "lib/bb_api.h"
#include "lib/sa_api.h"

QList<DeviceConnectionInfo> Device::GetDeviceList()
{
    QList<DeviceConnectionInfo> deviceList;
    DeviceConnectionInfo info;

    int serialNumbers[8];
    int deviceCount;

    info.series = SA_SERIES;
    saGetSerialNumberList(serialNumbers, &deviceCount);
    for(int i = 0; i < deviceCount; i++) {
        info.serialNumber = serialNumbers[i];
        deviceList.push_back(info);
    }

    info.series = BB_SERIES;
    bbGetSerialNumberList(serialNumbers, &deviceCount);
    for(int i = 0; i < deviceCount; i++) {
        info.serialNumber = serialNumbers[i];
        deviceList.push_back(info);
    }

    return deviceList;
}

#endif // DEVICE_CPP
