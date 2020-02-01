#include "pch.h"
XRayHardware* HW = nullptr;
extern const bchar* GGraphicsAPI[];
bsize GCurrentApi =0 ;

XRayHardware::XRayHardware()
{
	BEAR_ASSERT(BearRenderInterface::Initialize(GGraphicsAPI[GCurrentApi]));
	Context = BearRenderInterface::CreateContext();
}

XRayHardware::~XRayHardware()
{
	Viewport.clear();
	Context.clear();
	BearRenderInterface::Destroy();
}

void XRayHardware::Update(BearWindow& viewport)
{
	if (Viewport.empty())
	{
		BearViewportDescription Description;
		Description.Clear = true;
		Description.ClearColor = BearColor::Black;
		Viewport = BearRenderInterface::CreateViewport(viewport, Description);
	}
	else
	{
		Viewport->Resize(viewport.GetSize().x, viewport.GetSize().y);
		Viewport->SetFullScreen(viewport.IsFullScreen());
	}
}
