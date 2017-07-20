#include <string.h>
#include <map>
#include <string>

#include "IviConfSrv.h"

IviConfSrv::IviConfSrv()
{
	m_hIviConfigStore           = 0;
	m_hDriverSessionCollection  = 0;
	m_hLogicalNameCollection    = 0;
	m_hSoftwareModuleCollection = 0;
	m_hPublishedAPICollection   = 0;

	// Очистка кода и информации об ошибке
	ClearError();

	m_nError = IviConfig_Initialize(&m_hIviConfigStore);
}

IviConfSrv::~IviConfSrv()
{

}

// Чтение конфигурационного файла
ViInt32 IviConfSrv::Deserialize(ViChar *pLocation)
{
	// Очистка кода и информации об ошибке
	ClearError();

	if((m_hIviConfigStore == 0) || (pLocation == 0))
		return -1;

	m_nError = IviConfig_Deserialize(m_hIviConfigStore, pLocation);

	if (m_nError == IVICONFIG_ERROR_ALREADY_DESERIALIZED)
	{
		m_nError = IviConfig_Close(m_hIviConfigStore);

		if (m_nError != IVI_SUCCESS)
			return m_nError;

		m_nError = IviConfig_Initialize(&m_hIviConfigStore);

		if (m_nError != IVI_SUCCESS)
			return m_nError;

		m_nError = IviConfig_Deserialize(m_hIviConfigStore, pLocation);
	}
	else if (m_nError != IVI_SUCCESS)
		return m_nError;
	
	m_nError = IVICFG_GETCOLLECTION(LogicalName, m_hIviConfigStore, &m_hLogicalNameCollection);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETCOLLECTION(DriverSession, m_hIviConfigStore, &m_hDriverSessionCollection);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETCOLLECTION(SoftwareModule, m_hIviConfigStore, &m_hSoftwareModuleCollection);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETCOLLECTION(PublishedAPI, m_hIviConfigStore, &m_hPublishedAPICollection);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETCOLLECTION(HardwareAsset, m_hIviConfigStore, &m_hHwAssetCollection);
	
	return m_nError;
}

// Сохранение конфигурационного файла
ViInt32 IviConfSrv::Serialize(ViChar *pLocation)
{
	// Очистка кода и информации об ошибке
	ClearError();

	if ((m_hIviConfigStore == 0) || (pLocation == 0))
		return -1;

	m_nError = IviConfig_Serialize(m_hIviConfigStore, pLocation);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	return 0;
}

// Получить код ошибки
ViInt32 IviConfSrv::GetError()
{
	return m_nError;
}

// Получение информации об ошибке
ViInt32 IviConfSrv::GetErrorInfo(ViChar *pInfo, ViInt32 nSize)
{
	IviConfig_GetError(nSize, pInfo);
	return 0;
}

ViInt32 IviConfSrv::GetConfigStore(TIVIConfigStore *pConfigStore)
{
	ClearError();

	if ((m_hIviConfigStore == 0) || (pConfigStore == 0))
		return -1;
	
	m_nError = IVICFG_GETPROPERTY_STR(ConfigStore, m_hIviConfigStore, IVICONFIG_VAL_CONFIG_STORE_MASTER_LOCATION, pConfigStore->sMasterLocation);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(ConfigStore, m_hIviConfigStore, IVICONFIG_VAL_CONFIG_STORE_ACTUAL_LOCATION, pConfigStore->sActualLocation);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(ConfigStore, m_hIviConfigStore, IVICONFIG_VAL_CONFIG_STORE_REVISION, pConfigStore->sRevision);

	return m_nError;
}

ViInt32 IviConfSrv::GetLogicalNameCount()
{
	ViInt32 nCount;

	ClearError();
	
	if(m_hIviConfigStore == 0)
		return -1;

	m_nError = IVICFG_GETCOUNT(LogicalName, m_hLogicalNameCollection, &nCount);

	if (m_nError != IVI_SUCCESS)
		return -1;

	return nCount;
}

ViInt32 IviConfSrv::GetLogicalName(ViInt32 nIdx, TIVILogicalName *pLogicalName)
{
	IviLogicalNameHandle	hLogicalName;
	
	ClearError();

	if((m_hLogicalNameCollection == 0) || (pLogicalName == 0) || (nIdx < 1))
		return -1;

	m_nError = IVICFG_GETITEMBYIDX(LogicalName, m_hLogicalNameCollection, nIdx, &hLogicalName);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	return GetLogicalName(hLogicalName, pLogicalName);
}

ViInt32 IviConfSrv::GetLogicalName(ViChar *pName, TIVILogicalName *pLogicalName)
{
	IviLogicalNameHandle	hLogicalName;

	ClearError();

	if ((m_hLogicalNameCollection == 0) || (pLogicalName == 0) || (pName == 0))
		return -1;

	m_nError = IVICFG_GETITEMBYNAME(LogicalName, m_hLogicalNameCollection, pName, &hLogicalName);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	return GetLogicalName(hLogicalName, pLogicalName);
}

ViInt32 IviConfSrv::GetLogicalName(IviLogicalNameHandle hLogicalName, TIVILogicalName *pLogicalName)
{
	IviSessionHandle hSession;

	m_nError = IVICFG_GETPROPERTY_STR(LogicalName, hLogicalName, IVICONFIG_VAL_LOGICAL_NAME_NAME, pLogicalName->sName);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(LogicalName, hLogicalName, IVICONFIG_VAL_LOGICAL_NAME_DESCRIPTION, pLogicalName->sDescription);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IviConfig_GetLogicalNameSessionReference(hLogicalName, &hSession);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	pLogicalName->sSessionName[0] = '\0';

	if (hSession != 0)
		m_nError = IVICFG_GETPROPERTY_STR(Session, hSession, IVICONFIG_VAL_DATA_COMPONENT_NAME, pLogicalName->sSessionName);
	
	return m_nError;
}

ViInt32 IviConfSrv::SetLogicalName(TIVILogicalName rLogicalName)
{
	IviLogicalNameHandle	hLogicalName;
	IviSessionHandle		hSession = 0;

	ClearError();

	if (m_hIviConfigStore == 0)
		return -1;

	m_nError = IVICFG_GETITEMBYNAME(LogicalName, m_hLogicalNameCollection, rLogicalName.sName, &hLogicalName);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(LogicalName, hLogicalName, IVICONFIG_VAL_LOGICAL_NAME_NAME, rLogicalName.sName);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(LogicalName, hLogicalName, IVICONFIG_VAL_LOGICAL_NAME_DESCRIPTION, rLogicalName.sDescription);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	if(strlen(rLogicalName.sSessionName) != 0)
	{
		m_nError = IviConfig_GetSession(m_hIviConfigStore, rLogicalName.sSessionName, &hSession);

		if (m_nError != IVI_SUCCESS)
			return m_nError;
	}

	m_nError = IviConfig_SetLogicalNameSessionReference(hLogicalName, hSession);

	return m_nError;
}

