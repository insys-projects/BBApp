#include <vector>
#include <algorithm>

#include "IviSpecAn.h"

#include "ivispecan_functions.h"

IviSpecAnFunctions::IviSpecAnFunctions()
{
	m_pIviConfSrv = new IviConfSrv;
}

IviSpecAnFunctions::~IviSpecAnFunctions()
{

}

int IviSpecAnFunctions::GetSerialNumberList(int serialNumbers[8], int *deviceCount)
{
	return -1;
}

int IviSpecAnFunctions::GetNameList(char **ppNames, int *deviceCount)
{
	int i;
	ViInt32 nError = 0;
	ViInt32 nDriverSessionCount = 0;
	ViInt32 nLogicalNameCount = 0;
	TIVIConfigStore	rConfigStore;
	ViChar sLocation[8192];
	std::vector<char*> vDriverSession;

	*deviceCount = 0;

	// Получить стандартный путь
	nError = m_pIviConfSrv->GetConfigStore(&rConfigStore);

	if(nError != IVI_SUCCESS)
		return -1;

	if(strlen(rConfigStore.sActualLocation) == 0)
		// Берется основной стандартный путь
		strcpy(sLocation, rConfigStore.sMasterLocation);
	else
		// Берется текущий путь
		strcpy(sLocation, rConfigStore.sActualLocation);

	nError = m_pIviConfSrv->Deserialize(sLocation);

	if(nError != IVI_SUCCESS)
		return -1;

	nDriverSessionCount = m_pIviConfSrv->GetDriverSessionCount();

	if(nDriverSessionCount == -1)
		return -1;

	for(i = 0; i < nDriverSessionCount; i++)
	{
		TIVIDriverSession	rDriverSession;
		TIVISoftwareModule	rSoftwareModule;

		nError = m_pIviConfSrv->GetDriverSession(i + 1, &rDriverSession);

		if(nError != IVI_SUCCESS)
			continue;

		nError = m_pIviConfSrv->GetSoftwareModule(rDriverSession.sSoftwareModuleName, &rSoftwareModule);

		if(nError != IVI_SUCCESS)
			continue;

		if(!IsIviSpecAn(rSoftwareModule))
			continue;

		if(ppNames)
		{
			strcpy(ppNames[*deviceCount], rDriverSession.sName);
			vDriverSession.push_back(rDriverSession.sName);
		}
		
		(*deviceCount)++;
	}

	nLogicalNameCount = m_pIviConfSrv->GetLogicalNameCount();

	if(nLogicalNameCount == -1)
		return 0;

	for(i = 0; i < nLogicalNameCount; i++)
	{
		TIVILogicalName		rLogicalName;

		nError = m_pIviConfSrv->GetLogicalName(i + 1, &rLogicalName);

		if(nError != IVI_SUCCESS)
			continue;
		
		if(!std::binary_search(vDriverSession.begin(), vDriverSession.end(), rLogicalName.sSessionName))
			continue;

		if(ppNames)
			strcpy(ppNames[*deviceCount], rLogicalName.sName);

		(*deviceCount)++;
	}

	return 0;
}

int IviSpecAnFunctions::OpenDeviceBySerialNumber(int *device, int serialNumber)
{
	return -1;
}

int IviSpecAnFunctions::OpenDeviceByName(int *device, char *pName)
{
	ViSession session;
	ViInt32 nError = IviSpecAn_init((ViRsrc)pName, VI_TRUE, 0, &session);

	if(nError != IVI_SUCCESS)
		return -1;

	*device = session;

	return 0;
}

int IviSpecAnFunctions::OpenDevice(int *device)
{
	return -1;
}

int IviSpecAnFunctions::CloseDevice(int device)
{
	return -1;
}

int IviSpecAnFunctions::Preset(int device)
{
	return -1;
}

int IviSpecAnFunctions::GetSerialNumber(int device, int *serial)
{
	return -1;
}

int IviSpecAnFunctions::GetFirmwareString(int device, char firmwareString[16])
{
	return -1;
}

int IviSpecAnFunctions::GetDeviceType(int device, int *device_type)
{
	return -1;
}

int IviSpecAnFunctions::ConfigAcquisition(int device, int detector, int scale)
{
	ViInt32 nError = IviSpecAn_ConfigureAcquisition(device, VI_FALSE, 1, VI_TRUE,
						IVISPECAN_VAL_DETECTOR_TYPE_AVERAGE,
						IVISPECAN_VAL_VERTICAL_SCALE_LOGARITHMIC);

	if(nError != IVI_SUCCESS)
		return -1;

	return 0;
}

int IviSpecAnFunctions::ConfigCenterSpan(int device, double center, double span)
{
	ViInt32 nError = IviSpecAn_ConfigureFrequencyCenterSpan(device, center, span);
	
	if(nError != IVI_SUCCESS)
		return -1;

	return 0;
}

int IviSpecAnFunctions::ConfigLevel(int device, double ref)
{
	return -1;
}

int IviSpecAnFunctions::ConfigGainAtten(int device, int atten, int gain, bool preAmp)
{
	return -1;
}

int IviSpecAnFunctions::ConfigSweepCoupling(int device, double rbw, double vbw, bool reject)
{
	return -1;
}

int IviSpecAnFunctions::ConfigProcUnits(int device, int units)
{
	return -1;
}

int IviSpecAnFunctions::ConfigIQ(int device, int decimation, double bandwidth)
{
	return -1;
}

