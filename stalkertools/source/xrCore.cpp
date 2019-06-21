// xrCore.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#pragma hdrstop

#include <mmsystem.h>
#include <objbase.h>
#include "xrCore.h"
XRCORE_API Fmatrix Fidentity;
XRCORE_API Dmatrix Didentity;
XRCORE_API CRandom Random; 

#pragma comment(lib,"winmm.lib")
#pragma warning(push)
#pragma warning(disable:4996)
auto __vsnprintf = _vsnprintf;
#pragma warning(pop)
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


bool shared_str_initialized = false;

void xrCore::Initialize(LogCallback cb)
{
	if (init_counter)return;	init_counter++;
	DWORD sz_user = sizeof(UserName);
	GetUserName(UserName, &sz_user);

	DWORD sz_comp = sizeof(CompName);
	GetComputerName(CompName, &sz_comp);

	//Memory._initialize( TRUE );

	DUMP_PHASE;

	InitLog();

	rtc_initialize();

	SetLogCB(cb);

	Fidentity.identity(); 
	Didentity.identity();
	pvInitializeStatics();

	CInifile::Initialize();


	g_pStringContainer = xr_new<str_container>();
	g_pSharedMemoryContainer = xr_new<smem_container>();
	shared_str_initialized = true;

	XrThread::Initialize();
	//XrTimerController::Initialize();
}

#ifndef _EDITOR
#include "compression_ppmd_stream.h"
extern compression::ppmd::stream* trained_model;
#endif
void xrCore::Destroy()
{
	
   
	if (0 == init_counter)
	{
		XrTimerController::Destroy();
		XrThread::Destroy();
		FlushLog();
#ifndef _EDITOR
		if (trained_model)
		{
			void* buffer = trained_model->buffer();
			xr_free(buffer);
			xr_delete(trained_model);
		}
#endif
		shared_str_initialized = false;
		xr_delete(g_pStringContainer);
		xr_delete(g_pSharedMemoryContainer);
		//Memory._destroy();
	}else

	{
		--init_counter;
	}
}

