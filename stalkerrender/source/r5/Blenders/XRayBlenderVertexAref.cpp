#include "pch.h"

XRayBlenderVertexAref::XRayBlenderVertexAref()
{
	Description.ID = B_VERT_AREF;
	Description.Version = 1;
	oAREF.value = 32;
	oAREF.min = 0;
	oAREF.max = 255;
	oBlend.value = FALSE;
}

XRayBlenderVertexAref::~XRayBlenderVertexAref()
{
}

LPCSTR XRayBlenderVertexAref::getComment()
{
	return LPCSTR();
}

void XRayBlenderVertexAref::Save(IWriter & fs)
{
}

void XRayBlenderVertexAref::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);

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


