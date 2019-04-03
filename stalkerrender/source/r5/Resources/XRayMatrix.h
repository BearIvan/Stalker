#pragma once
#include "engine/WaveForm.h"
class XRayMatrix
{
public:
	XRayMatrix();
	~XRayMatrix();
	WaveForm		scaleU, scaleV;
	WaveForm		rotate;
	WaveForm		scrollU, scrollV;

	u32				tcm;
	u32				dwMode;
	void			Load(IReader* fs);
	void			Save(IWriter* fs);
	
};