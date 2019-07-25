#include "pch.h"

#define		VER_2_oBlendCount	7
#define		VER_4_oBlendCount	9
#define		VER_5_oBlendCount	10

XRayBlenderScreenSet::XRayBlenderScreenSet()
{
	Description.ID = B_SCREEN_SET;
	Description.Version = 4;
	oBlend.Count = VER_4_oBlendCount;
	oBlend.IDselected = 0;
	oAREF.value = 32;
	oAREF.min = 0;
	oAREF.max = 255;
	oZTest.value = FALSE;
	oZWrite.value = FALSE;
	oLighting.value = FALSE;
	oFog.value = FALSE;
	oClamp.value = TRUE;
}

XRayBlenderScreenSet::~XRayBlenderScreenSet()
{
}

LPCSTR XRayBlenderScreenSet::getComment()
{
	return LPCSTR();
}

void XRayBlenderScreenSet::Save(IWriter & fs)
{
}

void XRayBlenderScreenSet::Load(IReader & fs, u16 version)
{
	XRayBlender::Load(fs, version);

	switch (version) {
	case 2:
		xrPREAD_PROP(fs, xrPID_TOKEN, oBlend);	oBlend.Count = VER_5_oBlendCount;
		xrPREAD_PROP(fs, xrPID_INTEGER, oAREF);
		xrPREAD_PROP(fs, xrPID_BOOL, oZTest);
		xrPREAD_PROP(fs, xrPID_BOOL, oZWrite);
		xrPREAD_PROP(fs, xrPID_BOOL, oLighting);
		xrPREAD_PROP(fs, xrPID_BOOL, oFog);
		break;
	case 3:
		xrPREAD_PROP(fs, xrPID_TOKEN, oBlend);	oBlend.Count = VER_5_oBlendCount;
		xrPREAD_PROP(fs, xrPID_BOOL, oClamp);
		xrPREAD_PROP(fs, xrPID_INTEGER, oAREF);
		xrPREAD_PROP(fs, xrPID_BOOL, oZTest);
		xrPREAD_PROP(fs, xrPID_BOOL, oZWrite);
		xrPREAD_PROP(fs, xrPID_BOOL, oLighting);
		xrPREAD_PROP(fs, xrPID_BOOL, oFog);
		break;
	default:
		xrPREAD_PROP(fs, xrPID_TOKEN, oBlend);	oBlend.Count = VER_5_oBlendCount;
		xrPREAD_PROP(fs, xrPID_BOOL, oClamp);
		xrPREAD_PROP(fs, xrPID_INTEGER, oAREF);
		xrPREAD_PROP(fs, xrPID_BOOL, oZTest);
		xrPREAD_PROP(fs, xrPID_BOOL, oZWrite);
		xrPREAD_PROP(fs, xrPID_BOOL, oLighting);
		xrPREAD_PROP(fs, xrPID_BOOL, oFog);
		break;
	}
}

void XRayBlenderScreenSet::Compile(XRayShader & shader)
{
	shader.SetVertexState(FVF::F_TL);
	shader.SetShader( "default_tl", "notransform_tl");
	shader.SetTransformType(shader.TT_Screen);
	switch (oBlend.IDselected)
	{
	case 6:
		shader.SetBlend(XRayShader::BF_SRC_ALPHA, XRayShader::BF_INV_SRC_ALPHA);
		break;
	default:
		break;
	}
	switch (oBlend.IDselected)
	{
	case 0:	// SET
		break;
	case 1: // BLEND
		shader.SetBlend(XRayShader::BF_SRC_ALPHA, XRayShader::BF_INV_SRC_ALPHA);
		break;
	case 2:	// ADD
		shader.SetBlend(XRayShader::BF_ONE, XRayShader::BF_ONE);
		break;
	case 3:	// MUL
		shader.SetBlend(XRayShader::BF_DEST_COLOR, XRayShader::BF_ZERO);
		break;
	case 4:	// MUL_2X
		shader.SetBlend(XRayShader::BF_DEST_COLOR, XRayShader::BF_SRC_COLOR);
		break;
	case 5:	// ALPHA-ADD
		shader.SetBlend(XRayShader::BF_SRC_ALPHA, XRayShader::BF_ONE);
		break;
	case 6:	// MUL_2X + A-test
		shader.SetBlend(XRayShader::BF_DEST_COLOR, XRayShader::BF_SRC_COLOR);
		break;
	case 7:	// SET (2r)
		break;
	case 8: // BLEND (2r)
		shader.SetBlend(XRayShader::BF_SRC_ALPHA, XRayShader::BF_INV_SRC_ALPHA);
		break;
	case 9: // BLEND (2r)
		shader.SetBlend(XRayShader::BF_SRC_ALPHA, XRayShader::BF_INV_SRC_ALPHA);
		break;
	}
}

void XRayBlenderScreenSet::Destroy()
{
	bear_delete(this);
}