ViInt32 IviConfSrv::CreateLogicalName(ViChar *pName)
{
	IviLogicalNameHandle hLogicalName;

	ClearError();

	if ((m_hLogicalNameCollection == 0) || (m_hIviConfigStore == 0))
		return -1;

	m_nError = IVICFG_CREATE(LogicalName, m_hLogicalNameCollection, pName, &hLogicalName);

	return m_nError;
}

ViInt32 IviConfSrv::DestroyLogicalName(ViChar *pName)
{
	IviLogicalNameHandle hLogicalName;

	ClearError();

	if ((m_hLogicalNameCollection == 0) || (m_hIviConfigStore == 0))
		return -1;

	m_nError = IVICFG_GETITEMBYNAME(LogicalName, m_hLogicalNameCollection, pName, &hLogicalName);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_DESTROY(LogicalName, m_hLogicalNameCollection, hLogicalName);

	return m_nError;
}

ViInt32 IviConfSrv::GetDriverSessionCount()
{
	ViInt32 nCount;

	ClearError();

	if (m_hIviConfigStore == 0)
		return -1;

	m_nError = IVICFG_GETCOUNT(DriverSession, m_hDriverSessionCollection, &nCount);

	if (m_nError != IVI_SUCCESS)
		return -1;

	return nCount;
}

ViInt32 IviConfSrv::GetDriverSession(ViInt32 nIdx, TIVIDriverSession *pDriverSession)
{
	IviLogicalNameHandle	hDriverSession;

	ClearError();

	if ((m_hDriverSessionCollection == 0) || (pDriverSession == 0) || (nIdx < 1))
		return -1;

	m_nError = IVICFG_GETITEMBYIDX(DriverSession, m_hDriverSessionCollection, nIdx, &hDriverSession);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	return GetDriverSession(hDriverSession, pDriverSession);
}

ViInt32 IviConfSrv::GetDriverSession(ViChar *pName, TIVIDriverSession *pDriverSession)
{
	IviDriverSessionHandle	hDriverSession;

	ClearError();

	if ((m_hDriverSessionCollection == 0) || (pDriverSession == 0) || (pName == 0))
		return -1;

	m_nError = IVICFG_GETITEMBYNAME(DriverSession, m_hDriverSessionCollection, pName, &hDriverSession);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	return GetDriverSession(hDriverSession, pDriverSession);
}

ViInt32 IviConfSrv::GetDriverSession(IviLogicalNameHandle hDriverSession, TIVIDriverSession *pDriverSession)
{
	IviSoftwareModuleHandle hSoftwareModule = 0;
	IviVirtualNameCollectionHandle hVirtualNameCollection = 0;
	IviHardwareAssetHandle hHwAsset;

	TIVIHardwareAsset rHwAsset;

	ClearError();

	m_nError = IVICFG_GETPROPERTY_STR(DriverSession, hDriverSession, IVICONFIG_VAL_DATA_COMPONENT_NAME, pDriverSession->sName);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(DriverSession, hDriverSession, IVICONFIG_VAL_DATA_COMPONENT_DESCRIPTION, pDriverSession->sDescription);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY(DriverSession, ViBoolean, hDriverSession, IVICONFIG_VAL_DRIVER_SESSION_CACHE, &pDriverSession->isCache);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(DriverSession, hDriverSession, IVICONFIG_VAL_DRIVER_SESSION_DRIVER_SETUP, pDriverSession->sDriverSetup);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY(DriverSession, ViBoolean, hDriverSession, IVICONFIG_VAL_DRIVER_SESSION_INTERCHANGE_CHECK, &pDriverSession->isInterchangeCheck);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY(DriverSession, ViBoolean, hDriverSession, IVICONFIG_VAL_DRIVER_SESSION_QUERY_INSTR_STATUS, &pDriverSession->isQueryInstrStatus);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY(DriverSession, ViBoolean, hDriverSession, IVICONFIG_VAL_DRIVER_SESSION_RANGE_CHECK, &pDriverSession->isRangeCheck);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY(DriverSession, ViBoolean, hDriverSession, IVICONFIG_VAL_DRIVER_SESSION_RECORD_COERCIONS, &pDriverSession->isRecordCoercions);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY(DriverSession, ViBoolean, hDriverSession, IVICONFIG_VAL_DRIVER_SESSION_SIMULATE, &pDriverSession->isSimulate);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IviConfig_GetSessionSoftwareModuleReference(hDriverSession, &hSoftwareModule);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	if(hSoftwareModule)
		m_nError = IVICFG_GETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_DATA_COMPONENT_NAME, pDriverSession->sSoftwareModuleName);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IviConfig_GetSessionVirtualNameCollection(hDriverSession, &hVirtualNameCollection);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = GetVirtualNames(hVirtualNameCollection, pDriverSession->vrVirtualName);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = GetDataComponent(hDriverSession, "Configurable Initial Settings", &(pDriverSession->rInitialSettings));

	if(m_nError == IVICONFIG_ERROR_NOT_EXIST)
		m_nError = 0;

	m_nError = IviConfig_GetSessionHardwareAssetReference(hDriverSession, &hHwAsset);

	if((m_nError != IVI_SUCCESS) || (hHwAsset == 0))
		return m_nError;

	m_nError = GetHardwareAsset(hHwAsset, &rHwAsset);
	
	if(m_nError != IVI_SUCCESS)
		return m_nError;

	strcpy(pDriverSession->sHwAssetName, rHwAsset.sName);

	return m_nError;
}

