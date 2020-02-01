#include "pch.h"

XRayBlenderLaEmB::XRayBlenderLaEmB()
{
	Description.ID = B_LaEmB;
	BearString::Copy(oT2_Name, TEXT("$null"));
	BearString::Copy(oT2_xform, TEXT("$null"));
	BearString::Copy(oT2_const, TEXT("$null"));
}

XRayBlenderLaEmB::~XRayBlenderLaEmB()
{
}

LPCSTR XRayBlenderLaEmB::getComment()
{
	return LPCSTR();
}

void XRayBlenderLaEmB::Save(IWriter & fs)
{
}

void XRayBlenderLaEmB::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	xrPREAD_MARKER(fs);
	xrPREAD_PROP(fs, xrPID_TEXTURE, oT2_Name);
	xrPREAD_PROP(fs, xrPID_MATRIX, oT2_xform);
	xrPREAD_PROP(fs, xrPID_CONSTANT, oT2_const);
}

