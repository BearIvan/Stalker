#include "pch.h"

XRayConstant::XRayConstant()
{
}

XRayConstant::~XRayConstant()
{
}

void XRayConstant::Load(IReader * fs)
{
	fs->r(&_R, sizeof(WaveForm));
	fs->r(&_G, sizeof(WaveForm));
	fs->r(&_B, sizeof(WaveForm));
	fs->r(&_A, sizeof(WaveForm));
}

void XRayConstant::Save(IWriter * fs)
{
	fs->w(&_R, sizeof(WaveForm));
	fs->w(&_G, sizeof(WaveForm));
	fs->w(&_B, sizeof(WaveForm));
	fs->w(&_A, sizeof(WaveForm));
}
