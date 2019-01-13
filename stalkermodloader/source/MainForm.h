#pragma once 
class MainForm:public BearUI::BearUI
{
public:
	MainForm();
	~MainForm();
	::BearUI::BearUIButton btOk, btCancel;
	::BearUI::BearUIComboBox cbGames;
	::BearUI::BearUIListBox lbMods;
	bint Ok;
	GameVersionController::Path path;
private:
	void UpdateMods();
	void cbBtOk();
	void cbBtCancel();
	void cbCbGames();

};