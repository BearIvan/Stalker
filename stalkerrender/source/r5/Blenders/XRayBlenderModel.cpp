#include "pch.h"

XRayBlenderModel::XRayBlenderModel()
{
	Description.ID = B_MODEL;
	Description.Version = 2;
	oTessellation.Count = 4;
	oTessellation.IDselected = 0;
	oAREF.value = 32;
	oAREF.min = 0;
	oAREF.max = 255;
	oBlend.value = FALSE;
}

XRayBlenderModel::~XRayBlenderModel()
{
}

LPCSTR XRayBlenderModel::getComment()
{
	return LPCSTR();
}

void XRayBlenderModel::Save(IWriter & fs)
{
}

void XRayBlenderModel::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	switch (version)
	{
	case 0:
		oAREF.value = 32;
		oAREF.min = 0;
		oAREF.max = 255;
		oBlend.value = FALSE;
		break;
	case 1:
	default:
		xrPREAD_PROP(fs, xrPID_BOOL, oBlend);
		xrPREAD_PROP(fs, xrPID_INTEGER, oAREF);
		break;
	}
	if (version > 1)
	{
		xrPREAD_PROP(fs, xrPID_TOKEN, oTessellation);
	}
}