ViInt32 IviConfSrv::SetDriverSession(TIVIDriverSession rDriverSession)
{
	IviDriverSessionHandle	hDriverSession;
	IviSoftwareModuleHandle hSoftwareModule = 0;
	IviVirtualNameCollectionHandle hVirtualNameCollection;
	IviHardwareAssetHandle hHwAsset = 0;

	ClearError();

	if((m_hIviConfigStore == 0) || (m_hDriverSessionCollection == 0))
		return -1;

	m_nError = IVICFG_GETITEMBYNAME(DriverSession, m_hDriverSessionCollection, rDriverSession.sName, &hDriverSession);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(DriverSession, hDriverSession, IVICONFIG_VAL_DATA_COMPONENT_NAME, rDriverSession.sName);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(DriverSession, hDriverSession, IVICONFIG_VAL_DATA_COMPONENT_DESCRIPTION, rDriverSession.sDescription);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY(DriverSession, ViBoolean, hDriverSession, IVICONFIG_VAL_DRIVER_SESSION_CACHE, rDriverSession.isCache);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(DriverSession, hDriverSession, IVICONFIG_VAL_DRIVER_SESSION_DRIVER_SETUP, rDriverSession.sDriverSetup);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY(DriverSession, ViBoolean, hDriverSession, IVICONFIG_VAL_DRIVER_SESSION_INTERCHANGE_CHECK, rDriverSession.isInterchangeCheck);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY(DriverSession, ViBoolean, hDriverSession, IVICONFIG_VAL_DRIVER_SESSION_QUERY_INSTR_STATUS, rDriverSession.isQueryInstrStatus);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY(DriverSession, ViBoolean, hDriverSession, IVICONFIG_VAL_DRIVER_SESSION_RANGE_CHECK, rDriverSession.isRangeCheck);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY(DriverSession, ViBoolean, hDriverSession, IVICONFIG_VAL_DRIVER_SESSION_RECORD_COERCIONS, rDriverSession.isRecordCoercions);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY(DriverSession, ViBoolean, hDriverSession, IVICONFIG_VAL_DRIVER_SESSION_SIMULATE, rDriverSession.isSimulate);

	if(strlen(rDriverSession.sSoftwareModuleName) != 0)
	{
		m_nError = IVICFG_GETITEMBYNAME(SoftwareModule, m_hSoftwareModuleCollection, rDriverSession.sSoftwareModuleName, &hSoftwareModule);

		if(m_nError != IVI_SUCCESS)
			return m_nError;
	}

	m_nError = IviConfig_SetSessionSoftwareModuleReference(hDriverSession, hSoftwareModule);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IviConfig_GetSessionVirtualNameCollection(hDriverSession, &hVirtualNameCollection);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = SetVirtualNames(hVirtualNameCollection, rDriverSession.vrVirtualName);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = SetDataComponentStruct(hDriverSession, "Configurable Initial Settings", rDriverSession.rInitialSettings);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	if(strlen(rDriverSession.sHwAssetName) != 0)
	{
		m_nError = IVICFG_GETITEMBYNAME(HardwareAsset, m_hHwAssetCollection, rDriverSession.sHwAssetName, &hHwAsset);

		if(m_nError != IVI_SUCCESS)
			return m_nError;
	}
	
	m_nError = IviConfig_SetSessionHardwareAssetReference(hDriverSession, hHwAsset);

	return m_nError;
}

ViInt32 IviConfSrv::CreateDriverSession(ViChar *pName)
{
	IviDriverSessionHandle hDriverSession;

	if ((m_hLogicalNameCollection == 0) || (m_hIviConfigStore == 0))
		return -1;

	m_nError = IVICFG_CREATE(DriverSession, m_hDriverSessionCollection, pName, &hDriverSession);

	return m_nError;
}

ViInt32 IviConfSrv::DestroyDriverSession(ViChar *pName)
{
	IviDriverSessionHandle hDriverSession;

	ClearError();

	if ((m_hDriverSessionCollection == 0) || (m_hIviConfigStore == 0))
		return -1;

	m_nError = IVICFG_GETITEMBYNAME(DriverSession, m_hDriverSessionCollection, pName, &hDriverSession);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_DESTROY(DriverSession, m_hDriverSessionCollection, hDriverSession);

	return m_nError;
}

ViInt32 IviConfSrv::GetSoftwareModuleCount()
{
	ViInt32 nCount;

	ClearError();

	if (m_hIviConfigStore == 0)
		return -1;

	m_nError = IVICFG_GETCOUNT(SoftwareModule, m_hSoftwareModuleCollection, &nCount);

	if (m_nError != IVI_SUCCESS)
		return -1;

	return nCount;
}

ViInt32 IviConfSrv::GetSoftwareModule(ViInt32 nIdx, TIVISoftwareModule *pSoftwareModule)
{
	IviLogicalNameHandle	hSoftwareModule;

	ClearError();

	if ((m_hSoftwareModuleCollection == 0) || (pSoftwareModule == 0) || (nIdx < 1))
		return -1;

	m_nError = IVICFG_GETITEMBYIDX(SoftwareModule, m_hSoftwareModuleCollection, nIdx, &hSoftwareModule);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	return GetSoftwareModule(hSoftwareModule, pSoftwareModule);
}

ViInt32 IviConfSrv::GetSoftwareModule(ViChar *pName, TIVISoftwareModule *pSoftwareModule)
{
	IviLogicalNameHandle	hSoftwareModule;

	ClearError();

	if ((m_hSoftwareModuleCollection == 0) || (pSoftwareModule == 0) || (pName == 0))
		return -1;

	m_nError = IVICFG_GETITEMBYNAME(SoftwareModule, m_hSoftwareModuleCollection, pName, &hSoftwareModule);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	return GetSoftwareModule(hSoftwareModule, pSoftwareModule);
}

ViInt32 IviConfSrv::GetSoftwareModule(IviSoftwareModuleHandle hSoftwareModule, TIVISoftwareModule *pSoftwareModule)
{
	IviPublishedAPIHandle hPublishedAPIHandle = 0;
	IviPhysicalNameHandle hPhysicalNameHandle = 0;

	ClearError();

	m_nError = IVICFG_GETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_DATA_COMPONENT_NAME, pSoftwareModule->sName);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_DATA_COMPONENT_DESCRIPTION, pSoftwareModule->sDescription);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_SW_MODULE_ASMBLY_QLFD_CLASS_NAME, pSoftwareModule->sAssemblyQualifiedClassName);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_SW_MODULE_PATH, pSoftwareModule->sModulePath);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_SW_MODULE_PATH_32, pSoftwareModule->sModulePath32);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_SW_MODULE_PATH_64, pSoftwareModule->sModulePath64);

	if (m_nError != IVI_SUCCESS)
		return m_nError;
	
	m_nError = IVICFG_GETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_SW_MODULE_PREFIX, pSoftwareModule->sPrefix);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_SW_MODULE_PROGID, pSoftwareModule->sProgID);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_SW_MODULE_SUPPORTED_INSTR_MODELS, pSoftwareModule->sSupportedInstrumentModels);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	// Published API
	m_nError = GetPublishedAPIs(hSoftwareModule, pSoftwareModule->vrPublishedAPI);

	if (m_nError != IVI_SUCCESS)
		return m_nError;
	
	// Physical Names
	m_nError = GetPhysicalNames(hSoftwareModule, pSoftwareModule->vrPhysicalName);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = GetDataComponent(hSoftwareModule, "Configurable Initial Settings", &(pSoftwareModule->rConfigSettings));

	if(m_nError == IVICONFIG_ERROR_NOT_EXIST)
		m_nError = 0;

	return m_nError;
}

