#ifndef IVICONFSRV_H
#define IVICONFSRV_H

#include "IviConfigServer.h"

#include <vector>

using namespace std;

#define IVI_MAXSIZE 16384

#define IVICFG_GETPROPERTY_STR(ItemName, handle, id, buf) \
		IviConfig_Get##ItemName##PropertyViString(handle, id, IVI_MAXSIZE, buf)

#define IVICFG_SETPROPERTY_STR(ItemName, handle, id, buf) \
		IviConfig_Set##ItemName##PropertyViString(handle, id, buf)

#define IVICFG_GETPROPERTY(ItemName, Type, handle, id, buf) \
		IviConfig_Get##ItemName##Property##Type##(handle, id, buf)

#define IVICFG_SETPROPERTY(ItemName, Type, handle, id, buf) \
		IviConfig_Set##ItemName##Property##Type##(handle, id, buf)

#define IVICFG_GETCOLLECTION(CollectionName, ConfigStoreHandle, CollectionHandle) \
		IviConfig_GetConfigStore##CollectionName##Collection(ConfigStoreHandle, CollectionHandle)

#define IVICFG_GETCOUNT(CollectionName, CollectionHandle, ItemCount) \
		IviConfig_Get##CollectionName##Count(CollectionHandle, ItemCount)

#define IVICFG_GETITEMBYIDX(CollectionName, CollectionHandle, ItemIndex, ItemHandle) \
		IviConfig_Get##CollectionName##ItemByIndex(CollectionHandle, ItemIndex, ItemHandle)

#define IVICFG_GETITEMBYNAME(CollectionName, CollectionHandle, ItemName, ItemHandle) \
		IviConfig_Get##CollectionName##ItemByName(CollectionHandle, ItemName, ItemHandle)

#define IVICFG_CREATE(CollectionName, CollectionHandle, ItemName, ItemHandle) \
		IviConfig_Create##CollectionName##(CollectionHandle, ItemName, ItemHandle)

#define IVICFG_DESTROY(CollectionName, CollectionHandle, ItemHandle) \
		IviConfig_Destroy##CollectionName##(CollectionHandle, ItemHandle)

struct TIVIDataComponent
{
	ViChar    sName[IVI_MAXSIZE];
	ViChar    sDescription[IVI_MAXSIZE];
	ViInt32   nHelpContextID;
	ViChar    sHelpFilePath[IVI_MAXSIZE];
	ViBoolean isReadOnly;
	ViChar	  sSoftwareModuleKey[IVI_MAXSIZE];
	ViChar	  sType[IVI_MAXSIZE];
	ViChar	  sUsedInSession[IVI_MAXSIZE];
	ViChar	  sValue[IVI_MAXSIZE];
	vector<TIVIDataComponent> vrDataComponent;

	TIVIDataComponent()
	{
		sName[0] = '\0';
		sDescription[0] = '\0';
		nHelpContextID = 0;
		sHelpFilePath[0] = '\0';
		isReadOnly = 0;
		sSoftwareModuleKey[0] = '\0';
		sType[0] = '\0';
		sUsedInSession[0] = '\0';
		sValue[0] = '\0';
	}
};

struct TIVIConfigStore
{
	ViChar sActualLocation[IVI_MAXSIZE];
	ViChar sMasterLocation[IVI_MAXSIZE];
	ViChar sRevision[IVI_MAXSIZE];

	TIVIConfigStore()
	{
		sActualLocation[0] = '\0';
		sMasterLocation[0] = '\0';
		sRevision[0] = '\0';
	}
};

struct TIVILogicalName
{
	ViChar sName[IVI_MAXSIZE];
	ViChar sSessionName[IVI_MAXSIZE];
	ViChar sDescription[IVI_MAXSIZE];

	TIVILogicalName()
	{
		sName[0] = '\0';
		sSessionName[0] = '\0';
		sDescription[0] = '\0';
	}
};

struct TIVIVirtualName
{
	ViChar sName[IVI_MAXSIZE];
	ViChar sMapTo[IVI_MAXSIZE];

	TIVIVirtualName()
	{
		sName[0]  = '\0';
		sMapTo[0] = '\0';
	}
};

struct TIVIHardwareAsset
{
	ViChar sName[IVI_MAXSIZE];
	ViChar sDescription[IVI_MAXSIZE];
	ViChar sIORcDescriptor[IVI_MAXSIZE];

	TIVIHardwareAsset()
	{
		sName[0] = '\0';
		sDescription[0] = '\0';
		sIORcDescriptor[0] = '\0';
	}
};

struct TIVIDriverSession
{
	ViChar sName[IVI_MAXSIZE];
	ViChar sSoftwareModuleName[IVI_MAXSIZE];
	ViChar sDescription[IVI_MAXSIZE];
	ViBoolean isCache;
	ViChar sDriverSetup[IVI_MAXSIZE];
	ViBoolean isInterchangeCheck;
	ViBoolean isQueryInstrStatus;
	ViBoolean isRangeCheck;
	ViBoolean isRecordCoercions;
	ViBoolean isSimulate;
	ViChar sHwAssetName[IVI_MAXSIZE];
	vector<TIVIVirtualName> vrVirtualName;
	TIVIDataComponent rInitialSettings;

