#include "StdAfx.h"
#include "ui/map_hint.h"
#include "ui/UIStatic.h"
#include "ui/UIFrameWindow.h"
#include "ui/UIXmlInit.h"

CUIMapHint::~CUIMapHint	()
{}

void CUIMapHint::Init		()
{
	CUIXmlInit xml_init;
	CUIXml uiXml;
 uiXml.Load(CONFIG_PATH, UI_PATH, "hint_item.xml");
	//R_ASSERT3						(xml_result, "xml file not found", "hint_item.xml");

	xml_init.InitWindow(uiXml,"hint_item",0,this);
	
	m_border			= xr_new<CUIFrameWindow>();m_border->SetAutoDelete(true);
	AttachChild			(m_border);
	xml_init.InitFrameWindow(uiXml,"hint_item:frame",0,m_border);

	m_text				= xr_new<CUIStatic>();m_text->SetAutoDelete(true);
	AttachChild			(m_text);
	xml_init.InitStatic	(uiXml,"hint_item:description",0,m_text);

}

void CUIMapHint::SetText		(LPCSTR text)
{
	m_text->SetTextST			(text);
	m_text->AdjustHeightToText	();
	float new_h					= XrMath::max(64.0f, m_text->GetWndPos().y+m_text->GetWndSize().y+20.0f);
	SetWndSize					(Fvector2().set(GetWndSize().x, new_h));
	m_border->SetWidth			(GetWndSize().x);
	m_border->SetHeight			(GetWndSize().y);
}
void CUIMapHint::Draw_		()
{
	inherited::Draw			();
}
