#include "device_sa_functions.h"

#include "lib/sa_api.h"

DeviceSAFunctions::DeviceSAFunctions()
{
}

DeviceSAFunctions::~DeviceSAFunctions()
{

}

int DeviceSAFunctions::GetSerialNumberList(int serialNumbers[8], int *deviceCount)
{
	return saGetSerialNumberList(serialNumbers, deviceCount);
}

int DeviceSAFunctions::GetNameList(char **ppNames, int *deviceCount)
{
	return -1;
}

int DeviceSAFunctions::OpenDeviceBySerialNumber(int *device, int serialNumber)
{
	return saOpenDeviceBySerialNumber(device, serialNumber);
}

int DeviceSAFunctions::OpenDeviceByName(int *device, char *pName)
{
	return -1;
}

int DeviceSAFunctions::OpenDevice(int *device)
{
	return saOpenDevice(device);
}

int DeviceSAFunctions::CloseDevice(int device)
{
	return saCloseDevice(device);
}

int DeviceSAFunctions::Preset(int device)
{
	return saPreset(device);
}

int DeviceSAFunctions::GetSerialNumber(int device, int *serial)
{
	return saGetSerialNumber(device, serial);
}

int DeviceSAFunctions::GetFirmwareString(int device, char firmwareString[16])
{
	return saGetFirmwareString(device, firmwareString);
}

int DeviceSAFunctions::GetDeviceType(int device, int *device_type)
{
	return saGetDeviceType(device, (saDeviceType*)device_type);
}

int DeviceSAFunctions::ConfigAcquisition(int device, int detector, int scale)
{
	return saConfigAcquisition(device, detector, scale);
}

int DeviceSAFunctions::ConfigCenterSpan(int device, double center, double span)
{
	return saConfigCenterSpan(device, center, span);
}

int DeviceSAFunctions::ConfigLevel(int device, double ref)
{
	return saConfigLevel(device, ref);
}

int DeviceSAFunctions::ConfigGainAtten(int device, int atten, int gain, bool preAmp)
{
	return saConfigGainAtten(device, atten, gain, preAmp);
}

int DeviceSAFunctions::ConfigSweepCoupling(int device, double rbw, double vbw, bool reject)
{
	return saConfigSweepCoupling(device, rbw, vbw, reject);
}

int DeviceSAFunctions::ConfigProcUnits(int device, int units)
{
	return saConfigProcUnits(device, units);
}

int DeviceSAFunctions::ConfigIQ(int device, int decimation, double bandwidth)
{
	return saConfigIQ(device, decimation, bandwidth);
}

int DeviceSAFunctions::ConfigAudio(int device, int audioType, double centerFreq,
	double bandwidth, double audioLowPassFreq,
	double audioHighPassFreq, double fmDeemphasis)
{
	return saConfigAudio(device, audioType, centerFreq,
		bandwidth, audioLowPassFreq,
		audioHighPassFreq, fmDeemphasis);
}
int DeviceSAFunctions::ConfigRealTime(int device, double yScale, int frameRate)
{
	return saConfigRealTime(device, yScale, frameRate);
}

int DeviceSAFunctions::Initiate(int device, int mode, int flag)
{
	return saInitiate(device, mode, flag);
}

int DeviceSAFunctions::Abort(int device)
{
	return saAbort(device);
}

int DeviceSAFunctions::QuerySweepInfo(int device, int *sweepLength, double *startFreq, double *binSize)
{
	return saQuerySweepInfo(device, sweepLength, startFreq, binSize);
}

int DeviceSAFunctions::QueryStreamInfo(int device, int *returnLen, double *bandwidth, double *samplesPerSecond)
{
	return saQueryStreamInfo(device, returnLen, bandwidth, samplesPerSecond);
}

int DeviceSAFunctions::QueryRealTimeFrameInfo(int device, int *imageWidth, int *imageHeight)
{
	return saQueryRealTimeFrameInfo(device, imageWidth, imageHeight);
}

int DeviceSAFunctions::GetSweep_32f(int device, float *min, float *max)
{
	return saGetSweep_32f(device, min, max);
}

int DeviceSAFunctions::GetSweep_64f(int device, double *min, double *max)
{
	return saGetSweep_64f(device, min, max);
}

int DeviceSAFunctions::GetPartialSweep_32f(int device, float *min, float *max, int *start, int *stop)
{
	return saGetPartialSweep_32f(device, min, max, start, stop);
}

int DeviceSAFunctions::GetPartialSweep_64f(int device, double *min, double *max, int *start, int *stop)
{
	return saGetPartialSweep_64f(device, min, max, start, stop);
}

int DeviceSAFunctions::GetRealTimeFrame(int device, float *sweep, float *frame)
{
	return saGetRealTimeFrame(device, sweep, frame);
}

int DeviceSAFunctions::GetIQ_32f(int device, float *iq)
{
	return saGetIQ_32f(device, iq);
}

int DeviceSAFunctions::GetIQ_64f(int device, double *iq)
{
	return saGetIQ_64f(device, iq);
}

int DeviceSAFunctions::GetAudio(int device, float *audio)
{
	return saGetAudio(device, audio);
}

int DeviceSAFunctions::QueryTemperature(int device, float *temp)
{
	return saQueryTemperature(device, temp);
}

int DeviceSAFunctions::QueryDiagnostics(int device, float *voltage)
{
	return saQueryDiagnostics(device, voltage);
}

int DeviceSAFunctions::AttachTg(int device)
{
	return saAttachTg(device);
}

int DeviceSAFunctions::IsTgAttached(int device, bool *attached)
{
	return saIsTgAttached(device, attached);
}

int DeviceSAFunctions::ConfigTgSweep(int device, int sweepSize, bool highDynamicRange, bool passiveDevice)
{
	return saConfigTgSweep(device, sweepSize, highDynamicRange, passiveDevice);
}

int DeviceSAFunctions::StoreTgThru(int device, int flag)
{
	return saStoreTgThru(device, flag);
}

int DeviceSAFunctions::SetTg(int device, double frequency, double amplitude)
{
	return saSetTg(device, frequency, amplitude);
}

int DeviceSAFunctions::ConfigIFOutput(int device, double inputFreq, double outputFreq,
	int inputAtten, int outputGain)
{
	return saConfigIFOutput(device, inputFreq, outputFreq, inputAtten, outputGain);
}

int DeviceSAFunctions::SelfTest(int device, int *results)
{
	return saSelfTest(device, (saSelfTestResults*)results);
}

const char* DeviceSAFunctions::GetAPIVersion()
{
	return saGetAPIVersion();
}

const char* DeviceSAFunctions::GetErrorString(int code)
{
	return saGetErrorString((saStatus)code);
}

const char* DeviceSAFunctions::GetSeries()
{
	return "SA44/124";
}