#pragma once
class BearFlareRender:public IFlareRender
{
public:
	BearFlareRender();
	virtual void Copy(IFlareRender &_in) ;

	virtual void CreateShader(LPCSTR sh_name, LPCSTR tex_name);
	virtual void DestroyShader() ;
};