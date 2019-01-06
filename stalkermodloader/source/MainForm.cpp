#include "stdafx.h"

MainForm::MainForm():BearUI(400,100),Ok(-1)
{
	btOk.Font = GetFont(F_RUS, 16);
	btOk.Text = TEXT("Запустить");
	btOk.Rect.set(210, 60, 180, 30);
	btOk.SetCallback(this, &MainForm::cbBtOk);

	btCancel.Font = GetFont(F_RUS, 16);
	btCancel.Text = TEXT("Отмена");
	btCancel.Rect.set(10, 60, 180, 30);
	btCancel.SetCallback(this, &MainForm::cbBtCancel);

	cbGames.Rect.set(10, 20, 380, 20 * 4);
	cbGames.Font = GetFont(F_RUS, 16);
	cbGames.Items.push_back(TEXT("S.T.A.L.K.E.R. SOC(1.0.04)"));
	cbGames.Items.push_back(TEXT("S.T.A.L.K.E.R. SOC(1.0.06)"));
	cbGames.Items.push_back(TEXT("S.T.A.L.K.E.R. CS"));
	cbGames.Items.push_back(TEXT("S.T.A.L.K.E.R. COP"));
	cbGames.SelectItem = 0;
	cbGames.SetCallback(this, &MainForm::cbCbGames);
	path = GameVersionController::Path::SOC_1004;

	PushItem(&cbGames);
	PushItem(&btOk);
	PushItem(&btCancel);
}

MainForm::~MainForm()
{
}

void MainForm::cbBtOk()
{
	Ok = true;
}

void MainForm::cbBtCancel()
{
	Ok = false;
}

void MainForm::cbCbGames()
{
	switch (cbGames.SelectItem)
	{
	case 0:
		path = GameVersionController::Path::SOC_1004;
		break;
	case 1:
		path = GameVersionController::Path::SOC_1007;
		break;
	case 2:
		path = GameVersionController::Path::CS_1510;
		break;
	case 3:
		path = GameVersionController::Path::COP_1602;
		break;
	default:
		break;
	}
}
