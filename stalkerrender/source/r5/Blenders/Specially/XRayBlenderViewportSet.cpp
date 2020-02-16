#include "pch.h"
#include "XRayBlenderViewportSet.h"

XRayBlenderViewportSet::XRayBlenderViewportSet()
{
}

XRayBlenderViewportSet::~XRayBlenderViewportSet()
{
}

void XRayBlenderViewportSet::Initialize()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.Samplers[0].Shader = ST_Pixel;
	RootSignatureDescription.SRVResources[0].Shader = ST_Pixel;
	RootSignatureDescription.UniformBuffers[0].Shader = ST_Vertex;
	RootSignature[0] = BearRenderInterface::CreateRootSignature(RootSignatureDescription);

	BearPipelineDescription PipelineDescription;
	PipelineDescription.RenderPass = HW->Viewport_RenderPass;
	CreatePipeline(0,PipelineDescription, "notransform", "default_tl", SVD_TL);
}

void XRayBlenderViewportSet::Compile(XRayShaderElement& shader)
{
	if (IDShader == 0)
	{
		SetTexture(shader, 0, "$base0");
		shader.SamplerStates[0] = SSS_Default;
		shader.TypeTransformation = STT_Screen;
	}
}
