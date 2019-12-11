#include "stdafx.h"
#include "api/XrGameVersionController.h"
BearThread*SplashTread = 0;
extern int32 SplashStatus;
extern BearMutex SplashMutex;
extern int32 UpdateThread();
bool Modloader::Run()
{
	if (strstr(GetCommandLine(), "-game soc14"))
	{
		FS.AppendPath(TEXT("%content%"), TEXT("content") TEXT(BEAR_PATH) TEXT("soc"), TEXT("%main%"), -500);
		FS.AppendPath(TEXT("%user%"), TEXT("original" BEAR_PATH "soc14"), TEXT("%guser%"), 0);
		gameVersionController = bear_new<GameVersionController>(GameVersionController::SOC_1004);
		return true;
	}
	else if (strstr(GetCommandLine(), "-game soc"))
	{
		FS.AppendPath(TEXT("%content%"), TEXT("content") TEXT(BEAR_PATH) TEXT("soc16"), TEXT("%main%"), -499);
		FS.AppendPath(TEXT("%user%"), TEXT("original" BEAR_PATH "soc16"), TEXT("%guser%"), 0);
		gameVersionController = bear_new<GameVersionController>(GameVersionController::SOC_1007);
		return true;
	}
	else if (strstr(GetCommandLine(), "-game cs"))
	{
		FS.AppendPath(TEXT("%content%"), TEXT("content") TEXT(BEAR_PATH) TEXT("cs"), TEXT("%main%"), -500);
		FS.AppendPath(TEXT("%user%"), TEXT("original" BEAR_PATH "cs"), TEXT("%guser%"), 0);
		gameVersionController = bear_new<GameVersionController>(GameVersionController::CS_1510);
		return true;

	}
	else if (strstr(GetCommandLine(), "-game cop"))
	{
		FS.AppendPath(TEXT("%content%"), TEXT("content") TEXT(BEAR_PATH) TEXT("cop"), TEXT("%main%"), -500);
		FS.AppendPath(TEXT("%user%"), TEXT("original" BEAR_PATH "cop"), TEXT("%guser%"), 0);
		gameVersionController = bear_new<GameVersionController>(GameVersionController::COP_1602);
		return true;
	}
#ifdef MODLOADER_UI
	else
	{
		if (!BearGraphics::BearRenderInterface::Initialize(TEXT("bear_directx11")))
		{
			BEAR_ERRORMESSAGE(BearGraphics::BearRenderInterface::Initialize(TEXT("bear_directx10")), TEXT("Yе подерживается минимальная версия directx 10!!"));
		}
		bint ok = 0;
		{

			BearUI::BearViewport viewport(1000, 600);
			MainForm mainform;
			mainform.Reload();

			BearGraphics::BearRenderInterface::AttachRenderTargetView(
				viewport);
			/*BearGraphics::BearRenderInterface::SetViewport(0, 0, 1000, 600);
			BearGraphics::BearDepthStencilStateRef  DepthStencilState;
			{
				BearGraphics::BearDepthStencilStateInitializer Initializer;
				Initializer.DepthEnable = false;
				DepthStencilState = BearGraphics::BearDepthStencilStateRef(Initializer);
			}
			BearGraphics::BearRenderInterface::SetDepthStencilState(DepthStencilState, 0);*/
			while (viewport.Update() && mainform.Ok == -1)
			{

				BearUI::BearEventViewport ev;
				while (viewport.GetEvent(ev))
				{
					mainform.UseEventViewport(ev);
				}
				viewport.ClearColor(BearColor::BearColor(uint8(30), 30, 30));
				mainform.Update(0);
				mainform.Draw(0);
				viewport.Swap();
			}
			if (mainform.Ok == 1)
			{
				gameVersionController = bear_new<GameVersionController>(mainform.path);
				switch (gameVersionController->getPath())
				{
				case	GameVersionController::Path::SOC_1004:
					FS.AppendPath(TEXT("%content%"), TEXT("content") TEXT(BEAR_PATH) TEXT("soc"), TEXT("%main%"), -500);
					break;
				case	GameVersionController::Path::SOC_1007:
					FS.AppendPath(TEXT("%content%"), TEXT("content") TEXT(BEAR_PATH) TEXT("soc"), TEXT("%main%"), -500);
					FS.AppendPath(TEXT("%content%"), TEXT("content") TEXT(BEAR_PATH) TEXT("soc16"), TEXT("%main%"), -499);
					break;
				case	GameVersionController::Path::CS_1510:
					FS.AppendPath(TEXT("%content%"), TEXT("content") TEXT(BEAR_PATH) TEXT("cs"), TEXT("%main%"), -500);
					break;
				case	GameVersionController::Path::COP_1602:
					FS.AppendPath(TEXT("%content%"), TEXT("content") TEXT(BEAR_PATH) TEXT("cop"), TEXT("%main%"), -500);
					break;
				default:
					break;
				}
			}
			ok = mainform.Ok;
		}
		if (ok == 1)
		{
			SplashTread = bear_new<BearThread>(UpdateThread);
			SplashTread->Join(TEXT("Splash"));
		}
		return ok >= 1;
	}
#else
	else 
	{
		FS.AppendPath(TEXT("%content%"), TEXT("content") TEXT(BEAR_PATH) TEXT("cop"), TEXT("%main%"), -500);
		FS.AppendPath(TEXT("%user%"), TEXT("original" BEAR_PATH "cop"), TEXT("%guser%"), 0);
		gameVersionController = bear_new<GameVersionController>(GameVersionController::COP_1602);
		return true;
	}
#endif

}

void  Modloader::Destroy()
{
#ifdef MODLOADER_UI
	if (SplashTread)
	{
		{
			BearMutexLock lock(SplashMutex);
			SplashStatus = 0;

		}
		SplashTread->Wait();
		bear_delete(SplashTread);
	}
	BearGraphics::BearRenderInterface::Destroy();
#endif
}
BearString1024 GNameMod = TEXT("");
const bchar * Modloader::GetNameMod()
{
	return GNameMod;
}
