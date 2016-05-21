#include "stdafx.h"
#include "RuntimeHostV4.h"

RuntimeHostV4* g_runtimeHost;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" void _declspec(dllexport) Initialize()
{
	if (!g_runtimeHost->Load())
	{
		wsprintf(L"%s", L"Failed to host runtime.");
		return;
	}

	if (!g_runtimeHost->ExecuteAssembly(L"C:\\Users\\Koen\\Documents\\visual studio 2012\\Projects\\ManagedDllInjector\\ManagedDllTest\\bin\\Debug\\ManagedDllTest.exe", L"ManagedDllTest.Program", L"DllMain", L"Hello World!"))
	{
		wsprintf(L"%s", L"Failed to execute assembly.");
		return;
	}
}