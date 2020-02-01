#include "pch.h"
#include "XRayBlenderHudMovie.h"

XRayBlenderHudMovie::XRayBlenderHudMovie()
{
}

XRayBlenderHudMovie::~XRayBlenderHudMovie()
{
}

void XRayBlenderHudMovie::Initialize()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.Samplers[0].Shader = ST_Pixel;
	RootSignatureDescription.SRVResources[0].Shader = ST_Pixel;
	RootSignatureDescription.UniformBuffers[0].Shader = ST_Vertex;
	RootSignature = BearRenderInterface::CreateRootSignature(RootSignatureDescription);

	BearPipelineDescription PipelineDescription;
	SetInputLayout(PipelineDescription, FVF::F_TL);
	PipelineDescription.Shaders.Pixel = GResourcesManager->GetPixelShader("yuv2rgb");
	PipelineDescription.Shaders.Vertex = GResourcesManager->GetVertexShader("notransform_tl");
	PipelineDescription.RootSignature = RootSignature;

	Pipeline = BearRenderInterface::CreatePipeline(PipelineDescription);
}

void XRayBlenderHudMovie::Compile(XRayShaderElement& shader)
{
	if (IDShader == 0)
	{
		shader.Pipeline = Pipeline;
		shader.DescriptorHeap = CreateDescriptorHeap(RootSignature);
		SetTexture(shader, 0, "$base0");
		shader.DescriptorHeap->SetSampler(0, GResourcesManager->SamplerDefault);
		shader.TypeTransformation = STT_Screen;
	}
}
