#pragma once
class ButtonLink :public BearUI::Classic::BearUIButton
{
public:
	ButtonLink(const bchar*link,const bchar*textures);
	~ButtonLink();
private:
	virtual void Draw(BearUI::BearUI * ui, BearCore::BearTime time);
	virtual void OnMessage(int32 message);
	virtual bool OnMouse(float x, float y);
	virtual bool OnKeyDown(BearUI::BearInput::Key key);
	virtual bool OnKeyUp(BearUI::BearInput::Key key);
	virtual void Reset();
	virtual void Unload();
	virtual void Reload();
	virtual void KillFocus();
private:
	void CallBack();
	BearCore::BearString Link;
	BearUI::BearUITexture UIImage;
};