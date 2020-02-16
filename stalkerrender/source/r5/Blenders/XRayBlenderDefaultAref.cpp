#include "pch.h"

XRayBlenderDefaultAref::XRayBlenderDefaultAref()
{
	Description.ID = B_DEFAULT_AREF;
	Description.Version = 1;
	oAREF.value = 32;
	oAREF.min = 0;
	oAREF.max = 255;
	oBlend.value = FALSE;
}

XRayBlenderDefaultAref::~XRayBlenderDefaultAref()
{
}

LPCSTR XRayBlenderDefaultAref::getComment()
{
	return LPCSTR();
}

void XRayBlenderDefaultAref::Save(IWriter & fs)
{
}

void XRayBlenderDefaultAref::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);

	switch (version)
	{
	case 0:
		xrPREAD_PROP(fs, xrPID_INTEGER, oAREF);
		oBlend.value = FALSE;
		break;
	case 1:
	default:
		xrPREAD_PROP(fs, xrPID_INTEGER, oAREF);
		xrPREAD_PROP(fs, xrPID_BOOL, oBlend);
		break;
	}
}

void XRayBlenderDefaultAref::Initialize()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.Samplers[0].Shader = ST_Pixel;
	RootSignatureDescription.SRVResources[0].Shader = ST_Pixel;
	RootSignatureDescription.UniformBuffers[0].Shader = ST_Vertex;
	RootSignature[0] = BearRenderInterface::CreateRootSignature(RootSignatureDescription);

	BearPipelineDescription PipelineDescription;
	PipelineDescription.DepthStencilState.DepthEnable = true;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;
	if (oBlend.value)
	{
		PipelineDescription.BlendState.RenderTarget[0].Enable = true;
		PipelineDescription.BlendState.RenderTarget[0].ColorSrc = BF_SRC_ALPHA;
		PipelineDescription.BlendState.RenderTarget[0].ColorDst = BF_INV_SRC_ALPHA;
	}
	CreatePipeline(0, PipelineDescription, "default", "default_tl", SVD_R1Vert);

	CreatePipeline(0, PipelineDescription, "default", "default_tl", SVD_R1LMap);
}

void XRayBlenderDefaultAref::Compile(XRayShaderElement& shader)
{
	if (IDShader == 0)
	{
		SetTexture(shader, 0, "$base0");
		shader.SamplerStates[0] = SSS_Default;
		shader.TypeTransformation = STT_Matrix;

	}
}
