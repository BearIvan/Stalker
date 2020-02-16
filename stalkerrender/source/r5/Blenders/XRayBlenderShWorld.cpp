#include "pch.h"

XRayBlenderShWorld::XRayBlenderShWorld()
{
	Description.ID = B_SHADOW_WORLD;
}

XRayBlenderShWorld::~XRayBlenderShWorld()
{
}

LPCSTR XRayBlenderShWorld::getComment()
{
	return LPCSTR();
}

void XRayBlenderShWorld::Save(IWriter & fs)
{
}

void XRayBlenderShWorld::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
}

void XRayBlenderShWorld::Initialize()
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
}

void XRayBlenderShWorld::Compile(XRayShaderElement& shader)
{
	if (IDShader == 0)
	{
		SetTexture(shader, 0, "$base0");
		shader.SamplerStates[0] = SSS_Default;
		shader.TypeTransformation = STT_Matrix;

	}
}

