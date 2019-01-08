// EngineAPI.cpp: implementation of the CEngineAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EngineAPI.h"
#include "cdb/xrXRC.h"

#include "securom_api.h"

#include "BearCore.hpp"
#include "api/XrGameVersionController.h"
extern xr_token* vid_quality_token;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __cdecl dummy(void)
{
};
CEngineAPI::CEngineAPI():bRender(false)
{
    pCreate = 0;
    pDestroy = 0;
    tune_pause = dummy;
    tune_resume = dummy;
}

CEngineAPI::~CEngineAPI()
{
    // destroy quality token here
    if (vid_quality_token)
    {
        for (int i = 0; vid_quality_token[i].name; i++)
        {
            xr_free(vid_quality_token[i].name);
        }
        xr_free(vid_quality_token);
        vid_quality_token = NULL;
    }
}

extern u32 renderer_value; //con cmd
ENGINE_API int g_current_renderer = 0;

ENGINE_API bool is_enough_address_space_available()
{
    SYSTEM_INFO system_info;

    SECUROM_MARKER_HIGH_SECURITY_ON(12)

        GetSystemInfo(&system_info);

    SECUROM_MARKER_HIGH_SECURITY_OFF(12)

        return (*(u32*)&system_info.lpMaximumApplicationAddress) > 0x90000000;
}

#ifndef DEDICATED_SERVER

void CEngineAPI::InitializeNotDedicated()
{
    SECUROM_MARKER_HIGH_SECURITY_ON(2)

   /*LPCSTR r2_name = "StalkerRender2.dll";
    LPCSTR r3_name = "StalkerRender3.dll";
    LPCSTR r4_name = "StalkerRender4.dll";

    if (psDeviceFlags.test(rsR4))
    {
        // try to initialize R4
        Log("Loading DLL:", r4_name);
        hRender = LoadLibrary(r4_name);
        if (0 == hRender)
        {
            // try to load R1
            Msg("! ...Failed - incompatible hardware/pre-Vista OS.");
            psDeviceFlags.set(rsR2, TRUE);
        }
    }

    if (psDeviceFlags.test(rsR3))
    {
        // try to initialize R3
        Log("Loading DLL:", r3_name);
        hRender = LoadLibrary(r3_name);
        if (0 == hRender)
        {
            // try to load R1
            Msg("! ...Failed - incompatible hardware/pre-Vista OS.");
            psDeviceFlags.set(rsR2, TRUE);
        }
        else
            g_current_renderer = 3;
    }

    if (psDeviceFlags.test(rsR2))
    {
        // try to initialize R2
        psDeviceFlags.set(rsR4, FALSE);
        psDeviceFlags.set(rsR3, FALSE);
        Log("Loading DLL:", r2_name);
        hRender = LoadLibrary(r2_name);
        if (0 == hRender)
        {
            // try to load R1
            Msg("! ...Failed - incompatible hardware.");
        }
        else
            g_current_renderer = 2;
    }
	*/
    SECUROM_MARKER_HIGH_SECURITY_OFF(2)
}
#endif // DEDICATED_SERVER


