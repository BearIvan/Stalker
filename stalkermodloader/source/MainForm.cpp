#include "stdafx.h"

MainForm::MainForm() :BearUI(1000, 600), Ok(-1), btVK(TEXT("https://vk.com/isoboleskiy"),TEXT("ui") BEAR_PATH TEXT("vk")), btTwitter(TEXT("https://twitter.com/isobolevskiy"), TEXT("ui") BEAR_PATH TEXT("twitter")),btFacebook(TEXT("https://www.facebook.com/profile.php?id=100013416918583"), TEXT("ui") BEAR_PATH TEXT("facebook")),btYandexM(TEXT("https://money.yandex.ru/to/410012333877314"), TEXT("ui") BEAR_PATH TEXT("yandexm"))
{
	btOk.Font = GetFont(F_RUS, 16);
	btOk.Text = TEXT("Играть");
	btOk.Rect.set(810, 60, 180, 30);
	btOk.SetCallback(this, &MainForm::cbBtOk);

	btMods.Font = GetFont(F_RUS, 15);
	btMods.Text = TEXT("Открыть папку с модами");
	btMods.Rect.set(610, 60, 180, 30);
	btMods.SetCallback(this, &MainForm::cbBtMods);


	lbHelp.Font = GetFont(F_RUS, 16);
	lbHelp.Text = TEXT("Будем рады за поддержку нашего проекта:");
	lbHelp.Rect.set(610, 550, 180, 32);
	lbHelp.Style = lbSalf.ST_CenterOfHeight;
	btYandexM.Rect.set(610+16+ lbHelp.CalcWidth(), 550, 32, 32);


	lbSalf.Text = TEXT("Связь со мной:");
	lbSalf.Rect.set(610, 470, 32,32);
	lbSalf.Font= GetFont(F_RUS, 16);
	lbSalf.Style=lbSalf.ST_CenterOfHeight;



	btVK.Rect.set(684, 510, 32, 32);
	btTwitter.Rect.set(610, 510, 32, 32);
	btFacebook.Rect.set(758, 510 , 32, 32);


	cbGames.Rect.set(610, 20, 380, 21 * 5);
	cbGames.Font = GetFont(F_RUS, 16);
	cbGames.GetItems().push_back(TEXT("S.T.A.L.K.E.R.: Тень Чернобыля(1.0004)"));
	cbGames.GetItems().push_back(TEXT("S.T.A.L.K.E.R.: Тень Чернобыля(1.0006)"));
	cbGames.GetItems().push_back(TEXT("S.T.A.L.K.E.R.: Чистое небо"));
	cbGames.GetItems().push_back(TEXT("S.T.A.L.K.E.R.: Зов Припяти"));
	cbGames.SelectItem = 0;
	cbGames.SetCallback(this, &MainForm::cbCbGames);
	path = GameVersionController::Path::SOC_1004;

	lbMods.Font = GetFont(F_RUS, 16);
	lbMods.Rect.set(20, 20, 560, 560);




	PushItem(&cbGames);
	PushItem(&lbMods);
	PushItem(&btOk);
	PushItem(&btMods);
	//PushItem(&lbHelp);
	PushItem(&btVK);
	PushItem(&btTwitter);
	PushItem(&btFacebook);
	PushItem(&lbSalf);
	//PushItem(&btYandexM);


	UpdateMods();
}

MainForm::~MainForm()
{
}

