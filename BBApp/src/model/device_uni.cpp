#include "device_uni.h"
#include "mainwindow.h"

#include <QElapsedTimer>
#include <QLibrary>
#include <QDir>

#define STATUS_CHECK(status) \
    if(status < saNoError) { \
    return false; \
    }

DeviceUni::DeviceUni(const Preferences *preferences, QString sPluginName) :
    Device(preferences)
{
	TGetDeviceFunctions fnGetDeviceFunctions;

    id = -1;
    open = false;
    serial_number = 0;
    deviceType = saDeviceTypeNone;
	device_type = DeviceTypeNone;
    tgIsConnected = false;

    timebase_reference = TIMEBASE_INTERNAL;
    externalReference = false;

	m_pDevUniFunc = 0;

	if(sPluginName.isEmpty())
		return;

	fnGetDeviceFunctions = (TGetDeviceFunctions)QLibrary::resolve(sPluginName, "GetDeviceFunctions");

	m_pDevUniFunc = fnGetDeviceFunctions();
}

DeviceUni::~DeviceUni()
{
    CloseDevice();
}

QList<DeviceConnectionInfo> DeviceUni::GetDeviceList()
{
	QList<DeviceConnectionInfo> deviceList;
	DeviceConnectionInfo info;

	QDir dir;
	QStringList lsNameFilters;
	QStringList	lsPlugins;
	QString		sPlugin;

	TGetDeviceFunctions fnGetDeviceFunctions;

	int serialNumbers[8];
	int deviceCount = 0;
	char **ppName;

	deviceList = Device::GetDeviceList();

	lsNameFilters << "*.shmod";

	lsPlugins = dir.entryList(lsNameFilters);

	foreach(sPlugin, lsPlugins)
	{
		fnGetDeviceFunctions = (TGetDeviceFunctions)QLibrary::resolve(sPlugin, "GetDeviceFunctions");

		if(fnGetDeviceFunctions == 0)
			continue;

		m_pDevUniFunc = fnGetDeviceFunctions();

		if(m_pDevUniFunc == 0)
			continue;

		info.series = m_pDevUniFunc->GetSeries();

		if(m_pDevUniFunc->GetSerialNumberList(serialNumbers, &deviceCount) == 0)
		{
			for(int i = 0; i < deviceCount; i++)
			{
				info.serialNumber = serialNumbers[i];
				info.sPluginName = sPlugin;
				info.sName.clear();
				deviceList.push_back(info);
			}
		}
		else if(m_pDevUniFunc->GetNameList(0, &deviceCount) == 0)
		{
			ppName = new char*[deviceCount];

			for(int i = 0; i < deviceCount; i++)
				ppName[i] = new char[256];

			if(m_pDevUniFunc->GetNameList(ppName, &deviceCount) != 0)
				return deviceList;

			for(int i = 0; i < deviceCount; i++)
			{
				info.serialNumber = 0;
				info.sName = QString::fromLocal8Bit(ppName[i]);
				info.sPluginName = sPlugin;
				deviceList.push_back(info);
			}
		}
	}
	
	return deviceList;
}

bool DeviceUni::OpenDevice()
{
    if(open) {
        return true;
    }

    lastStatus = saOpenDevice(&id);
    if(lastStatus != saNoError) {
        return false;
    }

    saGetSerialNumber(id, &serial_number);
    serial_string.sprintf("%d", serial_number);
    // Get Firmware version
    char fs[16];
    saGetFirmwareString(id, fs);
    firmware_string = fs;

    saGetDeviceType(id, &deviceType);

    if(deviceType == saDeviceTypeSA44) {
        device_type = DeviceTypeSA44A;
    } else if(deviceType == saDeviceTypeSA44B) {
        device_type = DeviceTypeSA44B;
    } else if(deviceType == saDeviceTypeSA124A || deviceType == saDeviceTypeSA124B) {
        device_type = DeviceTypeSA124;
    }

    saQueryTemperature(id, &current_temp);
    QString diagnostics;
    diagnostics.sprintf("%.2f C", CurrentTemp());
    MainWindow::GetStatusBar()->SetDiagnostics(diagnostics);

    open = true;
    return true;
}

