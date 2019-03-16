#pragma once

class BearObjectSpaceRender:public IObjectSpaceRender
{
public:
	BearObjectSpaceRender();
	virtual void Copy(IObjectSpaceRender &_in);;

	virtual void dbgRender();;
	virtual void dbgAddSphere(const Fsphere &sphere, u32 colour);;
	virtual void SetShader();;
};