void CEngineAPI::Initialize(void)
{
    //////////////////////////////////////////////////////////////////////////
    // render
    LPCSTR r1_name =
#ifdef MIXED
		"stalker_r1_mixed.dll"
#elif DEBUG
		"stalker_r1_debug.dll"
#else
		"stalker_r1"
#endif
		;

#ifndef DEDICATED_SERVER
    InitializeNotDedicated();
#endif // DEDICATED_SERVER

    if (!bRender)
    {
        // try to load R1
        psDeviceFlags.set(rsR4, FALSE);
        psDeviceFlags.set(rsR3, FALSE);
        psDeviceFlags.set(rsR2, FALSE);
        renderer_value = 0; //con cmd
	
        Log("Loading DLL:", TEXT("stalker_r1") );

		R_ASSERT(BearCore::BearProjectTool::CheckProject(TEXT("stalker_r1")));
		bRender = true;
        g_current_renderer = 1;
    }

    Device.ConnectToRender();

    // game

	{
		const bchar*name = TEXT("stalkersoc");
		switch (gameVersionController->getGame())
		{
		case GameVersionController::SOC:
			break;
		case GameVersionController::COP:
			name = TEXT("stalkercop");
			break;
		case GameVersionController::CS:
			name = TEXT("stalkercs");
			break;
		default:
			NODEFAULT;
			break;
		}
        Log("Loading DLL:", name);
        R_ASSERT2(BearCore::BearProjectTool::CheckProject(name), "Game DLL raised exception during loading or there is no game DLL at all");
        pCreate = BearCore::BearProjectTool::GetFunctionInProject< Factory_Create*>(name, TEXT("xrFactory_Create"));
		pDestroy = BearCore::BearProjectTool::GetFunctionInProject< Factory_Destroy*>(name, TEXT("xrFactory_Destroy"));
    }

    //////////////////////////////////////////////////////////////////////////
    // vTune
    tune_enabled = FALSE;
   
}

void CEngineAPI::Destroy(void)
{
    pCreate = 0;
    pDestroy = 0;
    Engine.Event._destroy();
    XRC.r_clear_compact();
}

extern "C" {
    typedef bool __cdecl SupportsAdvancedRendering(void);
    typedef bool _declspec(dllexport) SupportsDX10Rendering();
    typedef bool _declspec(dllexport) SupportsDX11Rendering();
};