bool DeviceUni::OpenDeviceWithSerial(int serialToOpen)
{
    if(open) {
        return true;
    }

    lastStatus = (saStatus)m_pDevUniFunc->OpenDeviceBySerialNumber(&id, serialToOpen);
    if(lastStatus != saNoError) {
        return false;
    }

	m_pDevUniFunc->GetSerialNumber(id, &serial_number);
    serial_string.sprintf("%d", serial_number);
    // Get Firmware version
    char fs[16];
    m_pDevUniFunc->GetFirmwareString(id, fs);
    firmware_string = fs;
    firmware_string += "  ";

    m_pDevUniFunc->GetDeviceType(id, (int*)&deviceType);

    if(deviceType == saDeviceTypeSA44) {
        device_type = DeviceTypeSA44A;
    } else if(deviceType == saDeviceTypeSA44B) {
        device_type = DeviceTypeSA44B;
    } else if(deviceType == saDeviceTypeSA124A || deviceType == saDeviceTypeSA124B) {
        device_type = DeviceTypeSA124;
    }

    m_pDevUniFunc->QueryTemperature(id, &current_temp);
    QString diagnostics;
    diagnostics.sprintf("%.2f C", CurrentTemp());
    MainWindow::GetStatusBar()->SetDiagnostics(diagnostics);

    open = true;
    return true;
}

bool DeviceUni::OpenDeviceWithName(const QString &sName)
{
	QByteArray arr;

	if(open) {
		return true;
	}

	arr = sName.toLocal8Bit();

	lastStatus = (saStatus)m_pDevUniFunc->OpenDeviceByName(&id, (char*)arr.data());
	if(lastStatus != saNoError) {
		return false;
	}

	open = true;
	return true;
}

int DeviceUni::GetNativeDeviceType() const
{
    if(!open) {
        return (int)saDeviceTypeSA44B;
    }

    saDeviceType type;
    m_pDevUniFunc->GetDeviceType(id, (int*)&type);
    return (int)type;
}

bool DeviceUni::AttachTg()
{
    SHProgressDialog pd("Looking for Tracking Generator");
    pd.show();

    QEventLoop el;
    std::thread t = std::thread(&DeviceUni::connectTgInThread, this, &el);
    el.exec();
    if(t.joinable()) {
        t.join();
    }

    return tgIsConnected;
}

bool DeviceUni::IsTgAttached()
{

    return false;
}

bool DeviceUni::CloseDevice()
{
	if(m_pDevUniFunc)
		m_pDevUniFunc->CloseDevice(id);

    id = -1;
    open = false;
    device_type = DeviceTypeSA44B;
    serial_number = 0;

    return true;
}

bool DeviceUni::Abort()
{
    m_pDevUniFunc->Abort(id);
    return true;
}

bool DeviceUni::Preset()
{
    if(!open) {
        //lastStatus = saDeviceNotOpenErr;
        return false;
    }

    m_pDevUniFunc->Abort(id);
    m_pDevUniFunc->Preset(id);

    return true;
}

bool DeviceUni::Reconfigure(const SweepSettings *s, Trace *t)
{   
    Abort();
    tgCalState = tgCalStateUncalibrated;

    // Update temperature between configurations
    QString diagnostics;
    if(deviceType != saDeviceTypeSA44) {
        m_pDevUniFunc->QueryTemperature(id, &current_temp);
        m_pDevUniFunc->QueryDiagnostics(id, &voltage);
        diagnostics.sprintf("%.2f C  --  %.2f V", CurrentTemp(), Voltage());
    }
    MainWindow::GetStatusBar()->SetDiagnostics(diagnostics);

    int atten = (s->Atten() == 0) ? SA_AUTO_ATTEN : s->Atten() - 1;
    int gain = (s->Gain() == 0) ? SA_AUTO_GAIN : s->Gain() - 1;
    bool preamp = (s->Preamp() == 2);
    int scale = (s->RefLevel().IsLogScale() ? SA_LOG_SCALE : SA_LIN_SCALE);

    m_pDevUniFunc->ConfigCenterSpan(id, s->Center(), s->Span());
    m_pDevUniFunc->ConfigAcquisition(id, s->Detector(), scale);

    if(atten == SA_AUTO_ATTEN || gain == SA_AUTO_GAIN) {
        m_pDevUniFunc->ConfigLevel(id, s->RefLevel().ConvertToUnits(AmpUnits::DBM));
        m_pDevUniFunc->ConfigGainAtten(id, SA_AUTO_ATTEN, SA_AUTO_GAIN, true);
    } else {
        m_pDevUniFunc->ConfigGainAtten(id, atten, gain, preamp);
    }

    m_pDevUniFunc->ConfigSweepCoupling(id, s->RBW(), s->VBW(), s->Rejection());
    m_pDevUniFunc->ConfigProcUnits(id, s->ProcessingUnits());

    int init_mode = SA_SWEEPING;
    if(s->Mode() == BB_REAL_TIME) {
        m_pDevUniFunc->ConfigRealTime(id, s->Div() * 10.0, prefs->realTimeFrameRate);
        init_mode = SA_REAL_TIME;
    }
    if(s->Mode() == MODE_NETWORK_ANALYZER) {
        m_pDevUniFunc->ConfigTgSweep(id, s->tgSweepSize, s->tgHighRangeSweep, s->tgPassiveDevice);
        init_mode = SA_TG_SWEEP;
    }

    saStatus initStatus = (saStatus)m_pDevUniFunc->Initiate(id, init_mode, 0);

    int traceLength = 0;
    double startFreq = 0.0, binSize = 0.0;
    m_pDevUniFunc->QuerySweepInfo(id, &traceLength, &startFreq, &binSize);

    t->SetSettings(*s);
    t->SetSize(traceLength);
    t->SetFreq(binSize, startFreq);
    t->SetUpdateRange(0, traceLength);

    if(s->Mode() == MODE_REAL_TIME) {
        int w = 0, h = 0;
        m_pDevUniFunc->QueryRealTimeFrameInfo(id, &w, &h);
        rtFrameSize.setWidth(w);
        rtFrameSize.setHeight(h);
    }

    return true;
}

