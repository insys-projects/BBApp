#ifndef DEVICE_UNI_FUNCTIONS_H
#define DEVICE_UNI_FUNCTIONS_H

class DeviceUniFunctions
{
public:
	DeviceUniFunctions(){}
	virtual ~DeviceUniFunctions(){}

	virtual int GetSerialNumberList(int serialNumbers[8], int *deviceCount) = 0;
	virtual int OpenDeviceBySerialNumber(int *device, int serialNumber) = 0;
	virtual int OpenDevice(int *device) = 0;
	virtual int CloseDevice(int device) = 0;
	virtual int Preset(int device) = 0;

	virtual int GetSerialNumber(int device, int *serial) = 0;
	virtual int GetFirmwareString(int device, char firmwareString[16]) = 0;
	virtual int GetDeviceType(int device, int *device_type) = 0;
	virtual int ConfigAcquisition(int device, int detector, int scale) = 0;
	virtual int ConfigCenterSpan(int device, double center, double span) = 0;
	virtual int ConfigLevel(int device, double ref) = 0;
	virtual int ConfigGainAtten(int device, int atten, int gain, bool preAmp) = 0;
	virtual int ConfigSweepCoupling(int device, double rbw, double vbw, bool reject) = 0;
	virtual int ConfigProcUnits(int device, int units) = 0;
	virtual int ConfigIQ(int device, int decimation, double bandwidth) = 0;
	virtual int ConfigAudio(int device, int audioType, double centerFreq,
		double bandwidth, double audioLowPassFreq,
		double audioHighPassFreq, double fmDeemphasis) = 0;
	virtual int ConfigRealTime(int device, double yScale, int frameRate) = 0;

	virtual int Initiate(int device, int mode, int flag) = 0;
	virtual int Abort(int device) = 0;

	virtual int QuerySweepInfo(int device, int *sweepLength, double *startFreq, double *binSize) = 0;
	virtual int QueryStreamInfo(int device, int *returnLen, double *bandwidth, double *samplesPerSecond) = 0;
	virtual int QueryRealTimeFrameInfo(int device, int *imageWidth, int *imageHeight) = 0;
	virtual int GetSweep_32f(int device, float *min, float *max) = 0;
	virtual int GetSweep_64f(int device, double *min, double *max) = 0;
	virtual int GetPartialSweep_32f(int device, float *min, float *max, int *start, int *stop) = 0;
	virtual int GetPartialSweep_64f(int device, double *min, double *max, int *start, int *stop) = 0;
	virtual int GetRealTimeFrame(int device, float *sweep, float *frame) = 0;
	virtual int GetIQ_32f(int device, float *iq) = 0;
	virtual int GetIQ_64f(int device, double *iq) = 0;
	virtual int GetAudio(int device, float *audio) = 0;
	virtual int QueryTemperature(int device, float *temp) = 0;
	virtual int QueryDiagnostics(int device, float *voltage) = 0;

	virtual int AttachTg(int device) = 0;
	virtual int IsTgAttached(int device, bool *attached) = 0;
	virtual int ConfigTgSweep(int device, int sweepSize, bool highDynamicRange, bool passiveDevice) = 0;
	virtual int StoreTgThru(int device, int flag) = 0;
	virtual int SetTg(int device, double frequency, double amplitude) = 0;

	virtual int ConfigIFOutput(int device, double inputFreq, double outputFreq,
		int inputAtten, int outputGain) = 0;
	virtual int SelfTest(int device, int *results) = 0;

	virtual const char* GetAPIVersion() = 0;
	virtual const char* GetErrorString(int code) = 0;

	virtual const char* GetSeries() = 0;
};

#endif // DEVICE_UNI_FUNCTIONS_H