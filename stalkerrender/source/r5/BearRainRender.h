#pragma once
class BearRainRender:public IRainRender
{
public:
	BearRainRender();
	virtual void Copy(IRainRender &_in) ;

	virtual void Render(CEffect_Rain &owner) ;

	virtual const Fsphere& GetDropBounds() const;
};