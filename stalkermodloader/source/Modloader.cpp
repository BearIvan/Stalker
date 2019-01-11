#include "stdafx.h"
#include "api/XrGameVersionController.h"
bool Modloader::Run()
{
	BearGraphics::BearRenderInterface::Initialize(TEXT("bear_directx11"));
	bint ok = 0;
	{
		BearCore::BearLog::Printf(TEXT("%p"), sizeof(BearCore::BearString));
		
		BearUI::BearViewport viewport(400, 100);
		MainForm mainform;
		

		BearGraphics::BearRenderInterface::AttachRenderTargetView(0, viewport);
		BearGraphics::BearRenderInterface::SetViewport(0, 0, 400, 100);

		while (viewport.Update()&& mainform.Ok==-1)
		{

			BearUI::BearEventViewport ev;
			while (viewport.GetEvent(ev))
			{
				switch (ev.Type)
				{
				case BearUI::EVT_Active:
					mainform.KillFocus();
					BearCore::BearLog::Printf(TEXT("Active"));
					break;
				case BearUI::EVT_Deactive:
					mainform.KillFocus();
					BearCore::BearLog::Printf(TEXT("Deactive"));
					break;
				case  BearUI::EVT_Char:
					BearCore::BearLog::Printf(TEXT("Char:%c"), ev.Char);
					break;
				case BearUI::EVT_MouseEnter:
				case BearUI::EVT_MouseLevae:
					mainform.KillFocus();
					break;
				case BearUI::EVT_MouseMove:
					mainform.OnMouse(ev.Position.x, ev.Position.y);
				case BearUI::EVT_KeyDown:
					mainform.OnKeyDown(ev.Key);
					break;
				case BearUI::EVT_KeyUp:
					mainform.OnKeyUp(ev.Key);
					break;
				default:

					break;
				}
			}
			viewport.ClearColor(BearCore::BearColor::Black);
			mainform.Update(0);
			mainform.Draw(0);
			viewport.Swap();
		}
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
		ok = mainform.Ok;
	}
	BearGraphics::BearRenderInterface::Destroy();
	return ok;
}