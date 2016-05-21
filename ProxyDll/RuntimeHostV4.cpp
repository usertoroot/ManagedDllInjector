#include "stdafx.h"
#include "RuntimeHostV4.h"

RuntimeHostV4::RuntimeHostV4()
{
	pMetaHost = NULL;
	pRuntimeInfo = NULL;
	pCorRuntimeHost = NULL;
	spAppDomainThunk = NULL;
    spDefaultAppDomain = NULL;
}

RuntimeHostV4::~RuntimeHostV4()
{
	Unload();
}

bool RuntimeHostV4::Load()
{
	PCWSTR pszVersion = L"v4.0.30319";

	HRESULT hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_PPV_ARGS(&pMetaHost));
    if (FAILED(hr))
    {
        wprintf(L"CLRCreateInstance failed w/hr 0x%08lx\n", hr);
        return false;
    }

	hr = pMetaHost->GetRuntime(pszVersion, IID_PPV_ARGS(&pRuntimeInfo));
    if (FAILED(hr))
    {
        wprintf(L"ICLRMetaHost::GetRuntime failed w/hr 0x%08lx\n", hr);
		Unload();
        return false;
    }

	BOOL fLoadable;
    hr = pRuntimeInfo->IsLoadable(&fLoadable);
    if (FAILED(hr))
    {
        wprintf(L"ICLRRuntimeInfo::IsLoadable failed w/hr 0x%08lx\n", hr);
		Unload();
		return false;
    }

    if (!fLoadable)
    {
		wprintf(L".NET runtime %s cannot be loaded\n", pszVersion);
		Unload();
        return false;
    }

	hr = pRuntimeInfo->GetInterface(CLSID_CorRuntimeHost, IID_PPV_ARGS(&pCorRuntimeHost));
    if (FAILED(hr))
    {
        wprintf(L"ICLRRuntimeInfo::GetInterface failed w/hr 0x%08lx\n", hr);
		Unload();
		return false;
    }

    hr = pCorRuntimeHost->Start();
    if (FAILED(hr))
    {
        wprintf(L"CLR failed to start w/hr 0x%08lx\n", hr);
		Unload();
		return false;
    }

	hr = pCorRuntimeHost->GetDefaultDomain(&spAppDomainThunk);
    if (FAILED(hr))
    {
        wprintf(L"ICorRuntimeHost::GetDefaultDomain failed w/hr 0x%08lx\n", hr);
		Unload();
		return false;
    }

    hr = spAppDomainThunk->QueryInterface(IID_PPV_ARGS(&spDefaultAppDomain));
    if (FAILED(hr))
    {
        wprintf(L"Failed to get default AppDomain w/hr 0x%08lx\n", hr);
		Unload();
		return false;
    }

	return true;
}

void RuntimeHostV4::Unload()
{
	if (pCorRuntimeHost)
	{
		pCorRuntimeHost->Release();
		pCorRuntimeHost = NULL;
	}

	if (pRuntimeInfo)
	{
		pRuntimeInfo->Release();
		pRuntimeInfo = NULL;
	}

	if (pMetaHost)
	{
		pMetaHost->Release();
		pMetaHost = NULL;
	}
}

_AssemblyPtr RuntimeHostV4::LoadAssembly(PCWSTR pszAssemblyName)
{
	if (!pCorRuntimeHost)
		return NULL;

	_AssemblyPtr spAssembly = NULL;
    HRESULT hr = spDefaultAppDomain->Load_2(bstr_t(pszAssemblyName), &spAssembly);
    if (FAILED(hr))
    {
        wprintf(L"Failed to load the assembly w/hr 0x%08lx\n", hr);
        return NULL;
    }

	return spAssembly;
}

_AssemblyPtr RuntimeHostV4::ExecuteAssembly(PCWSTR pszAssemblyName, PCWSTR pszClassName, PCWSTR pszMethodName, SAFEARRAY* psaMethodArgs)
{
	_AssemblyPtr assembly = LoadAssembly(pszAssemblyName);
	if (!assembly)
		return NULL;

	_TypePtr spType = NULL;
	HRESULT hr = assembly->GetType_2(bstr_t(pszClassName), &spType);
	if (FAILED(hr))
	{
		wprintf(L"Failed to get type /hr 0x%08lx\n", hr);
		return NULL;
	}

	variant_t vtEmpty;
	variant_t vtReturnValue;
	hr = spType->InvokeMember_3(bstr_t(pszMethodName), static_cast<BindingFlags>(BindingFlags_InvokeMethod | BindingFlags_Static | BindingFlags_Public), NULL, vtEmpty, psaMethodArgs, &vtReturnValue);
	if (FAILED(hr))
	{
		wprintf(L"Failed to invoke the member function. /hr 0x%08lx\n", hr);
		return NULL;
	}

	return assembly;
}

_AssemblyPtr RuntimeHostV4::ExecuteAssembly(PCWSTR pszAssemblyName, PCWSTR pszClassName, PCWSTR pszMethodName, PCWSTR pszStringArgument)
{
	SAFEARRAY* psaMethodArgs = SafeArrayCreateVector(VT_VARIANT, 0, 1);
    LONG index = 0;

	variant_t vtStringArg(pszStringArgument);
    HRESULT hr = SafeArrayPutElement(psaMethodArgs, &index, &vtStringArg);
    if (FAILED(hr))
    {
        wprintf(L"SafeArrayPutElement failed w/hr 0x%08lx\n", hr);
        return NULL;
    }

	return ExecuteAssembly(pszAssemblyName, pszClassName, pszMethodName, psaMethodArgs);
}