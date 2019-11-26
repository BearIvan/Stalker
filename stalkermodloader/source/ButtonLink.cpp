#include "stdafx.h"
#ifdef MODLOADER_UI
ButtonLink::ButtonLink(const bchar*link, const bchar*texture):Link(link)
{
	PushItemInBegin(&UIImage);
	SetCallback(this,&ButtonLink::CallBack);
	BearGraphics::BearImage img;
	BEAR_ASSERT( img.LoadDDSFromStream(**FS.Read(TEXT("%textures%"), texture, TEXT(".dds"))));
	UIImage.Texture.Clear();
	UIImage.Texture.Create(img);
	UIImage.Rect.x1 =static_cast<float>( img.GetSize().x);
	UIImage.Rect.y1 = static_cast<float>(img.GetSize().y);
}

ButtonLink::~ButtonLink()
{
}



void ButtonLink::Draw(BearUI::BearUI * ui, BearCore::BearTime  time)
{
	__super::Draw(ui,time);
}

void ButtonLink::OnMessage(int32 message)
{
	__super::OnMessage(message);
}

bool ButtonLink::OnMouse(float x, float y)
{
	return 	__super::OnMouse(x, y);
}

bool ButtonLink::OnKeyDown(BearUI::BearInput::Key key)
{
	return __super::OnKeyDown(key);;
}

bool ButtonLink::OnKeyUp(BearUI::BearInput::Key key)
{
	return __super::OnKeyUp(key);;
}

void ButtonLink::Reset()
{
	UIImage.Rect.x = Rect.x+(Rect.x1 - UIImage.Rect.x1) / 2;
	UIImage.Rect.y = Rect.y + (Rect.y1 - UIImage.Rect.y1) / 2;
	__super::Reset();;
}

void ButtonLink::Unload()
{
	__super::Unload();;
}

void ButtonLink::Reload()
{
	__super::Reload();;
}

void ButtonLink::KillFocus()
{
	__super::KillFocus();;
}

void ButtonLink::CallBack()
{
	ShellExecute (NULL, TEXT("Open"), *Link,0,0, SW_SHOWNORMAL);
}
#endif