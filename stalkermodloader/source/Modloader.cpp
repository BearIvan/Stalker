#include "stdafx.h"
#include "api/XrGameVersionController.h"
BearCore::BearThread*SplashTread = 0;
extern int32 SplashStatus;
extern BearCore::BearMutex SplashMutex;
extern int32 UpdateThread();
bool Modloader::Run()
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
		while (viewport.Update()&& mainform.Ok==-1)
		{

			BearUI::BearEventViewport ev;
			while (viewport.GetEvent(ev))
			{
				mainform.UseEventViewport(ev);
			}
			viewport.ClearColor(BearCore::BearColor::BearColor(uint8(30),30,30));
			mainform.Update(0);
			mainform.Draw(0);
			viewport.Swap();
		}
		if (mainform.Ok == 1)
		{
			gameVersionController = BearCore::bear_new<GameVersionController>(mainform.path);
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
		SplashTread = BearCore::bear_new<BearCore::BearThread>(UpdateThread);
		SplashTread->Join(TEXT("Splash"));
	}
	return ok>=1;
}

void  Modloader::Destroy()
{
	if (SplashTread)
	{
		{
			BearCore::BearMutexLock lock(SplashMutex);
			SplashStatus = 0;

		}
		SplashTread->Wait();
		BearCore::bear_delete(SplashTread);
	}
	BearGraphics::BearRenderInterface::Destroy();
}
BearCore::BearString1024 GNameMod = TEXT("");
const bchar * Modloader::GetNameMod()
{
	return GNameMod;
}
