#pragma once 
class SplashForm :public BearUI::BearUI
{
public:
	SplashForm(BearGraphics::BearTexture2DRef&texture);
	~SplashForm();
	::BearUI::BearUITexture texture;

};