	TIVIDriverSession()
	{
		sName[0] = '\0';
		sSoftwareModuleName[0] = '\0';
		sDescription[0] = '\0';
		isCache = 0;
		sDriverSetup[0] = '\0';
		isInterchangeCheck = 0;
		isQueryInstrStatus = 0;
		isRangeCheck = 0;
		isRecordCoercions = 0;
		isSimulate = 0;
		sHwAssetName[0] = '\0';
	}
};

struct TIVIPublishedAPI
{
	ViInt32 nMajorVersion;
	ViInt32 nMinorVersion;
	ViChar	sName[IVI_MAXSIZE];
	ViChar	sType[IVI_MAXSIZE];

	TIVIPublishedAPI()
	{
		nMajorVersion = 1;
		nMinorVersion = 0;
		strcpy(sName, "IviDriver");
		strcpy(sType, "IVI-C");
	}
};

struct TIVIPhysicalName
{
	ViChar	sName[IVI_MAXSIZE];
	ViChar	sRCName[IVI_MAXSIZE];
	ViChar	sQualifier[IVI_MAXSIZE];

	TIVIPhysicalName()
	{
		strcpy(sName, "NewName");
		strcpy(sRCName, "Channel");
		sQualifier[0] = '\0';
	}
};

struct TIVISoftwareModule
{
	ViChar sName[IVI_MAXSIZE];
	ViChar sDescription[IVI_MAXSIZE];
	ViChar sAssemblyQualifiedClassName[IVI_MAXSIZE];
	ViChar sModulePath[IVI_MAXSIZE];
	ViChar sModulePath32[IVI_MAXSIZE];
	ViChar sModulePath64[IVI_MAXSIZE];
	ViChar sPrefix[IVI_MAXSIZE];
	ViChar sProgID[IVI_MAXSIZE];
	ViChar sSupportedInstrumentModels[IVI_MAXSIZE];
	vector<TIVIPublishedAPI> vrPublishedAPI;
	vector<TIVIPhysicalName> vrPhysicalName;
	TIVIDataComponent rConfigSettings;

	TIVISoftwareModule()
	{
		sName[0] = '\0';
		sDescription[0] = '\0';
		sAssemblyQualifiedClassName[0] = '\0';
		sModulePath[0] = '\0';
		sModulePath32[0] = '\0';
		sModulePath64[0] = '\0';
		sPrefix[0] = '\0';
		sProgID[0] = '\0';
		sSupportedInstrumentModels[0] = '\0';
	}
};

class IviConfSrv
{
	ViInt32 m_nError;
	ViChar	m_sErrorInfo[IVI_MAXSIZE];	// Информация об ошибке
	
	IviConfigStoreHandle				m_hIviConfigStore;
	IviDriverSessionCollectionHandle	m_hDriverSessionCollection;
	IviLogicalNameCollectionHandle		m_hLogicalNameCollection;
	IviSoftwareModuleCollectionHandle	m_hSoftwareModuleCollection;
	IviPublishedAPICollectionHandle		m_hPublishedAPICollection;
	IviHardwareAssetCollectionHandle    m_hHwAssetCollection;

public:
	IviConfSrv();
	~IviConfSrv();

	// Чтение конфигурационного файла
	ViInt32 Deserialize(ViChar *pLocation);
	// Сохранение конфигурационного файла
	ViInt32 Serialize(ViChar *pLocation);

	// Получить код ошибки
	ViInt32 GetError();
	// Получение информации об ошибке
	ViInt32 GetErrorInfo(ViChar *pInfo, ViInt32 nSize);

	ViInt32 GetConfigStore(TIVIConfigStore *pConfigStore);

	// LogicalName
	ViInt32 GetLogicalNameCount();
	ViInt32 GetLogicalName(ViInt32 nIdx, TIVILogicalName *pLogicalName);
	ViInt32 GetLogicalName(ViChar *pName, TIVILogicalName *pLogicalName);
	ViInt32 GetLogicalName(IviLogicalNameHandle hLogicalName, TIVILogicalName *pLogicalName);
	ViInt32 SetLogicalName(TIVILogicalName rLogicalName);
	ViInt32 CreateLogicalName(ViChar *pName);
	ViInt32 DestroyLogicalName(ViChar *pName);
	
	// DriverSession
	ViInt32 GetDriverSessionCount();
	ViInt32 GetDriverSession(ViInt32 nIdx, TIVIDriverSession *pDriverSession);
	ViInt32 GetDriverSession(ViChar *pName, TIVIDriverSession *pDriverSession);
	ViInt32 GetDriverSession(IviDriverSessionHandle hDriverSession, TIVIDriverSession *pDriverSession);
	ViInt32 SetDriverSession(TIVIDriverSession rDriverSession);
	ViInt32 CreateDriverSession(ViChar *pName);
	ViInt32 DestroyDriverSession(ViChar *pName);
	
