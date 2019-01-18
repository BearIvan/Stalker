#include "stdafx.h"

MainForm::MainForm() :BearUI(1000, 600), Ok(-1)
{
	btOk.Font = GetFont(F_RUS, 16);
	btOk.Text = TEXT("Запустить");
	btOk.Rect.set(810, 550, 180, 30);
	btOk.SetCallback(this, &MainForm::cbBtOk);

	btCancel.Font = GetFont(F_RUS, 16);
	btCancel.Text = TEXT("Отмена");
	btCancel.Rect.set(610, 550, 180, 30);
	btCancel.SetCallback(this, &MainForm::cbBtCancel);

	cbGames.Rect.set(610, 20, 380, 21 * 5);
	cbGames.Font = GetFont(F_RUS, 16);
	cbGames.Items.push_back(TEXT("S.T.A.L.K.E.R. SOC(1.0.04)"));
	cbGames.Items.push_back(TEXT("S.T.A.L.K.E.R. SOC(1.0.06)"));
	cbGames.Items.push_back(TEXT("S.T.A.L.K.E.R. CS"));
	cbGames.Items.push_back(TEXT("S.T.A.L.K.E.R. COP"));
	cbGames.SelectItem = 0;
	cbGames.SetCallback(this, &MainForm::cbCbGames);
	path = GameVersionController::Path::SOC_1004;

	lbMods.Font = GetFont(F_RUS, 16);
	lbMods.Rect.set(20, 20, 560, 560);


	PushItem(&cbGames);
	PushItem(&lbMods);
	PushItem(&btOk);
	PushItem(&btCancel);

	UpdateMods();
}

MainForm::~MainForm()
{
}

void MainForm::UpdateMods()
{
	lbMods.Items.clear_not_free();
	lbMods.Items.push_back(TEXT("Оригинал"));
	lbMods.SelectItem = 0;
	BearCore::BearVector<BearCore::BearString> mods;
	switch (path)
	{
	case GameVersionController::Path::SOC_1004:
		FS.GetDirectories(mods, TEXT("%mods_soc14%"));
		break;
	case GameVersionController::Path::SOC_1007:
		FS.GetDirectories(mods, TEXT("%mods_soc16%"));
		break;
	case GameVersionController::Path::CS_1510:
		FS.GetDirectories(mods, TEXT("%mods_cs%"));
		break;
	case GameVersionController::Path::COP_1602:
		FS.GetDirectories(mods, TEXT("%mods_cop%"));
		break;
	}
	lbMods.Items.insert(lbMods.Items.end(), mods.begin(), mods.end());
	lbMods.Reset();
}

void MainForm::cbBtOk()
{
	if (lbMods.SelectItem > 0)
	{
		BearCore::BearStringPath path_mods;
		BearCore::BearString::Copy(path_mods, *lbMods.Items[lbMods.SelectItem]);
		BearCore::BearString::Contact(path_mods,BEAR_PATH);
		BearCore::BearString::Contact(path_mods, TEXT("gamedata"));
		const bchar*PathToMods = TEXT("%mods_soc14%");
		switch (path)
		{
		case GameVersionController::SOC_1007:
			PathToMods = TEXT("%mods_soc16%");
			break;
		case GameVersionController::CS_1510:
			PathToMods = TEXT("%mods_cs%");
			break;
		case GameVersionController::COP_1602:
			PathToMods = TEXT("%mods_cop%");
			break;
		default:
			break;
		}
		if (FS.ExistDirectory(PathToMods, path_mods))
		{
			FS.AppendPath(TEXT("%content%"), path_mods, PathToMods, 1000);
		}
		else
		{
			FS.AppendPath(TEXT("%content%"), *lbMods.Items[lbMods.SelectItem], PathToMods, 1000);
		}
		FS.AppendPath(TEXT("%user%"), *lbMods.Items[lbMods.SelectItem], TEXT("%guser%"), 0);
	}
	else
	{
		switch (path)
		{
		case GameVersionController::SOC_1004:
			FS.AppendPath(TEXT("%user%"), TEXT("original" BEAR_PATH "soc14"), TEXT("%guser%"), 0);
			break;
		case GameVersionController::SOC_1007:
			FS.AppendPath(TEXT("%user%"), TEXT("original" BEAR_PATH "soc16"), TEXT("%guser%"), 0);
			break;
		case GameVersionController::CS_1510:
			FS.AppendPath(TEXT("%user%"), TEXT("original" BEAR_PATH "cs"), TEXT("%guser%"), 0);
			break;
		case GameVersionController::COP_1602:
			FS.AppendPath(TEXT("%user%"), TEXT("original" BEAR_PATH "cop"), TEXT("%guser%"), 0);
			break;
		}
		
	}
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
		if (path == GameVersionController::Path::SOC_1004)return;
		path = GameVersionController::Path::SOC_1004;
		break;
	case 1:
		if (path == GameVersionController::Path::SOC_1007)return;
		path = GameVersionController::Path::SOC_1007;
		break;
	case 2:
		if (path == GameVersionController::Path::CS_1510)return;
		path = GameVersionController::Path::CS_1510;
		break;
	case 3:
		if (path == GameVersionController::Path::COP_1602)return;
		path = GameVersionController::Path::COP_1602;
		break;
	default:
		break;
	}
	UpdateMods();
}
