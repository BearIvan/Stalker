#include "pch.h"

XRayMatrix::XRayMatrix()
{
}

XRayMatrix::~XRayMatrix()
{
}


void XRayMatrix::Load(IReader* fs)
{
	dwMode = fs->r_u32();
	tcm = fs->r_u32();
	fs->r(&scaleU, sizeof(WaveForm));
	fs->r(&scaleV, sizeof(WaveForm));
	fs->r(&rotate, sizeof(WaveForm));
	fs->r(&scrollU, sizeof(WaveForm));
	fs->r(&scrollV, sizeof(WaveForm));
}

void XRayMatrix::Save(IWriter* fs)
{
	fs->w_u32(dwMode);
	fs->w_u32(tcm);
	fs->w(&scaleU, sizeof(WaveForm));
	fs->w(&scaleV, sizeof(WaveForm));
	fs->w(&rotate, sizeof(WaveForm));
	fs->w(&scrollU, sizeof(WaveForm));
	fs->w(&scrollV, sizeof(WaveForm));
}
