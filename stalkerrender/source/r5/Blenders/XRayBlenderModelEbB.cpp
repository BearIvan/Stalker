#include "pch.h"

XRayBlenderModelEbB::XRayBlenderModelEbB()
{
	Description.ID = B_MODEL_EbB;
	Description.Version = 0x1;
	BearString::Copy(oT2_Name, TEXT("$null"));
	BearString::Copy(oT2_xform, TEXT("$null"));
	oBlend.value = FALSE;
}

XRayBlenderModelEbB::~XRayBlenderModelEbB()
{
}

LPCSTR XRayBlenderModelEbB::getComment()
{
	return LPCSTR();
}

void XRayBlenderModelEbB::Save(IWriter & fs)
{
}

void XRayBlenderModelEbB::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	xrPREAD_MARKER(fs);
	xrPREAD_PROP(fs, xrPID_TEXTURE, oT2_Name);
	xrPREAD_PROP(fs, xrPID_MATRIX, oT2_xform);
	if (version >= 0x1) {
		xrPREAD_PROP(fs, xrPID_BOOL, oBlend);
	}
} 

void XRayBlenderModelEbB::Initialize()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.Samplers[0].Shader = ST_Pixel;
	RootSignatureDescription.SRVResources[0].Shader = ST_Pixel;
	RootSignatureDescription.UniformBuffers[0].Shader = ST_Vertex;
	RootSignature[1] = BearRenderInterface::CreateRootSignature(RootSignatureDescription);
	RootSignatureDescription.UniformBuffers[1].Shader = ST_Vertex;
	RootSignature[0] = BearRenderInterface::CreateRootSignature(RootSignatureDescription);
	BearPipelineDescription PipelineDescription;
	PipelineDescription.DepthStencilState.DepthEnable = true;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;
	if (oBlend.value)
	{
		PipelineDescription.DepthStencilState.EnableDepthWrite = false;
		PipelineDescription.BlendState.RenderTarget[0].Enable = true;
		PipelineDescription.BlendState.RenderTarget[0].ColorSrc = BF_SRC_ALPHA;
		PipelineDescription.BlendState.RenderTarget[0].ColorDst = BF_INV_SRC_ALPHA;
	}
	CreatePipeline(1, PipelineDescription, "default", "default", SVD_M);
	CreatePipeline(1, PipelineDescription, "default", "default", SVD_0W);
	CreatePipeline(0, PipelineDescription, "default", "default", SVD_1W);
	CreatePipeline(0, PipelineDescription, "default", "default", SVD_2W);
}

void XRayBlenderModelEbB::Compile(XRayShaderElement& shader)
{
	if (IDShader == 0|| IDShader == 1)
	{
		SetTexture(shader, 0, "$base0");
		shader.SamplerStates[0] = SSS_Default;
		shader.TypeTransformation = STT_Matrix;

	}
}

