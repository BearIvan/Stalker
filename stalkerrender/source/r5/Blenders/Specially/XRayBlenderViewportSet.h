#pragma once
class  XRayBlenderViewportSet :public XRayBlenderCompiler
{
public:
	XRayBlenderViewportSet();
	virtual ~XRayBlenderViewportSet();
	virtual void Initialize();
	virtual void Compile(XRayShaderElement& shader);
private:
};