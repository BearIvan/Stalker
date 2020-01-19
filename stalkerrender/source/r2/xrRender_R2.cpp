// xrRender_R2.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "../xrRender/dxRenderFactory.h"
#include "../xrRender/dxUIRender.h"
#include "../xrRender/dxDebugRender.h"
#include "api/XrGameVersionController.h"
extern void free_luabind();
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH	:
		::Render					= &RImplementation;
		::RenderFactory				= &RenderFactoryImpl;
		::DU						= &DUImpl;
		//::vid_mode_token			= inited by HW;
		UIRender					= &UIRenderImpl;
#ifdef DEBUG
		DRender						= &DebugRenderImpl;
#endif // DEBUG
		xrRender_initconsole		();
		FS.SubPath(TEXT("%cur_shaders%"));
		FS.SubPath(TEXT("%shaders_cache%"));
		FS.AppendPath(TEXT("%cur_shaders%"), ::Render->getShaderPath(), TEXT("%shaders%"), 0);
		FS.AppendPath(TEXT("%shaders_cache%"), ::Render->getShaderPath(), TEXT("%user%"), 0);
		FS.CreateDirectory(TEXT("%shaders_cache%"), 0);
		if (gameVersionController->getGame() == GameVersionController::SOC)
		{
			ps_r2_ls_flags_ext.set(R2FLAGEXT_SUN_OLD, TRUE);
		}

		break	;
	case DLL_THREAD_ATTACH	:
		break;
	case DLL_THREAD_DETACH	:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C"
{
#ifdef MSVC
	__declspec(dllexport) 
#endif
bool  SupportsAdvancedRendering();
};

bool  SupportsAdvancedRendering()
{
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
}