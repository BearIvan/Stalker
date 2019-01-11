// xrCore.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#pragma hdrstop

#include <mmsystem.h>
#include <objbase.h>
#include "xrCore.h"

#pragma comment(lib,"winmm.lib")
auto __vsnprintf = _vsnprintf;
#ifdef DEBUG
# include <malloc.h>
#endif // DEBUG

XRCORE_API xrCore Core;
XRCORE_API u32 build_id;
XRCORE_API LPCSTR build_date;

namespace CPU
{
extern void Detect();
};

static u32 init_counter = 0;

extern char g_application_path[256];

//. extern xr_vector<shared_str>* LogFile;
string64 xrCore::UserName;
string64 xrCore::CompName;
u32 xrCore::dwFrame=0;




void xrCore::Initialize(LogCallback cb)
{

	DWORD sz_user = sizeof(UserName);
	GetUserName(UserName, &sz_user);

	DWORD sz_comp = sizeof(CompName);
	GetComputerName(CompName, &sz_comp);

	Memory._initialize( TRUE );

	DUMP_PHASE;

	InitLog();

	rtc_initialize();

	SetLogCB(cb);

	CPU::Detect();
	_initialize_cpu();
	CInifile::Initialize();
	init_counter++;
}

#ifndef _EDITOR
#include "compression_ppmd_stream.h"
extern compression::ppmd::stream* trained_model;
#endif
void xrCore::Destroy()
{
    --init_counter;
	if (0 == init_counter)
	{

#ifndef _EDITOR
		if (trained_model)
		{
			void* buffer = trained_model->buffer();
			xr_free(buffer);
			xr_delete(trained_model);
		}
#endif
		Memory._destroy();
	}
}

#ifndef XRCORE_STATIC

//. why ???
#ifdef _EDITOR
BOOL WINAPI DllEntryPoint(HINSTANCE hinstDLL, DWORD ul_reason_for_call, LPVOID lpvReserved)
#else
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD ul_reason_for_call, LPVOID lpvReserved)
#endif
{

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
      
    }
        //. LogFile.reserve (256);
    break;
    case DLL_THREAD_ATTACH:
       
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
#ifdef USE_MEMORY_MONITOR
        memory_monitor::flush_each_time(true);
#endif // USE_MEMORY_MONITOR
        break;
    }
    return TRUE;
}
#endif // XRCORE_STATIC