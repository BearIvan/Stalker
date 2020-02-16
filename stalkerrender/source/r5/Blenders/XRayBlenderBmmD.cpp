#include "pch.h"

XRayBlenderBmmD::XRayBlenderBmmD()
{
	Description.ID = B_BmmD;
	Description.Version = 3;

	BearString::Copy(oT2_Name, TEXT("$null"));
	BearString::Copy(oT2_xform, TEXT("$null"));
	BearString::Copy(oR_Name, TEXT("detail\\detail_grnd_grass"));	//"$null");
	BearString::Copy(oG_Name, TEXT("detail\\detail_grnd_asphalt"));	//"$null");
	BearString::Copy(oB_Name, TEXT("detail\\detail_grnd_earth"));	//"$null");
	BearString::Copy(oA_Name, TEXT("detail\\detail_grnd_yantar"));	//"$null");
}

XRayBlenderBmmD::~XRayBlenderBmmD()
{
}

LPCSTR XRayBlenderBmmD::getComment()
{
	return LPCSTR();
}

void XRayBlenderBmmD::Save(IWriter & fs)
{
}

void XRayBlenderBmmD::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	if (version < 3) {
		xrPREAD_MARKER(fs);
		xrPREAD_PROP(fs, xrPID_TEXTURE, oT2_Name);
		xrPREAD_PROP(fs, xrPID_MATRIX, oT2_xform);
	}
	else {
		xrPREAD_MARKER(fs);
		xrPREAD_PROP(fs, xrPID_TEXTURE, oT2_Name);
		xrPREAD_PROP(fs, xrPID_MATRIX, oT2_xform);
		xrPREAD_PROP(fs, xrPID_TEXTURE, oR_Name);
		xrPREAD_PROP(fs, xrPID_TEXTURE, oG_Name);
		xrPREAD_PROP(fs, xrPID_TEXTURE, oB_Name);
		xrPREAD_PROP(fs, xrPID_TEXTURE, oA_Name);
	}
}

void XRayBlenderBmmD::Initialize()
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

void XRayBlenderBmmD::Compile(XRayShaderElement& shader)
{
	if (IDShader == 0)
	{
		SetTexture(shader, 0, "$base0");
		shader.SamplerStates[0] = SSS_Default;
		shader.TypeTransformation = STT_Matrix;

	}
}

