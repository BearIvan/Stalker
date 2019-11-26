#include "stdafx.h"
#ifdef MODLOADER_UI
SplashForm::SplashForm(BearGraphics::BearTexture2DRef&texture_) :BearUI(static_cast<bsize>(texture_.GetSize().x),static_cast<bsize>( texture_.GetSize().y))
{
	texture.Texture.Clear();
	texture.Texture = texture_;
	texture.Rect.x1 = texture_.GetSize().x;
	texture.Rect.y1= texture_.GetSize().y;
	PushItem(&texture);
}

SplashForm::~SplashForm()
{
}
int32 SplashStatus=0;
BearCore::BearMutex SplashMutex;
int32 UpdateThread()
{
	SplashMutex.Lock();
	BearGraphics::BearTexture2DRef texture;
	BearGraphics::BearImage image;
	
	SplashStatus = image.LoadFromStream(**FS.Read(TEXT("%content%"), TEXT("splash.bmp"))) ? 1 : -1;
	texture.Create(image);
	BearUI::BearViewport viewport(image.GetSize().x, image.GetSize().y, false, BearUI::BearViewport::TW_POPUP | BearUI::BearViewport::TW_WIHTOUT_CLOSED);
	SplashForm splash(texture);
	BearGraphics::BearRenderInterface::AttachRenderTargetView( viewport);
	BearGraphics::BearRenderInterface::SetViewport(0, 0,static_cast<float>( image.GetSize().x),static_cast<float>( image.GetSize().y));
	if (SplashStatus < 0)return 0;
	SplashMutex.Unlock();
	while (true)
	{
		viewport.Update();
		if (SplashStatus == 0)break;
		BearCore::BearMutexLock lock(SplashMutex);
		viewport.ClearColor(BearCore::BearColor::Brown);
		splash.Update(0);
		splash.Draw(0);
		viewport.Swap();
	}
	return 0;
}
#endif