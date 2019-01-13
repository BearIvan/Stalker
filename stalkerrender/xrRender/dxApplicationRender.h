#ifndef dxApplicationRender_included
#define dxApplicationRender_included
#pragma once

#include "ApplicationRender.h"

class dxApplicationRender : public IApplicationRender
{
public:
	virtual void Copy(IApplicationRender &_in);
	virtual void LoadTitleInt(LPCSTR str);
	virtual void LoadBegin();
	virtual void destroy_loading_shaders();
	virtual void setLevelLogo(LPCSTR pszLogoName);
	virtual void load_draw_internal(CApplication &owner);
	//	?????
	virtual void KillHW();

private:
	ref_shader				hLevelLogo;
	ref_shader				hLevelLogo_Add;
	ref_geom				ll_hGeom;
	ref_geom				ll_hGeom2;

	ref_shader				sh_progress;
	void					draw_face		(ref_shader& sh, Frect& coords, Frect& tex_coords, const Fvector2& tex_size);
};

class dxApplicationRenderSOC : public IApplicationRender
{
public:
	virtual void Copy(IApplicationRender &_in);
	virtual void LoadTitleInt(LPCSTR str);
	virtual void LoadBegin();
	virtual void destroy_loading_shaders();
	virtual void setLevelLogo(LPCSTR pszLogoName);
	virtual void load_draw_internal(CApplication &owner);
	//	?????
	virtual void KillHW();

private:
	CGameFont*				pFontSystem;
	string256				app_title;
	ref_shader				hLevelLogo;
	ref_geom				ll_hGeom;
	ref_geom				ll_hGeom2;

	ref_shader				sh_progress;
	int		max_load_stage;
	int		load_stage;
};

#endif	//	ApplicationRender_included