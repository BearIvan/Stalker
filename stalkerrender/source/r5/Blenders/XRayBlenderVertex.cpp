#include "pch.h"

XRayBlenderVertex::XRayBlenderVertex()
{
	Description.ID = B_VERT;
	Description.Version = 1;
	oTessellation.Count = 4;
	oTessellation.IDselected = 0;
}

XRayBlenderVertex::~XRayBlenderVertex()
{
}

LPCSTR XRayBlenderVertex::getComment()
{
	return LPCSTR();
}

void XRayBlenderVertex::Save(IWriter & fs)
{
}

void XRayBlenderVertex::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	if (version > 0)
	{
		xrPREAD_PROP(fs, xrPID_TOKEN, oTessellation);
	}
}

void XRayBlenderVertex::Initialize()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.Samplers[0].Shader = ST_Pixel;
	RootSignatureDescription.SRVResources[0].Shader = ST_Pixel;
	RootSignatureDescription.UniformBuffers[0].Shader = ST_Vertex;
	RootSignature = BearRenderInterface::CreateRootSignature(RootSignatureDescription);

	BearPipelineDescription PipelineDescription;
	PipelineDescription.DepthStencilState.DepthEnable = true;
	PipelineDescription.RenderPass = HW->RenderPass;
	CreatePipeline(PipelineDescription, "default", "default_tl", SVD_R1Vert);
}

void XRayBlenderVertex::Compile(XRayShaderElement& shader)
{
	if (IDShader==0)
	{
		SetTexture(shader, 0, "$base0");
		shader.SamplerStates[0] = SSS_Default;
		shader.TypeTransformation = STT_Matrix;

	}
}


