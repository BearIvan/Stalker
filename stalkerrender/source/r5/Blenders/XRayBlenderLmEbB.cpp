#include "pch.h"

XRayBlenderLmEbB::XRayBlenderLmEbB()
{
	Description.ID = B_LmEbB;
	Description.Version = 0x1;
	BearString::Copy(oT2_Name, TEXT("$null"));
	BearString::Copy(oT2_xform, TEXT("$null"));
	oBlend.value = FALSE;
}

XRayBlenderLmEbB::~XRayBlenderLmEbB()
{
}

LPCSTR XRayBlenderLmEbB::getComment()
{
	return LPCSTR();
}

void XRayBlenderLmEbB::Save(IWriter & fs)
{
}

void XRayBlenderLmEbB::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	xrPREAD_MARKER(fs);
	xrPREAD_PROP(fs, xrPID_TEXTURE, oT2_Name);
	xrPREAD_PROP(fs, xrPID_MATRIX, oT2_xform);
	if (version >= 0x1) {
		xrPREAD_PROP(fs, xrPID_BOOL, oBlend);
	}
}