bool DeviceUni::GetSweep(const SweepSettings *s, Trace *t)
{
    saStatus status = saNoError;

    int startIx, stopIx;

    status = (saStatus)m_pDevUniFunc->GetPartialSweep_32f(id, t->Min(), t->Max(), &startIx, &stopIx);

    // Testing Full sweep, not for production
    //status = saGetSweep_32f(id, t->Min(), t->Max());
    //startIx = 0;
    //stopIx = t->Length();

    if((status == saUSBCommErr) || (status == -1)) {
        emit connectionIssues();
        return false;
    }

    t->SetUpdateRange(startIx, stopIx);

    if(s->Mode() == MODE_NETWORK_ANALYZER && tgCalState == tgCalStatePending) {
        if(stopIx >= t->Length()) {
            tgCalState = tgCalStateCalibrated;
        }
    }

    adc_overflow = (status == saCompressionWarning);

    return true;
}

bool DeviceUni::GetRealTimeFrame(Trace &t, RealTimeFrame &frame)
{
    Q_ASSERT(frame.alphaFrame.size() == rtFrameSize.width() * rtFrameSize.height());
    Q_ASSERT(frame.rgbFrame.size() == frame.alphaFrame.size() * 4);

    // TODO check return value, emit error if not good
    saStatus status = (saStatus)m_pDevUniFunc->GetRealTimeFrame(id, t.Max(), &frame.alphaFrame[0]);

    if(status == saUSBCommErr) {
        emit connectionIssues();
        return false;
    }

    // Real-time only returns a max or avg trace
    // Copy max into min for real-time
    for(int i = 0; i < t.Length(); i++) {
        t.Min()[i] = t.Max()[i];
    }

    // Convert the alpha/intensity frame to a 4 channel image
    int totalPixels = frame.dim.height() * frame.dim.width();
    for(int i = 0; i < totalPixels; i++) {
        int toSet = frame.alphaFrame[i] * 255;
        if(toSet > 255) toSet = 255;
        frame.rgbFrame[i*4] = frame.rgbFrame[i*4+1] = frame.rgbFrame[i*4+2] = toSet;
        if(frame.alphaFrame[i] > 0.0) {
            frame.rgbFrame[i*4+3] = 255;
        } else {
            frame.rgbFrame[i*4+3] = 0;
        }
    }

    adc_overflow = (status == saCompressionWarning);

    return true;
}

// I/Q streaming setup
bool DeviceUni::Reconfigure(const DemodSettings *s, IQDescriptor *iqc)
{
    m_pDevUniFunc->Abort(id);

    int atten = (s->Atten() == 0) ? SA_AUTO_ATTEN : s->Atten() - 1;
    int gain = (s->Gain() == 0) ? SA_AUTO_GAIN : s->Gain() - 1;
    m_pDevUniFunc->ConfigCenterSpan(id, s->CenterFreq(), 250.0e3);

    if(s->Atten() == 0 || s->Gain() == 0 || s->Preamp() == 0) {
        m_pDevUniFunc->ConfigLevel(id, s->InputPower());
    } else {
        m_pDevUniFunc->ConfigGainAtten(id, s->Atten(), s->Gain(), s->Preamp());
    }

    m_pDevUniFunc->ConfigIQ(id, 0x1 << s->DecimationFactor(), s->Bandwidth());
    m_pDevUniFunc->Initiate(id, SA_IQ, 0);

    m_pDevUniFunc->QueryStreamInfo(id, &iqc->returnLen, &iqc->bandwidth, &iqc->sampleRate);
    iqc->timeDelta = 1.0 / iqc->sampleRate;
    iqc->decimation = 1;

    return true;
}

bool DeviceUni::GetIQ(IQCapture *iqc)
{
    saStatus status = (saStatus)m_pDevUniFunc->GetIQ_32f(id, (float*)(&iqc->capture[0]));

    if(status == saUSBCommErr) {
        emit connectionIssues();
        return false;
    }

    adc_overflow = (status == saCompressionWarning);

    return true;
}

