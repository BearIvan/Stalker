#include "pch.h"

XRayBlenderTree::XRayBlenderTree()
{
	Description.ID = B_TREE;
	Description.Version = 1;
	oBlend.value = FALSE;
	oNotAnTree.value = FALSE;
}

XRayBlenderTree::~XRayBlenderTree()
{
}

LPCSTR XRayBlenderTree::getComment()
{
	return LPCSTR();
}

void XRayBlenderTree::Save(IWriter & fs)
{
}

void XRayBlenderTree::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	xrPREAD_PROP(fs, xrPID_BOOL, oBlend);
	if (version >= 1) {
		xrPREAD_PROP(fs, xrPID_BOOL, oNotAnTree);
	}
}

