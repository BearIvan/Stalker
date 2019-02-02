#pragma once 
class MainForm:public BearUI::BearUI
{
public:
	MainForm();
	~MainForm();
	::BearUI::BearUIButton btOk;
	::BearUI::BearUIButton btMods;
	::BearUI::BearUIComboBox cbGames;
	::BearUI::BearUIListBox lbMods;

	ButtonLink btVK;
	ButtonLink btTwitter;
	ButtonLink btFacebook;
	::BearUI::BearUIText lbSalf;

	::BearUI::BearUIText lbHelp;
	ButtonLink btYandexM;

	::BearUI::BearUIButton cbHelp;
	bint Ok;
	GameVersionController::Path path;
private:
	void UpdateMods();
	void cbBtOk();
	void cbBtHelp();
	void cbCbGames();
	void cbBtMods();
};