ViInt32 IviConfSrv::SetSoftwareModule(TIVISoftwareModule rSoftwareModule)
{
	IviSoftwareModuleHandle	hSoftwareModule;

	ClearError();

	if ((m_hIviConfigStore == 0) || (m_hSoftwareModuleCollection == 0))
		return -1;

	m_nError = IVICFG_GETITEMBYNAME(SoftwareModule, m_hSoftwareModuleCollection, rSoftwareModule.sName, &hSoftwareModule);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_DATA_COMPONENT_NAME, rSoftwareModule.sName);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_DATA_COMPONENT_DESCRIPTION, rSoftwareModule.sDescription);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_SW_MODULE_ASMBLY_QLFD_CLASS_NAME, rSoftwareModule.sAssemblyQualifiedClassName);

	if (m_nError != IVI_SUCCESS)
		return m_nError;
	
#ifdef WIN32
	strcpy(rSoftwareModule.sModulePath, rSoftwareModule.sModulePath32);
#else
	strcpy(rSoftwareModule.sModulePath, rSoftwareModule.sModulePath64);
#endif

	m_nError = IVICFG_SETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_SW_MODULE_PATH, rSoftwareModule.sModulePath);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_SW_MODULE_PATH_32, rSoftwareModule.sModulePath32);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_SW_MODULE_PATH_64, rSoftwareModule.sModulePath64);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_SW_MODULE_PREFIX, rSoftwareModule.sPrefix);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_SW_MODULE_PROGID, rSoftwareModule.sProgID);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(SoftwareModule, hSoftwareModule, IVICONFIG_VAL_SW_MODULE_SUPPORTED_INSTR_MODELS, rSoftwareModule.sSupportedInstrumentModels);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	// Published APIs
	m_nError = SetPublishedAPIs(hSoftwareModule, rSoftwareModule.vrPublishedAPI);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = SetPhysicalNames(hSoftwareModule, rSoftwareModule.vrPhysicalName);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = SetDataComponentStruct(hSoftwareModule, "Configurable Initial Settings", rSoftwareModule.rConfigSettings);

	return m_nError;
}

ViInt32 IviConfSrv::CreateSoftwareModule(ViChar *pName)
{
	IviSoftwareModuleHandle hSoftwareModule;

	if ((m_hLogicalNameCollection == 0) || (m_hIviConfigStore == 0))
		return -1;

	m_nError = IVICFG_CREATE(SoftwareModule, m_hSoftwareModuleCollection, pName, &hSoftwareModule);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	TIVISoftwareModule  rSoftwareModule;
	TIVIPublishedAPI	rPublishedAPI;

	strcpy(rSoftwareModule.sName, pName);
	rSoftwareModule.vrPublishedAPI.push_back(rPublishedAPI);

	m_nError = SetSoftwareModule(rSoftwareModule);
	
	return m_nError;
}

ViInt32 IviConfSrv::DestroySoftwareModule(ViChar *pName)
{
	IviSoftwareModuleHandle hSoftwareModule;

	ClearError();

	if ((m_hSoftwareModuleCollection == 0) || (m_hIviConfigStore == 0))
		return -1;

	m_nError = IVICFG_GETITEMBYNAME(SoftwareModule, m_hSoftwareModuleCollection, pName, &hSoftwareModule);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_DESTROY(SoftwareModule, m_hSoftwareModuleCollection, hSoftwareModule);

	return m_nError;
}

ViInt32 IviConfSrv::GetHardwareAssets(vector<TIVIHardwareAsset> &vrHwAsset)
{
	IviHardwareAssetHandle hHwAsset = 0;
	ViInt32 i, nCnt = 0;
	TIVIHardwareAsset rHwAsset;

	m_nError = IVICFG_GETCOUNT(HardwareAsset, m_hHwAssetCollection, &nCnt);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	for(i = 0; i < nCnt; i++)
	{
		m_nError = IVICFG_GETITEMBYIDX(HardwareAsset, m_hHwAssetCollection, i + 1, &hHwAsset);

		if(m_nError != IVI_SUCCESS)
			return m_nError;

		m_nError = GetHardwareAsset(hHwAsset, &rHwAsset);

		if(m_nError != IVI_SUCCESS)
			return m_nError;

		vrHwAsset.push_back(rHwAsset);
	}

	return 0;
}

ViInt32 IviConfSrv::GetHardwareAsset(ViChar *pName, TIVIHardwareAsset *prHwAsset)
{
	IviHardwareAssetHandle	hHardwareAsset;

	m_nError = IVICFG_GETITEMBYNAME(HardwareAsset, m_hHwAssetCollection, pName, &hHardwareAsset);

	if (m_nError != IVI_SUCCESS)
		return m_nError;

	return GetHardwareAsset(hHardwareAsset, prHwAsset);
}

ViInt32 IviConfSrv::GetHardwareAsset(IviHardwareAssetHandle hHardwareAsset, TIVIHardwareAsset *prHwAsset)
{
	m_nError = IVICFG_GETPROPERTY_STR(HardwareAsset, hHardwareAsset, IVICONFIG_VAL_DATA_COMPONENT_NAME, prHwAsset->sName);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(HardwareAsset, hHardwareAsset, IVICONFIG_VAL_DATA_COMPONENT_DESCRIPTION, prHwAsset->sDescription);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(HardwareAsset, hHardwareAsset, IVICONFIG_VAL_HARDWARE_ASSET_IO_DESCRIPTOR, prHwAsset->sIORcDescriptor);

	return m_nError;
}

ViInt32 IviConfSrv::SetHardwareAssets(vector<TIVIHardwareAsset> vrHwAsset)
{
	ViInt32 i;

	for(i = 0; i < vrHwAsset.size(); i++)
	{
		m_nError = SetHardwareAsset(vrHwAsset[i]);

		if(m_nError != IVI_SUCCESS)
			return m_nError;
	}

	return m_nError;
}

ViInt32 IviConfSrv::SetHardwareAsset(TIVIHardwareAsset rHwAsset)
{

	IviHardwareAssetHandle hHardwareAsset = 0;

	m_nError = IVICFG_GETITEMBYNAME(HardwareAsset, m_hHwAssetCollection, rHwAsset.sName, &hHardwareAsset);

	if(m_nError == IVICONFIG_ERROR_NOT_EXIST)
	{	// Hardware Asset не существует, создаем его
		m_nError = IVICFG_CREATE(HardwareAsset, m_hHwAssetCollection, rHwAsset.sName, &hHardwareAsset);

		if(m_nError != IVI_SUCCESS)
			return m_nError;
	}

	m_nError = IVICFG_SETPROPERTY_STR(HardwareAsset, hHardwareAsset, IVICONFIG_VAL_DATA_COMPONENT_NAME, rHwAsset.sName);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(HardwareAsset, hHardwareAsset, IVICONFIG_VAL_DATA_COMPONENT_DESCRIPTION, rHwAsset.sDescription);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(HardwareAsset, hHardwareAsset, IVICONFIG_VAL_HARDWARE_ASSET_IO_DESCRIPTOR, rHwAsset.sIORcDescriptor);

	return m_nError;
}

