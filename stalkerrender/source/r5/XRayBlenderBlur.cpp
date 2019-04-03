#include "pch.h"

XRayBlenderBlur::XRayBlenderBlur()
{
	Description.ID = B_BLUR;
}

XRayBlenderBlur::~XRayBlenderBlur()
{
}

LPCSTR XRayBlenderBlur::getComment()
{
	return LPCSTR();
}

void XRayBlenderBlur::Save(IWriter & fs)
{
}

void XRayBlenderBlur::Load(IReader & fs, u16 version)
{
	XRayBlender::Load(fs, version);
}

void XRayBlenderBlur::Destroy()
{
	bear_delete(this);
}
