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
	BearVector< XRayFontRender*> FontRenders;
};
extern XRayHardware* HW;