#include "pch.h"

XRayBlenderDefault::XRayBlenderDefault()
{
	Description.ID = B_DEFAULT;
	Description.Version = 1;
	oTessellation.Count = 4;
	oTessellation.IDselected = 0;
}

XRayBlenderDefault::~XRayBlenderDefault()
{
}

LPCSTR XRayBlenderDefault::getComment()
{
	return LPCSTR();
}

void XRayBlenderDefault::Save(IWriter & fs)
{
}

void XRayBlenderDefault::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	if (version > 0)
	{
		xrPREAD_PROP(fs, xrPID_TOKEN, oTessellation);
	}
}

void XRayBlenderDefault::Initialize()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.Samplers[0].Shader = ST_Pixel;
	RootSignatureDescription.SRVResources[0].Shader = ST_Pixel;
	RootSignatureDescription.UniformBuffers[0].Shader = ST_Vertex;
	RootSignature[0] = BearRenderInterface::CreateRootSignature(RootSignatureDescription);

	BearPipelineDescription PipelineDescription;
	PipelineDescription.DepthStencilState.DepthEnable = true;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;
	CreatePipeline(0, PipelineDescription, "default", "default_tl", SVD_R1Vert);

	CreatePipeline(0, PipelineDescription, "default", "default_tl", SVD_R1LMap);
}

void XRayBlenderDefault::Compile(XRayShaderElement& shader)
{
	if (IDShader == 0)
	{
		SetTexture(shader, 0, "$base0");
		shader.SamplerStates[0] = SSS_Default;
		shader.TypeTransformation = STT_Matrix;

	}
}