ViInt32 IviConfSrv::DestroyHardwareAsset(ViChar *pName)
{
	IviHardwareAssetHandle hHardwareAsset;

	m_nError = IVICFG_GETITEMBYNAME(HardwareAsset, m_hHwAssetCollection, pName, &hHardwareAsset);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_DESTROY(HardwareAsset, m_hHwAssetCollection, hHardwareAsset);

	return m_nError;
}

// Published API
ViInt32 IviConfSrv::GetPublishedAPICount(IviPublishedAPICollectionHandle hPublishedAPICollection)
{
	ViInt32 nCount;

	ClearError();

	if(m_hIviConfigStore == 0)
		return -1;

	m_nError = IVICFG_GETCOUNT(PublishedAPI, hPublishedAPICollection, &nCount);

	if(m_nError != IVI_SUCCESS)
		return -1;

	return nCount;
}

ViInt32 IviConfSrv::GetPublishedAPI(IviPublishedAPICollectionHandle hPublishedAPICollection, ViInt32 nIdx, TIVIPublishedAPI *pPublishedAPI)
{
	IviPublishedAPIHandle hPublishedAPIHandle;

	ClearError();

	if(m_hIviConfigStore == 0)
		return -1;

	m_nError = IVICFG_GETITEMBYIDX(PublishedAPI, hPublishedAPICollection, nIdx, &hPublishedAPIHandle);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY(PublishedAPI, ViInt32, hPublishedAPIHandle, IVICONFIG_VAL_PUBLISHED_API_MAJOR_VERSION, &(pPublishedAPI->nMajorVersion));

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY(PublishedAPI, ViInt32, hPublishedAPIHandle, IVICONFIG_VAL_PUBLISHED_API_MINOR_VERSION, &(pPublishedAPI->nMinorVersion));

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(PublishedAPI, hPublishedAPIHandle, IVICONFIG_VAL_PUBLISHED_API_NAME, pPublishedAPI->sName);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(PublishedAPI, hPublishedAPIHandle, IVICONFIG_VAL_PUBLISHED_API_TYPE, pPublishedAPI->sType);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	return m_nError;
}

ViInt32	IviConfSrv::GetPublishedAPIs(IviSoftwareModuleHandle hSoftwareModule, vector<TIVIPublishedAPI> &vrPublishedAPI)
{
	IviPublishedAPICollectionHandle hPublishedAPICollection = 0;
	ViInt32 nPublishedAPICount = 0;
	TIVIPublishedAPI rPublishedAPI;
	int i;

	m_nError = IviConfig_GetSoftwareModulePublishedAPICollection(hSoftwareModule, &hPublishedAPICollection);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	nPublishedAPICount = GetPublishedAPICount(hPublishedAPICollection);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	for(i = 0; i < nPublishedAPICount; i++)
	{
		m_nError = GetPublishedAPI(hPublishedAPICollection, i + 1, &rPublishedAPI);

		if(m_nError != IVI_SUCCESS)
			return m_nError;

		vrPublishedAPI.push_back(rPublishedAPI);
	}

	return m_nError;
}

ViInt32	IviConfSrv::SetPublishedAPIs(IviSoftwareModuleHandle hSoftwareModule, vector<TIVIPublishedAPI> vrPublishedAPI)
{
	int i;
	IviPublishedAPICollectionHandle hCurPublishedAPICollection;

	m_nError = IviConfig_GetSoftwareModulePublishedAPICollection(hSoftwareModule, &hCurPublishedAPICollection);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	// Удаляем старые ссылки
	m_nError = RemoveAllPublishedAPIReference(hCurPublishedAPICollection);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	// Перебор новых ссылок
	for(i = 0; i < vrPublishedAPI.size(); i++)
	{
		IviPublishedAPIHandle	hPublishedAPI = 0;
		TIVIPublishedAPI		rPublishedAPI = vrPublishedAPI[i];

		// Поиск среди существующих API
		m_nError = FindPublishedAPI(rPublishedAPI, &hPublishedAPI);

		if(m_nError != IVI_SUCCESS)
			return m_nError;

		if(hPublishedAPI == 0)
			// API не найдено, создаем его
			m_nError = CreatePublishedAPI(rPublishedAPI, &hPublishedAPI);

		if(m_nError != IVI_SUCCESS)
			return m_nError;

		// Добавляем ссылку
		m_nError = AddPublishedAPIReference(hCurPublishedAPICollection, hPublishedAPI);

		if(m_nError != IVI_SUCCESS)
			return m_nError;
	}

	return m_nError;
}

ViInt32 IviConfSrv::CreatePublishedAPI(TIVIPublishedAPI rPublishedAPI, IviPublishedAPIHandle *hPublishedAPI)
{
	ClearError();

	if(m_hIviConfigStore == 0)
		return -1;

	m_nError = IviConfig_CreatePublishedAPI(m_hPublishedAPICollection, rPublishedAPI.sName, rPublishedAPI.sType, hPublishedAPI);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY(PublishedAPI, ViInt32, *hPublishedAPI, IVICONFIG_VAL_PUBLISHED_API_MAJOR_VERSION, rPublishedAPI.nMajorVersion);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY(PublishedAPI, ViInt32, *hPublishedAPI, IVICONFIG_VAL_PUBLISHED_API_MINOR_VERSION, rPublishedAPI.nMinorVersion);
		
	return m_nError;
}

ViInt32 IviConfSrv::FindPublishedAPI(TIVIPublishedAPI rPublishedAPI, IviPublishedAPIHandle *phPublishedAPI)
{
	ViInt32 nPublishedAPICount = 0;
	ViInt32 i;
	TIVIPublishedAPI rCurPublishedAPI;

	ClearError();

	if(m_hIviConfigStore == 0)
		return -1;

	nPublishedAPICount = GetPublishedAPICount(m_hPublishedAPICollection);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	for(i = 0; i < nPublishedAPICount; i++)
	{
		m_nError = GetPublishedAPI(m_hPublishedAPICollection, i + 1, &rCurPublishedAPI);

		if(m_nError != IVI_SUCCESS)
			return m_nError;

		// Сравнение
		if(ComparePublishedAPI(rPublishedAPI, rCurPublishedAPI))
		{
			m_nError = IVICFG_GETITEMBYIDX(PublishedAPI, m_hPublishedAPICollection, i + 1, phPublishedAPI);
			break;
		}
	}

	return m_nError;
}

