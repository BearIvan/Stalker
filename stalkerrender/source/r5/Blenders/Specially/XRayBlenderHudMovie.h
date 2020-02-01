#pragma once
class  XRayBlenderHudMovie :public XRayBlenderCompiler
{
public:
	XRayBlenderHudMovie();
	virtual ~XRayBlenderHudMovie();
	virtual void Initialize();
	virtual void Compile(XRayShaderElement& shader);
private:
	BearFactoryPointer<BearRHI::BearRHIPipeline> Pipeline;
	BearFactoryPointer<BearRHI::BearRHIRootSignature> RootSignature;
};