#pragma once
enum ShaderTypeTransformation
{
	STT_Screen,
	STT_Matrix,
};
enum ShaderVertexDeclaration
{
	SVD_L,
	SVD_V,
	SVD_LIT,
	SVD_TL0uv,
	SVD_TL,
	SVD_TL2uv,
	SVD_TL4uv,
	SVD_1W,
	SVD_2W,
	SVD_3W,
	SVD_4W,
	SVD_R1LMap,
	SVD_R1Vert,
	SVD_XVert,
	SVD_MUModel,
	SVD_Reserve,
	SVD_Count,
};
enum ShaderSamplerState
{
	SSS_None,
	SSS_Default,
};
class  XRayShaderElement
{
public:
	
	XRayShaderElement() { bear_fill(Textures); for (bsize i = 0; i < 16; i++)SamplerStates[i] = SSS_None; }
	~XRayShaderElement();
	void Clear();
	void Copy(const XRayShaderElement& right);
	void Swap(XRayShaderElement& right);
	
	XRayShaderElement(const XRayShaderElement& right)	{ bear_fill(Textures); Copy(right);}
	XRayShaderElement( XRayShaderElement&& right){  bear_fill(Textures); for (bsize i = 0; i < 16; i++)SamplerStates[i] = SSS_None;;	Swap(right);}
	XRayShaderElement& operator=(const XRayShaderElement& right) { Copy(right); return*this; }
	XRayShaderElement& operator=( XRayShaderElement&& right) { Swap(right); return*this; }

	BearFactoryPointer<BearRHI::BearRHIPipeline> Pipeline[16];
	BearFactoryPointer<BearRHI::BearRHIRootSignature> RootSignature;
	BearFactoryPointer<BearRHI::BearRHIDescriptorHeap> DescriptorHeap;
	ShaderTypeTransformation TypeTransformation;
	XRayTexture* Textures[16];
	ShaderSamplerState SamplerStates[16];
	BearFactoryPointer<BearRHI::BearRHIUniformBuffer> TransformationBuffer;
	void CreateDescriptorHeap();
	inline bool Set(BearFactoryPointer<BearRHI::BearRHIContext>& context,u32 FVF)
	{
		auto fvf = ToVertexDeclaration(FVF);
		//BEAR_ASSERT(!Pipeline[fvf].empty());
		if (Pipeline[fvf].empty())return false;
		context->SetPipeline(Pipeline[fvf]);
		context->SetDescriptorHeap(DescriptorHeap);
		return true;
	}
private:
	
	inline ShaderVertexDeclaration ToVertexDeclaration(u32 FVF)
	{
		switch (FVF)
		{
		case FVF::F_L:
			return SVD_L;
		case FVF::F_V:
			return SVD_V;
		case FVF::F_LIT:
			return SVD_LIT;
		case FVF::F_TL0uv:
			return SVD_TL0uv;
		case FVF::F_TL:
			return SVD_TL;
		case FVF::F_TL2uv:
			return SVD_TL2uv;
		case FVF::F_TL4uv:
			return SVD_TL4uv;
		case FVF::F_1W:
			return SVD_1W;
		case FVF::F_2W:
			return SVD_2W;
		case FVF::F_3W:
			return SVD_3W;
		case FVF::F_4W:
			return SVD_4W;
		case FVF::F_R1LMap:
			return SVD_R1LMap;
		case FVF::F_R1Vert:
			return SVD_R1Vert;
		case FVF::F_XVert:
			return SVD_XVert;
		case FVF::F_MUModel:
			return SVD_MUModel;
		default:
			BEAR_ASSERT(0);
			break;
		};
		return SVD_Reserve;
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