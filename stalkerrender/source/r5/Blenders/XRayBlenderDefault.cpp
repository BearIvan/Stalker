#include "pch.h"

XRayBlenderDefault::XRayBlenderDefault()
{
	Description.ID = B_DEFAULT;
	Description.Version = 1;
	oTessellation.Count = 4;
	oTessellation.IDselected = 0;
}

XRayBlenderDefault::~XRayBlenderDefault()
{
}

LPCSTR XRayBlenderDefault::getComment()
{
	return LPCSTR();
}

void XRayBlenderDefault::Save(IWriter & fs)
{
}

void XRayBlenderDefault::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	if (version > 0)
	{
		xrPREAD_PROP(fs, xrPID_TOKEN, oTessellation);
	}
}
