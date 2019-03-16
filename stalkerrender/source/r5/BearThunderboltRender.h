#pragma once
class BearThunderboltRender:public IThunderboltRender
{
public:
	BearThunderboltRender();
	virtual void Copy(IThunderboltRender &_in) ;

	virtual void Render(CEffect_Thunderbolt &owner);
};
