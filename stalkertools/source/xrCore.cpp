// xrCore.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#pragma hdrstop



#include "xrCore.h"
XRCORE_API Fmatrix Fidentity;
XRCORE_API Dmatrix Didentity;
XRCORE_API CRandom Random; 

#pragma warning(push)
#pragma warning(disable:4996)
auto __vsnprintf = _vsnprintf;
#pragma warning(pop)


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

void xrCore::Initialize()
{
	if (init_counter)return;	init_counter++;
	DWORD sz_user = sizeof(UserName);
	GetUserName(UserName, &sz_user);

	DWORD sz_comp = sizeof(CompName);
	GetComputerName(CompName, &sz_comp);

	Fidentity.identity(); 
	Didentity.identity();
	XrCompressor::Initialize();
	XrStringContainer::Initilize();
	XrSharedMemoryContainer::Initilize();
	CInifile::Initialize();
	XrThread::Initialize();
	//XrTimerController::Initialize();
}


void xrCore::Destroy()
{

	if (0 == init_counter)
	{
		XrCompressor::Destroy();
		XrStringContainer::Destroy();
		XrSharedMemoryContainer::Destroy();
		CInifile::Destroy();
		XrThread::Destroy();
		XrCompressor::Destroy();
	}else

	{
		--init_counter;
	}
}

