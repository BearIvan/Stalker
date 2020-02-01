#include "pch.h"

XRayBlenderScreenGray::XRayBlenderScreenGray()
{
	Description.ID = B_SCREEN_GRAY;
}

XRayBlenderScreenGray::~XRayBlenderScreenGray()
{
}

LPCSTR XRayBlenderScreenGray::getComment()
{
	return LPCSTR();
}

void XRayBlenderScreenGray::Save(IWriter & fs)
{
}

void XRayBlenderScreenGray::Load(IReader & fs, u16 version)
{
	XRayBlender::Load(fs, version);
}

void XRayBlenderScreenGray::Destroy()
{
	bear_delete(this);
}
