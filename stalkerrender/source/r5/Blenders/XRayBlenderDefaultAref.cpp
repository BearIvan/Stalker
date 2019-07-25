#include "pch.h"

XRayBlenderDefaultAref::XRayBlenderDefaultAref()
{
	Description.ID = B_DEFAULT_AREF;
	Description.Version = 1;
	oAREF.value = 32;
	oAREF.min = 0;
	oAREF.max = 255;
	oBlend.value = FALSE;
}

XRayBlenderDefaultAref::~XRayBlenderDefaultAref()
{
}

LPCSTR XRayBlenderDefaultAref::getComment()
{
	return LPCSTR();
}

void XRayBlenderDefaultAref::Save(IWriter & fs)
{
}

void XRayBlenderDefaultAref::Load(IReader & fs, u16 version)
{
	XRayBlender::Load(fs, version);

	switch (version)
	{
	case 0:
		xrPREAD_PROP(fs, xrPID_INTEGER, oAREF);
		oBlend.value = FALSE;
		break;
	case 1:
	default:
		xrPREAD_PROP(fs, xrPID_INTEGER, oAREF);
		xrPREAD_PROP(fs, xrPID_BOOL, oBlend);
		break;
	}
}

void XRayBlenderDefaultAref::Destroy()
{
	bear_delete(this);
}
