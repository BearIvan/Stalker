#include "pch.h"
#include "XRayBlenderHudCrosshair.h"

XRayBlenderHudCrosshair::XRayBlenderHudCrosshair()
{
}

XRayBlenderHudCrosshair::~XRayBlenderHudCrosshair()
{
}

void XRayBlenderHudCrosshair::Initialize()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.UniformBuffers[0].Shader = ST_Vertex;
	RootSignature[2] = BearRenderInterface::CreateRootSignature(RootSignatureDescription);
	RootSignature[3] = RootSignature[2];

	BearPipelineDescription PipelineDescription;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;
	PipelineDescription.TopologyType = TT_LINE_LIST;
	CreatePipeline(2, PipelineDescription, "notransform", "simple_color", SVD_TL0uv);
	PipelineDescription.TopologyType = TT_LINE_STRIP;
	CreatePipeline(3, PipelineDescription, "notransform", "simple_color", SVD_TL0uv);
}

void XRayBlenderHudCrosshair::Compile(XRayShaderElement& shader)
{
	shader.TypeTransformation = STT_Screen;
}