// xrCore.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#pragma hdrstop



XRCORE_API Fmatrix Fidentity;
XRCORE_API Dmatrix Didentity;
XRCORE_API CRandom Random; 

#pragma warning(push)
#pragma warning(disable:4996)
auto __vsnprintf = _vsnprintf;
#pragma warning(pop)


XRCORE_API u32 build_id;
XRCORE_API LPCSTR build_date;


static u32 init_counter = 0;

extern char g_application_path[256];

//. extern xr_vector<shared_str>* LogFile;
string64 XrCore::UserName;
string64 XrCore::CompName;
u32 XrCore::dwFrame=0;


bool shared_str_initialized = false;

void XrCore::Initialize()
{
	if (init_counter)return;	init_counter++;
	DWORD sz_user = sizeof(UserName);
	GetUserName(UserName, &sz_user);

	DWORD sz_comp = sizeof(CompName);
	GetComputerName(CompName, &sz_comp);

	Fidentity.identity(); 
	Didentity.identity();
	XrCompressor::Initialize();
	XrStringContainer::Initialize();
	XrSharedMemoryContainer::Initialize();
	CInifile::Initialize();
	XrThread::Initialize();
	//XrTimerController::Initialize();
}


void XrCore::Destroy()
{
	if(init_counter!=0) --init_counter;
	if (! init_counter)
	{
		
		XrCompressor::Destroy();
		XrStringContainer::Destroy();
		XrSharedMemoryContainer::Destroy();
		//CInifile::Destroy();
		XrThread::Destroy();
		XrCompressor::Destroy();
	}
}