void MainForm::UpdateMods()
{
	lbMods.GetItems().clear_not_free();
	lbMods.GetItems().push_back(TEXT("Оригинал"));
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
	lbMods.GetItems().insert(lbMods.GetItems().end(), mods.begin(), mods.end());
}
extern BearCore::BearString1024 GNameMod;
void MainForm::cbBtOk()
{
	if (lbMods.SelectItem > 0)
	{

		BearCore::BearStringPath path_mods;
		BearCore::BearString::Copy(path_mods, *lbMods.GetItems()[lbMods.SelectItem]);
		BearCore::BearString::Contact(path_mods,BEAR_PATH);
		BearCore::BearString::Contact(path_mods, TEXT("gamedata"));
		const bchar*PathToMods = TEXT("%mods_soc14%");
		switch (path)
		{
		case GameVersionController::SOC_1007:
			BearCore::BearLog::Printf(TEXT("Игра:[%s]"),* cbGames.GetItems()[1]);
			PathToMods = TEXT("%mods_soc16%");
			break;
		case GameVersionController::CS_1510:
			BearCore::BearLog::Printf(TEXT("Игра:[%s]"), *cbGames.GetItems()[2]);
			PathToMods = TEXT("%mods_cs%");
			break;
		case GameVersionController::COP_1602:
			BearCore::BearLog::Printf(TEXT("Игра:[%s]"), *cbGames.GetItems()[3]);
			PathToMods = TEXT("%mods_cop%");
			break;
		default:
			BearCore::BearLog::Printf(TEXT("Игра:[%s]"), *cbGames.GetItems()[0]);
			break;
		}

		BearCore::BearLog::Printf(TEXT("Мод:[%s]"), *lbMods.GetItems()[lbMods.SelectItem]);
		if(lbMods.SelectItem)BearCore::BearString::Copy(GNameMod, *lbMods.GetItems()[lbMods.SelectItem]);
		if (FS.ExistDirectory(PathToMods, path_mods))
		{
			FS.AppendPath(TEXT("%content%"), path_mods, PathToMods, 1000);
		}
		else
		{
			FS.AppendPath(TEXT("%content%"), *lbMods.GetItems()[lbMods.SelectItem], PathToMods, 1000);
		}
		FS.AppendPath(TEXT("%user%"), *lbMods.GetItems()[lbMods.SelectItem], TEXT("%guser%"), 0);
	}
	else
	{
		switch (path)
		{
		case GameVersionController::SOC_1004:
			BearCore::BearLog::Printf(TEXT("Игра:[%s]"), *cbGames.GetItems()[0]);
			FS.AppendPath(TEXT("%user%"), TEXT("original" BEAR_PATH "soc14"), TEXT("%guser%"), 0);
			break;
		case GameVersionController::SOC_1007:
			BearCore::BearLog::Printf(TEXT("Игра:[%s]"), *cbGames.GetItems()[1]);
			FS.AppendPath(TEXT("%user%"), TEXT("original" BEAR_PATH "soc16"), TEXT("%guser%"), 0);
			break;
		case GameVersionController::CS_1510:
			BearCore::BearLog::Printf(TEXT("Игра:[%s]"),* cbGames.GetItems()[2]);
			FS.AppendPath(TEXT("%user%"), TEXT("original" BEAR_PATH "cs"), TEXT("%guser%"), 0);
			break;
		case GameVersionController::COP_1602:
			BearCore::BearLog::Printf(TEXT("Игра:[%s]"), *cbGames.GetItems()[3]);
			FS.AppendPath(TEXT("%user%"), TEXT("original" BEAR_PATH "cop"), TEXT("%guser%"), 0);
			break;
		}
		
	}
	Ok = true;
}

void MainForm::cbBtHelp()
{
	ShellExecute(NULL, TEXT("Open"), TEXT("https://money.yandex.ru/to/410012333877314"), 0, 0, SW_SHOWNORMAL);
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

void MainForm::cbBtMods()
{
	BearCore::BearStringPath path_;

	BearCore::BearString str;
	str.append(TEXT(""));
	switch (cbGames.SelectItem)
	{
	case 0:
		FS.UpdatePath(TEXT("%mods_soc14%"), 0, path_);
		str.append(path_);
		break;
	case 1:
		FS.UpdatePath(TEXT("%mods_soc16%"), 0, path_);
		str.append(path_);
		break;
	case 2:
		FS.UpdatePath(TEXT("%mods_cs%"), 0, path_);
		str.append(path_);
		break;
	case 3:
		FS.UpdatePath(TEXT("%mods_cop%"), 0, path_);
		str.append(path_);
		break;
	default:
		break;
	}
	str.append(BEAR_PATH);
	ShellExecute(NULL, NULL, "explorer.exe", *str, NULL, SW_SHOWNORMAL);

}
