#include "stdafx.h"
#include "xrRender/UIShader.h"
#include "xrRender/UIRender.h"
#include "uistaticitem.h"
#include "hudmanager.h"

//ref_geom		hGeom_fan = NULL;	
/*
void CreateUIGeom()
{
	hGeom_fan.create(FVF::F_TL, RCache.Vertex.Buffer(), 0);
}*/
/*
void DestroyUIGeom()
{
	hGeom_fan = NULL;
}

ref_geom	GetUIGeom()
{
	return hGeom_fan;
}*/

CUIStaticItem::CUIStaticItem()
{    
	dwColor			= 0xffffffff;
	iTileX			= 1;
	iTileY			= 1;
	iRemX			= 0.0f;
	iRemY			= 0.0f;
	alpha_ref		= -1;
	//hShader			= NULL;
#ifdef DEBUG
	dbg_tex_name = NULL;
#endif
}

CUIStaticItem::~CUIStaticItem()
{
}

void CUIStaticItem::CreateShader(LPCSTR tex, LPCSTR sh)
{
	hShader->create(sh,tex);

#ifdef DEBUG
	dbg_tex_name = tex;
#endif
	uFlags &= !flValidRect;
}

void CUIStaticItem::SetShader(const ui_shader& sh)
{
	hShader = sh;
}

void CUIStaticItem::Init(LPCSTR tex, LPCSTR sh, float left, float top, u32 align)
{
	uFlags &= !flValidRect;

	CreateShader	(tex,sh);
	SetPos			(left,top);
	SetAlign		(align);
}

void CUIStaticItem::Render()
{
	VERIFY(g_bRendering);
	UIRender->SetShader(*hShader);

	if (alpha_ref != -1)UIRender->SetAlphaRef(alpha_ref);
		//CHK_DX(HW.pDevice->SetRenderState(D3DRS_ALPHAREF,));
	// convert&set pos
	Fvector2		bp;
	UI()->ClientToScreenScaled	(bp,float(iPos.x),float(iPos.y));
	bp.x						= (float)iFloor(bp.x);
	bp.y						= (float)iFloor(bp.y);

	// actual rendering
	u32							vOffset;
	Fvector2					pos;
	Fvector2					f_len;
	UI()->ClientToScreenScaled	(f_len, iVisRect.x2, iVisRect.y2 );

	int tile_x		 = fis_zero(iRemX) ? iTileX : iTileX + 1;
	int tile_y		 = fis_zero(iRemY) ? iTileY : iTileY + 1;
	int							x,y;
	if (!(tile_x&&tile_y))		return;
	// render
	UIRender->StartPrimitive(tile_x*tile_y*8, IUIRender::ptTriList, IUIRender::pttTL);

	for (x=0; x<tile_x; ++x){
		for (y=0; y<tile_y; ++y){
			pos.set				(bp.x+f_len.x*x,bp.y+f_len.y*y);
			inherited::Render	(pos,dwColor);
		}
	}

	// set scissor
	Frect clip_rect				= {iPos.x,iPos.y,iPos.x+iVisRect.x2*iTileX+iRemX,iPos.y+iVisRect.y2*iTileY+iRemY};
	UI()->PushScissor			(clip_rect);
	// set geom
	UIRender->FlushPrimitive();
	if(alpha_ref!=-1)
		UIRender->SetAlphaRef(0);
	UI()->PopScissor			();
}

void CUIStaticItem::Render(float angle)
{
	VERIFY						(g_bRendering);
	// установить обязательно перед вызовом CustomItem::Render() !!!
	VERIFY(!!hShader);
	UIRender->SetShader(*hShader);
	if(alpha_ref!=-1)
		UIRender->SetAlphaRef(alpha_ref);
	// convert&set pos
	Fvector2		bp_ns;
	bp_ns.set		(iPos);


	// actual rendering
	u32		vOffset;
	UIRender->StartPrimitive(32, IUIRender::ptTriList, IUIRender::pttTL);
	inherited::Render			(bp_ns,dwColor,angle);
	// unlock VB and Render it as triangle LIST
	UIRender->FlushPrimitive();
	if(alpha_ref!=-1)
		UIRender->SetAlphaRef(0);
}
