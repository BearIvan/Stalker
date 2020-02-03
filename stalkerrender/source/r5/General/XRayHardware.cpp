#include "pch.h"
XRayHardware* HW = nullptr;
extern const bchar* GGraphicsAPI[];
bsize GCurrentApi =3 ;

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

		BearRenderPassDescription  RenderPassDescription;
		RenderPassDescription.DepthStencil.Clear = true;
		RenderPassDescription.DepthStencil.Depth = 1.f;
		RenderPassDescription.DepthStencil.Format = DSF_DEPTH32F;
		RenderPassDescription.RenderTargets[0].Clear = true;
		RenderPassDescription.RenderTargets[0].Color = BearColor::Black;
		RenderPassDescription.RenderTargets[0].Format = Viewport->GetFormat();
		RenderPass = BearRenderInterface::CreateRenderPass(RenderPassDescription);
	}
	else
	{
		Viewport->Resize(viewport.GetSize().x, viewport.GetSize().y);
		Viewport->SetFullScreen(viewport.IsFullScreen());
	}
	TBasicColor = BearRenderInterface::CreateTexture2D(viewport.GetSize().x, viewport.GetSize().y, Viewport->GetFormat());
	TDepthStencil = BearRenderInterface::CreateTexture2D(viewport.GetSize().x, viewport.GetSize().y,DSF_DEPTH32F);
	BearFrameBufferDescription FrameBufferDescription;
	FrameBufferDescription.RenderPass = RenderPass;
	FrameBufferDescription.RenderTargets[0] = TBasicColor;
	FrameBufferDescription.DepthStencil = TDepthStencil;
	FrameBuffer = BearRenderInterface::CreateFrameBuffer(FrameBufferDescription);

}
