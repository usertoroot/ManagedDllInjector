#pragma once

#include <metahost.h>
#include <mscoree.h>

#import "mscorlib.tlb" raw_interfaces_only high_property_prefixes("_get","_put","_putref") rename("ReportEvent", "InteropServices_ReportEvent")

using namespace mscorlib;

class RuntimeHost
{
public:
	RuntimeHost();
	virtual ~RuntimeHost();

	virtual bool Load() = 0;
	virtual void Unload() = 0;
	virtual _AssemblyPtr LoadAssembly(PCWSTR pszAssemblyName) = 0;
protected:
private:
};

