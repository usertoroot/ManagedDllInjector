#pragma once

#include "RuntimeHost.h"

class RuntimeHostV4 : public RuntimeHost
{
public:
	RuntimeHostV4();
	~RuntimeHostV4();

	bool Load();
	void Unload();
	_AssemblyPtr LoadAssembly(PCWSTR pszAssemblyName);
	_AssemblyPtr ExecuteAssembly(PCWSTR pszAssemblyName, PCWSTR pszClassName, PCWSTR pszMethodName, SAFEARRAY* psaMethodArgs);
	_AssemblyPtr ExecuteAssembly(PCWSTR pszAssemblyName, PCWSTR pszClassName, PCWSTR pszMethodName, PCWSTR pszStringArgument);
protected:
private:
	ICLRMetaHost* pMetaHost;
	ICLRRuntimeInfo* pRuntimeInfo;
	ICorRuntimeHost *pCorRuntimeHost;
	IUnknownPtr spAppDomainThunk;
    _AppDomainPtr spDefaultAppDomain;
};