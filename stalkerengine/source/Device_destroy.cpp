#include "stdafx.h"

#include "XrRender/DrawUtils.h"
#include "render.h"
#include "IGame_Persistent.h"
#include "xr_IOConsole.h"

void CRenderDevice::_Destroy(BOOL bKeepTextures)
{
    DU->OnDeviceDestroy();

    // before destroy
    b_is_Ready = FALSE;
    Statistic->OnDeviceDestroy();
    ::Render->destroy();
    m_pRender->OnDeviceDestroy(bKeepTextures);
    //Resources->OnDeviceDestroy (bKeepTextures);
    //RCache.OnDeviceDestroy ();

//    Memory.mem_compact();
}

void CRenderDevice::Destroy(void)
{
    if (!b_is_Ready) return;

    Log("Destroying Direct3D...");

  //  ShowCursor(TRUE);
    m_pRender->ValidateHW();

    _Destroy(FALSE);
	BearCore::bear_delete(window);
    // real destroy
    m_pRender->DestroyHW();

    //xr_delete (Resources);
    //HW.DestroyDevice ();

    seqRender.R.clear();
    seqAppActivate.R.clear();
    seqAppDeactivate.R.clear();
    seqAppStart.R.clear();
    seqAppEnd.R.clear();
    seqFrame.R.clear();
    seqFrameMT.R.clear();
    seqDeviceReset.R.clear();
    seqParallel.clear();

    RenderFactory->DestroyRenderDeviceRender(m_pRender);
    m_pRender = 0;
    xr_delete(Statistic);
	Timer.~XrTimerPaused();
	TimerGlobal.~TimerGlobal();
}

#include "IGame_Level.h"
#include "CustomHUD.h"
#include "EnvironmentSOC.h"
#include "api/XrGameVersionController.h"
extern BOOL bNeed_re_create_env;
void CRenderDevice::Reset(bool precache)
{
	bsize dwWidth_before = dwWidth;
    bsize dwHeight_before = dwHeight;

 //   ShowCursor(TRUE);
    u32 tm_start = TimerAsync();
    if (g_pGamePersistent)
    {

        //. g_pGamePersistent->Environment().OnDeviceDestroy();
    }


	GetWindow().Resize(psCurrentVidMode[0], psCurrentVidMode[1]);
	dwWidth = GetWindow().GetSize().x;
	dwHeight = GetWindow().GetSize().y;
	fWidth_2 = GetWindow().GetSizeFloat().x / 2;
	fHeight_2 = GetWindow().GetSizeFloat().y / 2;
	m_pRender->Reset(
		GetWindow()
	);
	GetWindow().SetFullScreen(psDeviceFlags.is(rsFullscreen));

    if (g_pGamePersistent)
    {
		
        //. g_pGamePersistent->Environment().OnDeviceCreate();
        //bNeed_re_create_env = TRUE;
		if(gameVersionController->getGame() != gameVersionController->SOC)
        ENV.bNeed_re_create_env = TRUE;
		else     ENV_SOC.bNeed_re_create_env = TRUE;
    }


    _SetupStates();
    if (precache)
        PreCache(20, true, false);
    u32 tm_end = TimerAsync();
    Msg("*** RESET [%d ms]", tm_end - tm_start);

    // TODO: Remove this! It may hide crash
//    Memory.mem_compact();

#ifndef DEDICATED_SERVER
  //  ShowCursor(FALSE);
#endif

    seqDeviceReset.Process(rp_DeviceReset);

    if (dwWidth_before != dwWidth || dwHeight_before != dwHeight)
    {
        seqResolutionChanged.Process(rp_ScreenResolutionChanged);
    }
}
