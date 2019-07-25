#include "pch.h"

XRayBlenderDetailStill::XRayBlenderDetailStill()
{
	Description.ID = B_DETAIL;
	Description.Version = 0;
}

XRayBlenderDetailStill::~XRayBlenderDetailStill()
{
}

LPCSTR XRayBlenderDetailStill::getComment()
{
	return LPCSTR();
}

void XRayBlenderDetailStill::Save(IWriter & fs)
{
}

void XRayBlenderDetailStill::Load(IReader & fs, u16 version)
{
	XRayBlender::Load(fs, version);
	xrPREAD_PROP(fs, xrPID_BOOL, oBlend);
}

void XRayBlenderDetailStill::Destroy()
{
	bear_delete(this);
}