ViInt32 IviConfSrv::AddPublishedAPIReference(IviPublishedAPICollectionHandle hPublishedAPICollection, IviPublishedAPIHandle hPublishedAPI)
{
	ClearError();

	m_nError = IviConfig_AddPublishedAPIReference(hPublishedAPICollection, hPublishedAPI);

	return m_nError;
}

ViInt32 IviConfSrv::RemoveAllPublishedAPIReference(IviPublishedAPICollectionHandle hPublishedAPICollection)
{
	IviPublishedAPIHandle hPublishedAPIHandle;
	ViInt32 nPublishedAPICount = 0;
	ViInt32 i;

	ClearError();

	if(m_hIviConfigStore == 0)
		return -1;

	nPublishedAPICount = GetPublishedAPICount(hPublishedAPICollection);

	for(i = 0; i < nPublishedAPICount; i++)
	{
		m_nError = IVICFG_GETITEMBYIDX(PublishedAPI, hPublishedAPICollection, 1, &hPublishedAPIHandle);

		if(m_nError != IVI_SUCCESS)
			return m_nError;

		m_nError = IviConfig_RemovePublishedAPIReference(hPublishedAPICollection, hPublishedAPIHandle);

		if(m_nError != IVI_SUCCESS)
			return m_nError;
	}

	return m_nError;
}

ViInt32 IviConfSrv::ComparePublishedAPI(TIVIPublishedAPI rPublishedAPI1, TIVIPublishedAPI rPublishedAPI2)
{
	if(strcmp(rPublishedAPI1.sName, rPublishedAPI2.sName) != 0)
		return 0;

	if(strcmp(rPublishedAPI1.sType, rPublishedAPI2.sType) != 0)
		return 0;

	if(rPublishedAPI1.nMajorVersion != rPublishedAPI2.nMajorVersion)
		return 0;

	if(rPublishedAPI1.nMinorVersion != rPublishedAPI2.nMinorVersion)
		return 0;

	return 1;
}

ViInt32 IviConfSrv::GetPhysicalNameCount(IviPhysicalNameCollectionHandle hPhysicalNameCollection)
{
	ViInt32 nCount;

	ClearError();

	if(m_hIviConfigStore == 0)
		return -1;

	m_nError = IVICFG_GETCOUNT(PhysicalName, hPhysicalNameCollection, &nCount);

	if(m_nError != IVI_SUCCESS)
		return -1;

	return nCount;
}

ViInt32 IviConfSrv::GetPhysicalName(IviPhysicalNameCollectionHandle hPhysicalNameCollection, ViInt32 nIdx, TIVIPhysicalName *pPhysicalName)
{
	IviPhysicalNameHandle hPhysicalNameHandle;
	ViChar	sName[IVI_MAXSIZE];
	ViChar  *p;

	ClearError();

	if(m_hIviConfigStore == 0)
		return -1;

	m_nError = IVICFG_GETITEMBYIDX(PhysicalName, hPhysicalNameCollection, nIdx, &hPhysicalNameHandle);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(PhysicalName, hPhysicalNameHandle, IVICONFIG_VAL_PHYSICAL_NAME_NAME, sName);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	p = strstr(sName, "!!");

	if(p != 0)
	{
		p[0] = '\0';
		p += 2;
		strcpy(pPhysicalName->sName, p);
		strcpy(pPhysicalName->sQualifier, sName);
	}
	else
	{
		strcpy(pPhysicalName->sName, sName);
		pPhysicalName->sQualifier[0] = '\0';
	}

	m_nError = IVICFG_GETPROPERTY_STR(PhysicalName, hPhysicalNameHandle, IVICONFIG_VAL_PHYSICAL_NAME_RCNAME, pPhysicalName->sRCName);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	return m_nError;
}

ViInt32	IviConfSrv::GetPhysicalNames(IviSoftwareModuleHandle hSoftwareModule, vector<TIVIPhysicalName> &vrPhysicalName)
{
	IviPhysicalNameCollectionHandle hPhysicalNameCollection = 0;
	ViInt32 nPhysicalNameCount = 0;
	TIVIPhysicalName rPhysicalName;
	int i;

	m_nError = IviConfig_GetSoftwareModulePhysicalNameCollection(hSoftwareModule, &hPhysicalNameCollection);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	nPhysicalNameCount = GetPhysicalNameCount(hPhysicalNameCollection);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	for(i = 0; i < nPhysicalNameCount; i++)
	{
		m_nError = GetPhysicalName(hPhysicalNameCollection, i + 1, &rPhysicalName);

		if(m_nError != IVI_SUCCESS)
			return m_nError;

		vrPhysicalName.push_back(rPhysicalName);
	}

	return m_nError;
}

ViInt32	IviConfSrv::SetPhysicalNames(IviSoftwareModuleHandle hSoftwareModule, vector<TIVIPhysicalName> vrPhysicalName)
{
	int i;
	IviPhysicalNameCollectionHandle hCurPhysicalNameCollection;

	m_nError = IviConfig_GetSoftwareModulePhysicalNameCollection(hSoftwareModule, &hCurPhysicalNameCollection);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	// Удаляем
	m_nError = RemoveAllPhysicalName(hCurPhysicalNameCollection);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	// Перебор
	for(i = 0; i < vrPhysicalName.size(); i++)
	{
		IviPhysicalNameHandle	hPhysicalName = 0;
		TIVIPhysicalName		rPhysicalName = vrPhysicalName[i];
		ViChar					sName[IVI_MAXSIZE];

		if(rPhysicalName.sQualifier[0] != '\0')
			sprintf(sName, "%s!!%s", rPhysicalName.sQualifier, rPhysicalName.sName);
		else
			strcpy(sName, rPhysicalName.sName);

		m_nError = IviConfig_CreatePhysicalName(hCurPhysicalNameCollection, sName, rPhysicalName.sRCName, &hPhysicalName);

		if(m_nError != IVI_SUCCESS)
			return m_nError;
	}

	return m_nError;
}

