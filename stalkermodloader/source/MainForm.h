#pragma once 
class MainForm:public BearUI::BearUI
{
public:
	MainForm();
	~MainForm();
	::BearUI::Classic::BearUIButton btOk;
	::BearUI::Classic::BearUIButton btMods;
	::BearUI::Classic::BearUIComboBox cbGames;
	::BearUI::Classic::BearUIListBox lbMods;

	ButtonLink btVK;
	ButtonLink btTwitter;
	ButtonLink btFacebook;
	::BearUI::BearUIText lbSalf;

	::BearUI::BearUIText lbHelp;
	ButtonLink btYandexM;

	::BearUI::Classic::BearUIButton cbHelp;
	bint Ok;
	GameVersionController::Path path;
private:
	void UpdateMods();
	void cbBtOk();
	void cbBtHelp();
	void cbCbGames();
	void cbBtMods();
};