#pragma once
#include "engine/WaveForm.h"
class XRayConstant
{
public:
	XRayConstant();
	~XRayConstant();
	WaveForm		_R;
	WaveForm		_G;
	WaveForm		_B;
	WaveForm		_A;
	void			Load(IReader* fs);
	void			Save(IWriter* fs);
};