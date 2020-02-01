#pragma once
enum ShaderTypeTransformation
{
	STT_Screen,
	STT_Matrix,
};
class  XRayShaderElement
{
public:

	XRayShaderElement() { bear_fill(Textures); }
	~XRayShaderElement();
	void Clear();
	void Copy(const XRayShaderElement& right);
	void Swap(XRayShaderElement& right);

	XRayShaderElement(const XRayShaderElement& right)	{Copy(right);}
	XRayShaderElement( XRayShaderElement&& right){	bear_fill(Textures);	Swap(right);}
	XRayShaderElement& operator=(const XRayShaderElement& right) { Copy(right); return*this; }
	XRayShaderElement& operator=( XRayShaderElement&& right) { Swap(right); return*this; }

	BearFactoryPointer<BearRHI::BearRHIPipeline> Pipeline;
	BearFactoryPointer<BearRHI::BearRHIDescriptorHeap> DescriptorHeap;
	ShaderTypeTransformation TypeTransformation;
	XRayTexture* Textures[16];

	inline bool Set(BearFactoryPointer<BearRHI::BearRHIContext>& context)
	{
		if (Pipeline.empty())return false;
		context->SetPipeline(Pipeline);
		context->SetDescriptorHeap(DescriptorHeap);
		return true;
	}
};
struct XRayBlender
{
	XRayShaderElement E[6];
	inline void Clear()
	{
		for (bsize i = 0; i < 6; i++)E[i].Clear();
	}
};