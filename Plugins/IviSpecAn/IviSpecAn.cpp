#include "ivispecan_functions.h"

#include <windows.h>

#ifdef DLL_EXPORTS
#define EXPORT __declspec(dllexport)
#else
#define EXPORT 
#endif

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

EXPORT DeviceUniFunctions* GetDeviceFunctions()
{
	return new IviSpecAnFunctions();
}