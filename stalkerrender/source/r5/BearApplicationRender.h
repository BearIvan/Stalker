#pragma once

class BearApplicationRender :public  IApplicationRender
{
public:
	BearApplicationRender();
	virtual void Copy(IApplicationRender &_in) ;
	virtual void LoadTitleInt(LPCSTR str) ;
	virtual void LoadBegin() ;
	virtual void destroy_loading_shaders() ;
	virtual void setLevelLogo(LPCSTR pszLogoName) ;
	virtual void load_draw_internal(CApplication &owner) ;
	//	?????
	virtual void KillHW() ;
};