ViInt32 IviConfSrv::RemoveAllPhysicalName(IviPhysicalNameCollectionHandle hPhysicalNameCollection)
{
	IviPhysicalNameHandle hPhysicalName;
	ViInt32 nPhysicalNameCount = 0;
	ViInt32 i;

	ClearError();

	if(m_hIviConfigStore == 0)
		return -1;

	nPhysicalNameCount = GetPhysicalNameCount(hPhysicalNameCollection);

	for(i = 0; i < nPhysicalNameCount; i++)
	{
		m_nError = IVICFG_GETITEMBYIDX(PhysicalName, hPhysicalNameCollection, 1, &hPhysicalName);

		if(m_nError != IVI_SUCCESS)
			return m_nError;

		m_nError = IVICFG_DESTROY(PhysicalName, hPhysicalNameCollection, hPhysicalName);

		if(m_nError != IVI_SUCCESS)
			return m_nError;
	}

	return m_nError;
}

// Data Component
ViInt32	IviConfSrv::GetDataComponent(IviConfigHandle hIviConfig, ViChar *pName, TIVIDataComponent *pDataComponent)
{
	IviDataComponentCollectionHandle	hDataComponentCollection;
	IviDataComponentHandle				hDataComponent;

	m_nError = IviConfig_GetConfigComponentDataComponentCollection(hIviConfig, &hDataComponentCollection);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETITEMBYNAME(DataComponent, hDataComponentCollection, pName, &hDataComponent);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = GetDataComponent(hDataComponent, pDataComponent);

	return m_nError;
}

ViInt32 IviConfSrv::GetDataComponent(IviDataComponentCollectionHandle hDataComponentCollection, ViInt32 nIdx, TIVIDataComponent *pDataComponent)
{
	IviDataComponentHandle hDataComponent;

	m_nError = IVICFG_GETITEMBYIDX(DataComponent, hDataComponentCollection, nIdx, &hDataComponent);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = GetDataComponent(hDataComponent, pDataComponent);

	return m_nError;
}

ViInt32	IviConfSrv::GetDataComponent(IviDataComponentHandle hDataComponent, TIVIDataComponent *pDataComponent)
{
	ClearError();

	m_nError = IVICFG_GETPROPERTY_STR(DataComponent, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_DESCRIPTION, pDataComponent->sDescription);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY(DataComponent, ViInt32, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_HELP_CONTEXT_ID, &(pDataComponent->nHelpContextID));

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(DataComponent, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_HELP_FILE_PATH, pDataComponent->sHelpFilePath);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(DataComponent, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_NAME, pDataComponent->sName);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY(DataComponent, ViBoolean, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_READ_ONLY, &(pDataComponent->isReadOnly));

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(DataComponent, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_SW_MODULE_KEY, pDataComponent->sSoftwareModuleKey);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_GETPROPERTY_STR(DataComponent, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_USED_IN_SESSION, pDataComponent->sUsedInSession);

	if(strcmp(pDataComponent->sUsedInSession, "Optional") == 0)
		strcpy(pDataComponent->sUsedInSession, "False");
	else
		strcpy(pDataComponent->sUsedInSession, "True");

	if(m_nError != IVI_SUCCESS)
		return m_nError;
	
	m_nError = IVICFG_GETPROPERTY_STR(DataComponent, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_TYPE, pDataComponent->sType);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	if(strcmp(pDataComponent->sType, "Structure") == 0)
		m_nError = GetDataComponents(hDataComponent, pDataComponent->vrDataComponent);
	else if(strcmp(pDataComponent->sType, "String") == 0)
		m_nError = IVICFG_GETPROPERTY_STR(DataComponent, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_VALUE, pDataComponent->sValue);
	else if(strcmp(pDataComponent->sType, "Real") == 0)
	{
		ViReal64 fVal;
		m_nError = IVICFG_GETPROPERTY(DataComponent, ViReal64, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_VALUE, &fVal);
		sprintf(pDataComponent->sValue, "%f", fVal);
	}
	else if(strcmp(pDataComponent->sType, "Integer") == 0)
	{
		ViInt32 nVal;
		m_nError = IVICFG_GETPROPERTY(DataComponent, ViInt32, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_VALUE, &nVal);
		sprintf(pDataComponent->sValue, "%d", nVal);
	}
	else if(strcmp(pDataComponent->sType, "Boolean") == 0)
	{
		ViBoolean nVal;
		m_nError = IVICFG_GETPROPERTY(DataComponent, ViBoolean, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_VALUE, &nVal);

		if(nVal)
			strcpy(pDataComponent->sValue, "True");
		else
			strcpy(pDataComponent->sValue, "False");
	}

	return m_nError;
}

ViInt32	IviConfSrv::GetDataComponents(IviDataComponentHandle hStructComponent, vector<TIVIDataComponent> &vrDataComponent)
{
	IviDataComponentCollectionHandle	hDataComponentCollection;
	ViInt32								nDataComponentCount;
	ViInt32								i;
	TIVIDataComponent					rDataComponent;

	m_nError = IviConfig_GetStructureDataComponentCollection(hStructComponent, &hDataComponentCollection);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IviConfig_GetDataComponentCount(hDataComponentCollection, &nDataComponentCount);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	for(i = 0; i < nDataComponentCount; i++)
	{
		m_nError = GetDataComponent(hDataComponentCollection, i + 1, &rDataComponent);

		if(m_nError != IVI_SUCCESS)
			return m_nError;

		vrDataComponent.push_back(rDataComponent);
	}

	return m_nError;
}

ViInt32	IviConfSrv::SetDataComponentStruct(IviConfigHandle hIviConfig, ViChar *pName, TIVIDataComponent rDataComponent)
{
	map<string, int> mType;

	int nType;
	ViChar *pComponentName;
	ViInt32 i;

	IviDataComponentCollectionHandle	hDataComponentCollection;
	IviDataComponentCollectionHandle	hDataComponentStructCollection;
	IviDataComponentHandle				hDataComponent;
	IviDataComponentHandle				hDataComponentStruct;

	mType.insert(pair<string, int>("Boolean", IVICONFIG_VAL_TYPE_BOOLEAN));
	mType.insert(pair<string, int>("Integer", IVICONFIG_VAL_TYPE_INTEGER));
	mType.insert(pair<string, int>("Real",    IVICONFIG_VAL_TYPE_REAL));
	mType.insert(pair<string, int>("String",  IVICONFIG_VAL_TYPE_STRING));

	// Получаем коллекцию компонентов
	m_nError = IviConfig_GetConfigComponentDataComponentCollection(hIviConfig, &hDataComponentCollection);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	// Получаем компонент структуры по имени
	m_nError = IVICFG_GETITEMBYNAME(DataComponent, hDataComponentCollection, pName, &hDataComponentStruct);

	if(m_nError == IVI_SUCCESS)
		// Удаляем существующую структуру
		IVICFG_DESTROY(DataComponent, hDataComponentCollection, hDataComponentStruct);

	if(rDataComponent.vrDataComponent.size() == 0)
		// Структура пустая
		return 0;

	// Тип первого элемента структуры
	nType = mType[rDataComponent.vrDataComponent[0].sType];
	// Имя первого элемента структуры
	pComponentName = rDataComponent.vrDataComponent[0].sName;
	// Создаем структуру
	m_nError = IviConfig_CreateStructure(hDataComponentCollection, pName, nType, pComponentName, &hDataComponentStruct);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	// Получаем коллекцию структуры
	m_nError = IviConfig_GetStructureDataComponentCollection(hDataComponentStruct, &hDataComponentStructCollection);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	// Заполняем первый элемент
	m_nError = IVICFG_GETITEMBYNAME(DataComponent, hDataComponentStructCollection, pComponentName, &hDataComponent);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = SetDataComponent(hDataComponent, rDataComponent.vrDataComponent[0]);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	// Создаем элементы струтктуры
	for(i = 1; i < rDataComponent.vrDataComponent.size(); i++)
	{	
		// Тип элемента
		nType = mType[rDataComponent.vrDataComponent[i].sType];
		// Имя элемента
		pComponentName = rDataComponent.vrDataComponent[i].sName;
		// Создаем элемент
		m_nError = IviConfig_CreateDataComponent(hDataComponentStructCollection, nType, pComponentName, &hDataComponent);

		if(m_nError != IVI_SUCCESS)
			return m_nError;

		// Заполняем элемент данными
		m_nError = SetDataComponent(hDataComponent, rDataComponent.vrDataComponent[i]);

		if(m_nError != IVI_SUCCESS)
			return m_nError;
	}

	return m_nError;
}