bool DeviceUni::GetIQFlush(IQCapture *iqc, bool sync)
{
    int rs;
    if(!sync) {
        return GetIQ(iqc);
    } else {
        QElapsedTimer timer;
        do {
            timer.start();
            if(!GetIQ(iqc)) {
                return false;
            }
            rs = timer.restart();
            //printf("%d\n", rs);
        } while(rs < 2);
    }
    return true;
}

bool DeviceUni::ConfigureForTRFL(double center,
                                MeasRcvrRange range,
                                int atten,
                                int gain,
                                IQDescriptor &desc)
{
    m_pDevUniFunc->Abort(id);

    double refLevel;
    switch(range) {
    case MeasRcvrRangeHigh:
        refLevel = 0.0;
        break;
    case MeasRcvrRangeMid:
        refLevel = -25.0;
        break;
    case MeasRcvrRangeLow:
        refLevel = -50.0;
        break;
    }

    m_pDevUniFunc->ConfigCenterSpan(id, center, 100.0e3);
    m_pDevUniFunc->ConfigLevel(id, refLevel);
    m_pDevUniFunc->ConfigGainAtten(id, SA_AUTO_ATTEN, SA_AUTO_GAIN, false);
    m_pDevUniFunc->ConfigIQ(id, 2, 100.0e3);
    m_pDevUniFunc->Initiate(id, SA_IQ, 0);
    m_pDevUniFunc->QueryStreamInfo(id, &desc.returnLen, &desc.bandwidth, &desc.sampleRate);
    desc.timeDelta = 1.0 / desc.sampleRate;
    desc.decimation = 2;

    return true;
}

bool DeviceUni::ConfigureAudio(const AudioSettings &as)
{
    /*lastStatus = */m_pDevUniFunc->ConfigAudio(
                id,
                as.AudioMode(),
                as.CenterFreq(),
                as.IFBandwidth(),
                as.LowPassFreq(),
                as.HighPassFreq(),
                as.FMDeemphasis());

    /*lastStatus = */m_pDevUniFunc->Initiate(id, SA_AUDIO, 0);

    return true;
}

bool DeviceUni::GetAudio(float *audio)
{
    saStatus status = (saStatus)m_pDevUniFunc->GetAudio(id, audio);

    if(status == saUSBCommErr) {
        emit connectionIssues();
        return false;
    }

    adc_overflow = (status == saCompressionWarning);

    return true;
}

const char* DeviceUni::GetLastStatusString() const
{
    return saGetErrorString(lastStatus);
}

QString DeviceUni::GetDeviceString() const
{
    if(deviceType == saDeviceTypeSA44) return "SA44";
    if(deviceType == saDeviceTypeSA44B) return "SA44B";
    if(deviceType == saDeviceTypeSA124A) return "SA124A";
    if(deviceType == saDeviceTypeSA124B) return "SA124B";

    return "No Device Open";
}

void DeviceUni::UpdateDiagnostics()
{

}

bool DeviceUni::IsPowered() const
{
    return true;
}

bool DeviceUni::NeedsTempCal() const
{
    return false;
}

bool DeviceUni::SetTg(Frequency freq, double amp)
{
    saStatus stat = (saStatus)m_pDevUniFunc->SetTg(id, freq, amp);
    if(stat != saNoError) {
        return false;
    }

    return true;
}

void DeviceUni::TgStoreThrough()
{
    m_pDevUniFunc->StoreTgThru(id, TG_THRU_0DB);
    tgCalState = tgCalStatePending;
}

void DeviceUni::TgStoreThroughPad()
{
    m_pDevUniFunc->StoreTgThru(id, TG_THRU_20DB);
}

int DeviceUni::SetTimebase(int newTimebase)
{
    if(!externalReference && newTimebase == TIMEBASE_INTERNAL) {
        return timebase_reference;
    }

    if(externalReference) {
        QMessageBox::information(0, "Information",
                                 "Unable to modify the external reference of the device "
                                 "once it has been enabled.");
        return timebase_reference;
    }

    /*saStatus status = saEnableExternalReference(id);
    if(status == saExternalReferenceNotFound) {
        QMessageBox::warning(0, "Warning",
                             "No external reference on port.\n"
                             "Connect reference and try again.");
        return timebase_reference;
    }

    if(status == saInvalidDeviceErr) {
        QMessageBox::warning(0, "Warning",
                             "The current device does not support\n"
                             "an external reference.");
        return timebase_reference;
    }*/

    timebase_reference = newTimebase;
    externalReference = true;
    return timebase_reference;
}
void DeviceUni::ConfigureIFOutput(double inputFreq,
                                 double outputFreq,
                                 int inputAtten,
                                 int outputGain)
{
    m_pDevUniFunc->ConfigIFOutput(id, inputFreq, outputFreq, inputAtten, outputGain);
}

QString DeviceUni::GetSeries()
{
	return m_pDevUniFunc->GetSeries();
}