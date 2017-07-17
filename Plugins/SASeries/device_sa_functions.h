#ifndef DEVICE_SA_FUNCTIONS_H
#define DEVICE_SA_FUNCTIONS_H

#include "device_uni_functions.h"

class DeviceSAFunctions: public DeviceUniFunctions
{
public:
	DeviceSAFunctions();
	virtual ~DeviceSAFunctions();

	virtual int GetSerialNumberList(int serialNumbers[8], int *deviceCount);
	virtual int OpenDeviceBySerialNumber(int *device, int serialNumber);
	virtual int OpenDevice(int *device);
	virtual int CloseDevice(int device);
	virtual int Preset(int device);

	virtual int GetSerialNumber(int device, int *serial);
	virtual int GetFirmwareString(int device, char firmwareString[16]);
	virtual int GetDeviceType(int device, int *device_type);
	virtual int ConfigAcquisition(int device, int detector, int scale);
	virtual int ConfigCenterSpan(int device, double center, double span);
	virtual int ConfigLevel(int device, double ref);
	virtual int ConfigGainAtten(int device, int atten, int gain, bool preAmp);
	virtual int ConfigSweepCoupling(int device, double rbw, double vbw, bool reject);
	virtual int ConfigProcUnits(int device, int units);
	virtual int ConfigIQ(int device, int decimation, double bandwidth);
	virtual int ConfigAudio(int device, int audioType, double centerFreq,
		double bandwidth, double audioLowPassFreq,
		double audioHighPassFreq, double fmDeemphasis);
	virtual int ConfigRealTime(int device, double yScale, int frameRate);

	virtual int Initiate(int device, int mode, int flag);
	virtual int Abort(int device);

	virtual int QuerySweepInfo(int device, int *sweepLength, double *startFreq, double *binSize);
	virtual int QueryStreamInfo(int device, int *returnLen, double *bandwidth, double *samplesPerSecond);
	virtual int QueryRealTimeFrameInfo(int device, int *imageWidth, int *imageHeight);
	virtual int GetSweep_32f(int device, float *min, float *max);
	virtual int GetSweep_64f(int device, double *min, double *max);
	virtual int GetPartialSweep_32f(int device, float *min, float *max, int *start, int *stop);
	virtual int GetPartialSweep_64f(int device, double *min, double *max, int *start, int *stop);
	virtual int GetRealTimeFrame(int device, float *sweep, float *frame);
	virtual int GetIQ_32f(int device, float *iq);
	virtual int GetIQ_64f(int device, double *iq);
	virtual int GetAudio(int device, float *audio);
	virtual int QueryTemperature(int device, float *temp);
	virtual int QueryDiagnostics(int device, float *voltage);

	virtual int AttachTg(int device);
	virtual int IsTgAttached(int device, bool *attached);
	virtual int ConfigTgSweep(int device, int sweepSize, bool highDynamicRange, bool passiveDevice);
	virtual int StoreTgThru(int device, int flag);
	virtual int SetTg(int device, double frequency, double amplitude);

	virtual int ConfigIFOutput(int device, double inputFreq, double outputFreq,
		int inputAtten, int outputGain);
	virtual int SelfTest(int device, int *results);

	virtual const char* GetAPIVersion();
	virtual const char* GetErrorString(int code);

	virtual const char* GetSeries();
};

#endif // DEVICE_SA_FUNCTIONS_H