ViInt32	IviConfSrv::SetDataComponent(IviDataComponentHandle hDataComponent, TIVIDataComponent rDataComponent)
{
	ClearError();

	m_nError = IVICFG_SETPROPERTY_STR(DataComponent, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_DESCRIPTION, rDataComponent.sDescription);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY(DataComponent, ViInt32, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_HELP_CONTEXT_ID, rDataComponent.nHelpContextID);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(DataComponent, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_HELP_FILE_PATH, rDataComponent.sHelpFilePath);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(DataComponent, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_NAME, rDataComponent.sName);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY(DataComponent, ViBoolean, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_READ_ONLY, rDataComponent.isReadOnly);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	m_nError = IVICFG_SETPROPERTY_STR(DataComponent, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_SW_MODULE_KEY, rDataComponent.sSoftwareModuleKey);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	if(strcmp(rDataComponent.sUsedInSession, "False") == 0)
		strcpy(rDataComponent.sUsedInSession, "Optional");
	else
		strcpy(rDataComponent.sUsedInSession, "Required");

	m_nError = IVICFG_SETPROPERTY_STR(DataComponent, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_USED_IN_SESSION, rDataComponent.sUsedInSession);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	if(strcmp(rDataComponent.sType, "String") == 0)
		m_nError = IVICFG_SETPROPERTY_STR(DataComponent, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_VALUE, rDataComponent.sValue);
	else if(strcmp(rDataComponent.sType, "Real") == 0)
	{
		ViReal64 fVal = atof(rDataComponent.sValue);
		m_nError = IVICFG_SETPROPERTY(DataComponent, ViReal64, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_VALUE, fVal);
	}
	else if(strcmp(rDataComponent.sType, "Integer") == 0)
	{
		ViInt32 nVal = atoi(rDataComponent.sValue);
		m_nError = IVICFG_SETPROPERTY(DataComponent, ViInt32, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_VALUE, nVal);
	}
	else if(strcmp(rDataComponent.sType, "Boolean") == 0)
	{
		ViBoolean nVal;

		if(strcmp(rDataComponent.sValue, "True") == 0)
			nVal = 1;
		else
			nVal = 0;
		
		m_nError = IVICFG_SETPROPERTY(DataComponent, ViBoolean, hDataComponent, IVICONFIG_VAL_DATA_COMPONENT_VALUE, nVal);
	}

	return m_nError;
}

ViInt32 IviConfSrv::GetVirtualNames(IviVirtualNameCollectionHandle hVirtualNameCollection, vector<TIVIVirtualName> &vrVirtualName)
{
	ViInt32 i;
	ViInt32 nCount;
	IviVirtualNameHandle hVirtualName;
	TIVIVirtualName rVirtualName;

	m_nError = IVICFG_GETCOUNT(VirtualName, hVirtualNameCollection, &nCount);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	for(i = 0; i < nCount; i++)
	{
		m_nError = IVICFG_GETITEMBYIDX(VirtualName, hVirtualNameCollection, i + 1, &hVirtualName);

		if(m_nError != IVI_SUCCESS)
			return m_nError;

		m_nError = IVICFG_GETPROPERTY_STR(VirtualName, hVirtualName, IVICONFIG_VAL_VIRTUAL_NAME_NAME, rVirtualName.sName);

		if(m_nError != IVI_SUCCESS)
			return m_nError;

		m_nError = IVICFG_GETPROPERTY_STR(VirtualName, hVirtualName, IVICONFIG_VAL_VIRTUAL_NAME_MAPTO, rVirtualName.sMapTo);

		if(m_nError != IVI_SUCCESS)
			return m_nError;

		vrVirtualName.push_back(rVirtualName);
	}

	return m_nError;
}

ViInt32 IviConfSrv::SetVirtualNames(IviVirtualNameCollectionHandle hVirtualNameCollection, vector<TIVIVirtualName> vrVirtualName)
{
	ViInt32 i;
	ViInt32 nCount;
	IviVirtualNameHandle hVirtualName;
	TIVIVirtualName rVirtualName;

	m_nError = IVICFG_GETCOUNT(VirtualName, hVirtualNameCollection, &nCount);

	if(m_nError != IVI_SUCCESS)
		return m_nError;

	// Удаляем старые записи
	for(i = 0; i < nCount; i++)
	{
		m_nError = IVICFG_GETITEMBYIDX(VirtualName, hVirtualNameCollection, 1, &hVirtualName);

		if(m_nError != IVI_SUCCESS)
			return m_nError;

		m_nError = IVICFG_DESTROY(VirtualName, hVirtualNameCollection, hVirtualName);

		if(m_nError != IVI_SUCCESS)
			return m_nError;
	}

	// Заполняем новыми
	for(i = 0; i < vrVirtualName.size(); i++)
	{
		m_nError = IviConfig_CreateVirtualName(hVirtualNameCollection, vrVirtualName[i].sName, vrVirtualName[i].sMapTo, &hVirtualName);

		if(m_nError != IVI_SUCCESS)
			return m_nError;
	}
	
	return m_nError;
}

// Очистка кода и информации об ошибке
void IviConfSrv::ClearError()
{
	m_nError = 0;
	m_sErrorInfo[0] = '\0';
}