void CEngineAPI::CreateRendererList()
{
#ifdef DEDICATED_SERVER

    vid_quality_token = xr_alloc<xr_token>(2);

    vid_quality_token[0].id = 0;
    vid_quality_token[0].name = xr_strdup("renderer_r1");

    vid_quality_token[1].id = -1;
    vid_quality_token[1].name = NULL;

#else
    // TODO: ask renderers if they are supported!
    if (vid_quality_token != NULL) return;
    bool bSupports_r2 = false;
    bool bSupports_r2_5 = false;
    bool bSupports_r3 = false;
    bool bSupports_r4 = false;

    LPCSTR r2_name = "xrRender_R2.dll";
    LPCSTR r3_name = "xrRender_R3.dll";
    LPCSTR r4_name = "xrRender_R4.dll";

    if (strstr(GetCommandLine(), "-perfhud_hack"))
    {
        bSupports_r2 = true;
        bSupports_r2_5 = true;
        bSupports_r3 = true;
        bSupports_r4 = true;
    }
    else
    {
        // try to initialize R2
      /*  Log("Loading DLL:", r2_name);
        hRender = LoadLibrary(r2_name);
        if (hRender)
        {
            bSupports_r2 = true;
            SupportsAdvancedRendering* test_rendering = (SupportsAdvancedRendering*)GetProcAddress(hRender, "SupportsAdvancedRendering");
            R_ASSERT(test_rendering);
            bSupports_r2_5 = test_rendering();
            FreeLibrary(hRender);
        }

        // try to initialize R3
        Log("Loading DLL:", r3_name);
        // Hide "d3d10.dll not found" message box for XP
        SetErrorMode(SEM_FAILCRITICALERRORS);
        hRender = LoadLibrary(r3_name);
        // Restore error handling
        SetErrorMode(0);
        if (hRender)
        {
            SupportsDX10Rendering* test_dx10_rendering = (SupportsDX10Rendering*)GetProcAddress(hRender, "SupportsDX10Rendering");
            R_ASSERT(test_dx10_rendering);
            bSupports_r3 = test_dx10_rendering();
            FreeLibrary(hRender);
        }

        // try to initialize R4
        Log("Loading DLL:", r4_name);
        // Hide "d3d10.dll not found" message box for XP
        SetErrorMode(SEM_FAILCRITICALERRORS);
        hRender = LoadLibrary(r4_name);
        // Restore error handling
        SetErrorMode(0);
        if (hRender)
        {
            SupportsDX11Rendering* test_dx11_rendering = (SupportsDX11Rendering*)GetProcAddress(hRender, "SupportsDX11Rendering");
            R_ASSERT(test_dx11_rendering);
            bSupports_r4 = test_dx11_rendering();
            FreeLibrary(hRender);
        }*/
    }

    bRender = 0;

    xr_vector<LPCSTR> _tmp;
    u32 i = 0;
    bool bBreakLoop = false;
    for (; i < 6; ++i)
    {
        switch (i)
        {
        case 1:
            if (!bSupports_r2)
                bBreakLoop = true;
            break;
        case 3: //"renderer_r2.5"
            if (!bSupports_r2_5)
                bBreakLoop = true;
            break;
        case 4: //"renderer_r_dx10"
            if (!bSupports_r3)
                bBreakLoop = true;
            break;
        case 5: //"renderer_r_dx11"
            if (!bSupports_r4)
                bBreakLoop = true;
            break;
        default:
            ;
        }

        if (bBreakLoop) break;

        _tmp.push_back(NULL);
        LPCSTR val = NULL;
        switch (i)
        {
        case 0:
            val = "renderer_r1";
            break;
        case 1:
            val = "renderer_r2a";
            break;
        case 2:
            val = "renderer_r2";
            break;
        case 3:
            val = "renderer_r2.5";
            break;
        case 4:
            val = "renderer_r3";
            break; // -)
        case 5:
            val = "renderer_r4";
            break; // -)
        }
        if (bBreakLoop) break;
        _tmp.back() = xr_strdup(val);
    }
    u32 _cnt = _tmp.size() + 1;
    vid_quality_token = xr_alloc<xr_token>(_cnt);

    vid_quality_token[_cnt - 1].id = -1;
    vid_quality_token[_cnt - 1].name = NULL;

#ifdef DEBUG
    Msg("Available render modes[%d]:", _tmp.size());
#endif // DEBUG
    for (u32 i = 0; i < _tmp.size(); ++i)
    {
        vid_quality_token[i].id = i;
        vid_quality_token[i].name = _tmp[i];
#ifdef DEBUG
        Msg("[%s]", _tmp[i]);
#endif // DEBUG
    }

    /*
    if(vid_quality_token != NULL) return;

    D3DCAPS9 caps;
    CHW _HW;
    _HW.CreateD3D ();
    _HW.pD3D->GetDeviceCaps (D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps);
    _HW.DestroyD3D ();
    u16 ps_ver_major = u16 ( u32(u32(caps.PixelShaderVersion)&u32(0xf << 8ul))>>8 );

    xr_vector<LPCSTR> _tmp;
    u32 i = 0;
    for(; i<5; ++i)
    {
    bool bBreakLoop = false;
    switch (i)
    {
    case 3: //"renderer_r2.5"
    if (ps_ver_major < 3)
    bBreakLoop = true;
    break;
    case 4: //"renderer_r_dx10"
    bBreakLoop = true;
    break;
    default: ;
    }

    if (bBreakLoop) break;

    _tmp.push_back (NULL);
    LPCSTR val = NULL;
    switch (i)
    {
    case 0: val ="renderer_r1"; break;
    case 1: val ="renderer_r2a"; break;
    case 2: val ="renderer_r2"; break;
    case 3: val ="renderer_r2.5"; break;
    case 4: val ="renderer_r_dx10"; break; // -)
    }
    _tmp.back() = xr_strdup(val);
    }
    u32 _cnt = _tmp.size()+1;
    vid_quality_token = xr_alloc<xr_token>(_cnt);

    vid_quality_token[_cnt-1].id = -1;
    vid_quality_token[_cnt-1].name = NULL;

    #ifdef DEBUG
    Msg("Available render modes[%d]:",_tmp.size());
    #endif // DEBUG
    for(u32 i=0; i<_tmp.size();++i)
    {
    vid_quality_token[i].id = i;
    vid_quality_token[i].name = _tmp[i];
    #ifdef DEBUG
    Msg ("[%s]",_tmp[i]);
    #endif // DEBUG
    }
    */
#endif //#ifndef DEDICATED_SERVER
}