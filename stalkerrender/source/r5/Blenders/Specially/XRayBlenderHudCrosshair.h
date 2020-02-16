#pragma once
class  XRayBlenderHudCrosshair :public XRayBlenderCompiler
{
public:
	XRayBlenderHudCrosshair();
	virtual ~XRayBlenderHudCrosshair();
	virtual void Initialize();
	virtual void Compile(XRayShaderElement& shader);
private:
};