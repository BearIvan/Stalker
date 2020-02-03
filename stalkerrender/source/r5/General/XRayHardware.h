#pragma once
class XRayFontRender;
class XRayHardware
{
public:
	XRayHardware();
	~XRayHardware();
	void Update(BearWindow& viewport);
	BearFactoryPointer<BearRHI::BearRHIContext> Context;
	BearFactoryPointer<BearRHI::BearRHIViewport> Viewport;
	BearFactoryPointer<BearRHI::BearRHIRenderPass> RenderPass;
	BearFactoryPointer<BearRHI::BearRHIFrameBuffer> FrameBuffer;
	BearFactoryPointer<BearRHI::BearRHITexture2D> TBasicColor;
	BearFactoryPointer<BearRHI::BearRHITexture2D> TDepthStencil;
	BearVector< XRayFontRender*> FontRenders;
};
extern XRayHardware* HW;