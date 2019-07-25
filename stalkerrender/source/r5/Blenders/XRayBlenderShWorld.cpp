#include "pch.h"

XRayBlenderShWorld::XRayBlenderShWorld()
{
	Description.ID = B_SHADOW_WORLD;
}

XRayBlenderShWorld::~XRayBlenderShWorld()
{
}

LPCSTR XRayBlenderShWorld::getComment()
{
	return LPCSTR();
}

void XRayBlenderShWorld::Save(IWriter & fs)
{
}

void XRayBlenderShWorld::Load(IReader & fs, u16 version)
{
	XRayBlender::Load(fs, version);
}

void XRayBlenderShWorld::Destroy()
{
	bear_delete(this);
}
