// xrRender_R2.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "xrRender/dxRenderFactory.h"
#include "xrRender/dxUIRender.h"
#include "xrRender/dxDebugRender.h"
extern void setup_luabind_allocator();
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH	:
		//	Can't call CreateDXGIFactory from DllMain
		//if (!xrRender_test_hw())	return FALSE;
		::Render					= &RImplementation;
		::RenderFactory				= &RenderFactoryImpl;
		::DU						= &DUImpl;
		//::vid_mode_token			= inited by HW;
		UIRender					= &UIRenderImpl;
#ifdef DEBUG
		DRender						= &DebugRenderImpl;
#endif	//	DEBUG
		FS.SubPath(TEXT("%cur_shaders%"));
		FS.SubPath(TEXT("%shaders_cache%"));
		FS.AppendPath(TEXT("%cur_shaders%"), ::Render->getShaderPath(), TEXT("%shaders%"), 0);
		FS.AppendPath(TEXT("%shaders_cache%"), ::Render->getShaderPath(), TEXT("%user%"), 0);
		FS.CreateDirectory(TEXT("%shaders_cache%"), 0);
		xrRender_initconsole		();
		setup_luabind_allocator();
		break	;
	case DLL_THREAD_ATTACH	:
		break;
	case DLL_THREAD_DETACH	:
		break;
	case DLL_PROCESS_DETACH	:
	
		break;
	}
	return TRUE;
}


extern "C"
{
	bool _declspec(dllexport) SupportsDX10Rendering();
};

bool _declspec(dllexport) SupportsDX10Rendering()
{
	return xrRender_test_hw()?true:false;
	/*
	D3DCAPS9					caps;
	CHW							_HW;
	_HW.CreateD3D				();
	_HW.pD3D->GetDeviceCaps		(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps);
	_HW.DestroyD3D				();
	u16		ps_ver_major		= u16 ( u32(u32(caps.PixelShaderVersion)&u32(0xf << 8ul))>>8 );

	if (ps_ver_major<3)
		return false;
	else
		return true;
	*/
}