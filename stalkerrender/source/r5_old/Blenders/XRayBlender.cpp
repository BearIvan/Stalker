#include "pch.h"

LPCSTR XRayBlender::getName()
{
	return Description.Name;
}

LPCSTR XRayBlender::getComment()
{
	return "Он пуст";
}

void XRayBlender::Save(IWriter & fs)
{
	BEAR_RASSERT(false);
}

void XRayBlender::Load(IReader & fs, u16 version)
{
	u16	V = Description.Version;
	fs.r(&Description, sizeof(Description));
	Description.Version = V;

	xrPREAD_MARKER(fs);
	xrPREAD_PROP(fs, xrPID_INTEGER, oPriority);
	xrPREAD_PROP(fs, xrPID_BOOL, oStrictSorting);
	xrPREAD_MARKER(fs);
	xrPREAD_PROP(fs, xrPID_TEXTURE, oTName);
	xrPREAD_PROP(fs, xrPID_MATRIX, oTXform);
}

void XRayBlender::Destroy()
{
	BEAR_RASSERT(false);
}

void XRayBlender::Compile(XRayShader & shader)
{
	BEAR_RASSERT(false);
}