int IviSpecAnFunctions::ConfigAudio(int device, int audioType, double centerFreq,
	double bandwidth, double audioLowPassFreq,
	double audioHighPassFreq, double fmDeemphasis)
{
	return -1;
}
int IviSpecAnFunctions::ConfigRealTime(int device, double yScale, int frameRate)
{
	return -1;
}

int IviSpecAnFunctions::Initiate(int device, int mode, int flag)
{
	return -1;
}

int IviSpecAnFunctions::Abort(int device)
{
	ViInt32 nError = 0;

	nError = IviSpecAn_Abort(device);

	if(nError != IVI_SUCCESS)
		return -1;

	return 0;
}

int IviSpecAnFunctions::QuerySweepInfo(int device, int *sweepLength, double *startFreq, double *binSize)
{
	ViInt32 nError = 0;
	ViReal64 dStopFreq = 0;

	nError = IviSpecAn_QueryTraceSize(device, "1", (ViInt32*)sweepLength);

	if(nError != IVI_SUCCESS)
		return -1;

	nError = IviSpecAn_GetAttributeViReal64(device, 0, IVISPECAN_ATTR_FREQUENCY_START, startFreq);

	if(nError != IVI_SUCCESS)
		return -1;

	nError = IviSpecAn_GetAttributeViReal64(device, 0, IVISPECAN_ATTR_FREQUENCY_STOP, &dStopFreq);

	if(nError != IVI_SUCCESS)
		return -1;

	*binSize = (dStopFreq - *startFreq) / (double)(*sweepLength);
	
	return 0;
}

int IviSpecAnFunctions::QueryStreamInfo(int device, int *returnLen, double *bandwidth, double *samplesPerSecond)
{
	return -1;
}

int IviSpecAnFunctions::QueryRealTimeFrameInfo(int device, int *imageWidth, int *imageHeight)
{
	return -1;
}

int IviSpecAnFunctions::GetSweep_32f(int device, float *min, float *max)
{
	return -1;
}

int IviSpecAnFunctions::GetSweep_64f(int device, double *min, double *max)
{
	return -1;
}

int IviSpecAnFunctions::GetPartialSweep_32f(int device, float *min, float *max, int *start, int *stop)
{
	ViInt32 nError = 0;
	ViInt32 nSize = 0;
	ViInt32 nActualPoints = 0;
	ViReal64 *buf;
	int i;

	nError = IviSpecAn_QueryTraceSize(device, "1", &nSize);

	if(nError != IVI_SUCCESS)
		return -1;

	buf = (ViReal64*)malloc(nSize * sizeof(ViReal64));

	nError = IviSpecAn_ReadYTrace(device, "1", IVISPECAN_VAL_MAX_TIME_INFINITE, nSize, &nActualPoints, buf);

	if(nError != IVI_SUCCESS)
	{
		free(buf);
		return -1;
	}

	for(i = 0; i < nSize; i++)
	{
		min[i] = buf[i];
		max[i] = buf[i];
	}

	free(buf);

	*start = 0;
	*stop = nSize;

	return 0;
}

int IviSpecAnFunctions::GetPartialSweep_64f(int device, double *min, double *max, int *start, int *stop)
{
	return -1;
}

int IviSpecAnFunctions::GetRealTimeFrame(int device, float *sweep, float *frame)
{
	return -1;
}

int IviSpecAnFunctions::GetIQ_32f(int device, float *iq)
{
	return -1;
}

int IviSpecAnFunctions::GetIQ_64f(int device, double *iq)
{
	return -1;
}

int IviSpecAnFunctions::GetAudio(int device, float *audio)
{
	return -1;
}

int IviSpecAnFunctions::QueryTemperature(int device, float *temp)
{
	return -1;
}

int IviSpecAnFunctions::QueryDiagnostics(int device, float *voltage)
{
	return -1;
}

int IviSpecAnFunctions::AttachTg(int device)
{
	return -1;
}

int IviSpecAnFunctions::IsTgAttached(int device, bool *attached)
{
	return -1;
}

int IviSpecAnFunctions::ConfigTgSweep(int device, int sweepSize, bool highDynamicRange, bool passiveDevice)
{
	return -1;
}

int IviSpecAnFunctions::StoreTgThru(int device, int flag)
{
	return -1;
}

int IviSpecAnFunctions::SetTg(int device, double frequency, double amplitude)
{
	return -1;
}

int IviSpecAnFunctions::ConfigIFOutput(int device, double inputFreq, double outputFreq,
	int inputAtten, int outputGain)
{
	return -1;
}

int IviSpecAnFunctions::SelfTest(int device, int *results)
{
	return -1;
}

const char* IviSpecAnFunctions::GetAPIVersion()
{
	return 0;
}

const char* IviSpecAnFunctions::GetErrorString(int code)
{
	return 0;
}

const char* IviSpecAnFunctions::GetSeries()
{
	return "IviSpecAn";
}

int IviSpecAnFunctions::IsIviSpecAn(TIVISoftwareModule rSoftwareModule)
{
	int i;
	int isIviSpecAn = 0, isIviDriver = 0;

	for(i = 0; i < rSoftwareModule.vrPublishedAPI.size(); i++)
	{
		if(strcmp(rSoftwareModule.vrPublishedAPI[i].sType, "IVI-C") != 0)
			continue;

		if(strcmp(rSoftwareModule.vrPublishedAPI[i].sName, "IviDriver") == 0)
			isIviDriver = 1;
		else if(strcmp(rSoftwareModule.vrPublishedAPI[i].sName, "IviSpecAn") == 0)
			isIviSpecAn = 1;

		if(isIviSpecAn & isIviDriver)
			break;
	}

	return isIviDriver & isIviSpecAn;
}