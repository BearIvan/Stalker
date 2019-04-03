#include "pch.h"

XRayBlenderVertex::XRayBlenderVertex()
{
	Description.ID = B_VERT;
	Description.Version = 1;
	oTessellation.Count = 4;
	oTessellation.IDselected = 0;
}

XRayBlenderVertex::~XRayBlenderVertex()
{
}

LPCSTR XRayBlenderVertex::getComment()
{
	return LPCSTR();
}

void XRayBlenderVertex::Save(IWriter & fs)
{
}

void XRayBlenderVertex::Load(IReader & fs, u16 version)
{
	XRayBlender::Load(fs, version);
	if (version > 0)
	{
		xrPREAD_PROP(fs, xrPID_TOKEN, oTessellation);
	}
}

void XRayBlenderVertex::Destroy()
{
	bear_delete(this);
}
