#pragma once
class XRayFontRender;
class XRayHardware
{
public:
	XRayHardware();
	~XRayHardware();
	void Update(BearWindow& viewport);
	BearFactoryPointer<BearRHI::BearRHIContext> Context;
	BearFactoryPointer<BearRHI::BearRHIContext> ContextViewport;
	BearVector< XRayFontRender*> FontRenders;
	BearFactoryPointer<BearRHI::BearRHIViewport> Viewport;
	BearFactoryPointer<BearRHI::BearRHIRenderPass> Viewport_RenderPass;
};
extern XRayHardware* HW;