	// Soft
	ViInt32 GetSoftwareModuleCount();
	ViInt32 GetSoftwareModule(ViInt32 nIdx, TIVISoftwareModule *pSoftwareModule);
	ViInt32 GetSoftwareModule(ViChar *pName, TIVISoftwareModule *pSoftwareModule);
	ViInt32 GetSoftwareModule(IviSoftwareModuleHandle hSoftwareModule, TIVISoftwareModule *pSoftwareModule);
	ViInt32 SetSoftwareModule(TIVISoftwareModule rSoftwareModule);
	ViInt32 CreateSoftwareModule(ViChar *pName);
	ViInt32 DestroySoftwareModule(ViChar *pName);

	// Hardware Asset
	ViInt32 GetHardwareAssets(vector<TIVIHardwareAsset> &vrHwAsset);
	ViInt32 GetHardwareAsset(ViChar *pName, TIVIHardwareAsset *prHwAsset);
	ViInt32 GetHardwareAsset(IviHardwareAssetHandle hHardwareAsset, TIVIHardwareAsset *prHwAsset);
	ViInt32 SetHardwareAssets(vector<TIVIHardwareAsset> vrHwAsset);
	ViInt32 SetHardwareAsset(TIVIHardwareAsset rHwAsset);
	ViInt32 DestroyHardwareAsset(ViChar *pName);
	
private:
	// Published API
	ViInt32 GetPublishedAPICount(IviPublishedAPICollectionHandle hPublishedAPICollection);
	ViInt32 GetPublishedAPI(IviPublishedAPICollectionHandle hPublishedAPICollection, ViInt32 nIdx, TIVIPublishedAPI *pPublishedAPI);
	ViInt32	GetPublishedAPIs(IviSoftwareModuleHandle hSoftwareModule, vector<TIVIPublishedAPI> &vrPublishedAPI);
	ViInt32	SetPublishedAPIs(IviSoftwareModuleHandle hSoftwareModule, vector<TIVIPublishedAPI> vrPublishedAPI);
	ViInt32 CreatePublishedAPI(TIVIPublishedAPI rPublishedAPI, IviPublishedAPIHandle *phPublishedAPI);
	ViInt32 FindPublishedAPI(TIVIPublishedAPI rPublishedAPI, IviPublishedAPIHandle *phPublishedAPI);
	ViInt32 AddPublishedAPIReference(IviPublishedAPICollectionHandle hPublishedAPICollection, IviPublishedAPIHandle hPublishedAPI);
	ViInt32 RemoveAllPublishedAPIReference(IviPublishedAPICollectionHandle hPublishedAPICollection);
	ViInt32 ComparePublishedAPI(TIVIPublishedAPI rPublishedAPI1, TIVIPublishedAPI rPublishedAPI2);

	// Physical Name
	ViInt32 GetPhysicalNameCount(IviPhysicalNameCollectionHandle hPhysicalNameCollection);
	ViInt32 GetPhysicalName(IviPhysicalNameCollectionHandle hPhysicalNameCollection, ViInt32 nIdx, TIVIPhysicalName *pPhysicalName);
	ViInt32	GetPhysicalNames(IviSoftwareModuleHandle hSoftwareModule, vector<TIVIPhysicalName> &vrPhysicalName);
	ViInt32	SetPhysicalNames(IviSoftwareModuleHandle hSoftwareModule, vector<TIVIPhysicalName> vrPhysicalName);
	ViInt32 RemoveAllPhysicalName(IviPhysicalNameCollectionHandle hPhysicalNameCollection);

	// Data Component
	ViInt32	GetDataComponent(IviConfigHandle hIviConfig, ViChar *pName, TIVIDataComponent *pDataComponent);
	ViInt32 GetDataComponent(IviDataComponentCollectionHandle hDataComponentCollection, ViInt32 nIdx, TIVIDataComponent *pDataComponent);
	ViInt32	GetDataComponent(IviDataComponentHandle hDataComponent, TIVIDataComponent *pDataComponent);
	ViInt32	GetDataComponents(IviDataComponentHandle hStructComponent, vector<TIVIDataComponent> &vrDataComponent);
	ViInt32	SetDataComponentStruct(IviConfigHandle hIviConfig, ViChar *pName, TIVIDataComponent rDataComponent);
	ViInt32	SetDataComponent(IviDataComponentHandle hDataComponent, TIVIDataComponent rDataComponent);

	// Virtual Name
	ViInt32 GetVirtualNames(IviVirtualNameCollectionHandle hVirtualNameCollection, vector<TIVIVirtualName> &vrVirtualName);
	ViInt32 SetVirtualNames(IviVirtualNameCollectionHandle hVirtualNameCollection, vector<TIVIVirtualName> vrVirtualName);

	// Очистка кода и информации об ошибке
	void ClearError();
};

#endif // IVICONFSRV_H
