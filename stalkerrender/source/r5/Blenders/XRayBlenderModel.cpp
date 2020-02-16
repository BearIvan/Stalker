#include "pch.h"

XRayBlenderModel::XRayBlenderModel()
{
	Description.ID = B_MODEL;
	Description.Version = 2;
	oTessellation.Count = 4;
	oTessellation.IDselected = 0;
	oAREF.value = 32;
	oAREF.min = 0;
	oAREF.max = 255;
	oBlend.value = FALSE;
}

XRayBlenderModel::~XRayBlenderModel()
{
}

LPCSTR XRayBlenderModel::getComment()
{
	return LPCSTR();
}

void XRayBlenderModel::Save(IWriter & fs)
{
}

void XRayBlenderModel::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	switch (version)
	{
	case 0:
		oAREF.value = 32;
		oAREF.min = 0;
		oAREF.max = 255;
		oBlend.value = FALSE;
		break;
	case 1:
	default:
		xrPREAD_PROP(fs, xrPID_BOOL, oBlend);
		xrPREAD_PROP(fs, xrPID_INTEGER, oAREF);
		break;
	}
	if (version > 1)
	{
		xrPREAD_PROP(fs, xrPID_TOKEN, oTessellation);
	}
}

void XRayBlenderModel::Initialize()
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
		PipelineDescription.BlendState.RenderTarget[0].Enable = true;
		PipelineDescription.BlendState.RenderTarget[0].ColorSrc = BF_SRC_ALPHA;
		PipelineDescription.BlendState.RenderTarget[0].ColorDst = BF_INV_SRC_ALPHA;
	}
	CreatePipeline(1, PipelineDescription, "default", "default", SVD_M);
	CreatePipeline(1, PipelineDescription, "default", "default", SVD_0W);
	CreatePipeline(0, PipelineDescription, "default", "default", SVD_1W);
	CreatePipeline(0, PipelineDescription, "default", "default", SVD_2W);
}
void XRayBlenderModel::Compile(XRayShaderElement& shader)
{
	if (IDShader == 0|| IDShader == 1)
	{
		SetTexture(shader, 0, "$base0");
		shader.SamplerStates[0] = SSS_Default;
		shader.TypeTransformation = STT_Matrix;

	}
}

