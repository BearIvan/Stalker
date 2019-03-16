#pragma once
class BearUIShader:public IUIShader
{
public:
	BearUIShader();
	virtual void Copy(IUIShader &_in) ;
	virtual void create(LPCSTR sh, LPCSTR tex = 0) ;
	virtual bool inited();
	virtual void destroy();
};