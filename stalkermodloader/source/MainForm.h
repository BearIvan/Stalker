#pragma once 
class MainForm:public BearUI::BearUI
{
public:
	MainForm();
	~MainForm();
	::BearUI::BearUIButton btOk, btCancel;
	::BearUI::BearUIComboBox cbGames;
	bint Ok;
	GameVersionController::Path path;
private:
	void cbBtOk();
	void cbBtCancel();
	void cbCbGames();

};