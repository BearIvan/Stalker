#include "pch.h"

XRayBlenderModelEbB::XRayBlenderModelEbB()
{
	Description.ID = B_MODEL_EbB;
	Description.Version = 0x1;
	BearString::Copy(oT2_Name, TEXT("$null"));
	BearString::Copy(oT2_xform, TEXT("$null"));
	oBlend.value = FALSE;
}

XRayBlenderModelEbB::~XRayBlenderModelEbB()
{
}

LPCSTR XRayBlenderModelEbB::getComment()
{
	return LPCSTR();
}

void XRayBlenderModelEbB::Save(IWriter & fs)
{
}

void XRayBlenderModelEbB::Load(IReader & fs, u16 version)
{
	XRayBlender::Load(fs, version);
	xrPREAD_MARKER(fs);
	xrPREAD_PROP(fs, xrPID_TEXTURE, oT2_Name);
	xrPREAD_PROP(fs, xrPID_MATRIX, oT2_xform);
	if (version >= 0x1) {
		xrPREAD_PROP(fs, xrPID_BOOL, oBlend);
	}
}

void XRayBlenderModelEbB::Destroy()
{
	bear_delete(this);
}
