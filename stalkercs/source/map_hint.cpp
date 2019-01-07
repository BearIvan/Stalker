#include "stdafx.h"
#include "ui/map_hint.h"
#include "ui/UIStatic.h"
#include "ui/UIXmlInit.h"
#include "map_location.h"
#include "map_spot.h"
#include "actor.h"
#include "gametaskmanager.h"
#include "gametask.h"
#include "ui/UIInventoryUtilities.h"
#include "string_table.h"

CUIStatic* init_static_field(CUIXml& uiXml, LPCSTR path, LPCSTR path2)
{
	CUIStatic* S					= xr_new<CUIStatic>();
	string512						buff;
	S->SetAutoDelete				(true);
	strconcat						(sizeof(buff),buff,path,":",path2);
	CUIXmlInit::InitStatic			(uiXml,buff,0,S);

	return							S;
}

void CUIMapLocationHint::Init(CUIXml& uiXml, LPCSTR path)
{
	CUIXmlInit						xml_init;

	xml_init.InitFrameWindow		(uiXml,path,0,this);

	CUIStatic* S					= NULL;

	S = init_static_field			(uiXml, path, "simple_text");
	AttachChild						(S);
	m_info["simple_text"]			= S;

	S = init_static_field			(uiXml, path, "t_icon");
	AttachChild						(S);
	m_info["t_icon"]				= S;

	S = init_static_field			(uiXml, path, "t_caption");
	AttachChild						(S);
	m_info["t_caption"]				= S;

	S = init_static_field			(uiXml, path, "t_time");
	AttachChild						(S);
	m_info["t_time"]				= S;

	S = init_static_field			(uiXml, path, "t_time_rem");
	AttachChild						(S);
	m_info["t_time_rem"]			= S;

	S = init_static_field			(uiXml, path, "t_hint_text");
	AttachChild						(S);
	m_info["t_hint_text"]			= S;

	m_posx_icon    = m_info["t_icon"]->GetWndPos().x;
	m_posx_caption = m_info["t_caption"]->GetWndPos().x;
}

void CUIMapLocationHint::SetInfoMode(u8 mode)
{
	m_info["simple_text"]->Show		(mode==1);
	m_info["t_icon"]->Show			(mode==2);
	m_info["t_caption"]->Show		(mode==2);
	m_info["t_time"]->Show			(mode==2);
	m_info["t_time_rem"]->Show		(mode==2);
	m_info["t_hint_text"]->Show		(mode==2);
}

void CUIMapLocationHint::Draw_()
{
	inherited::Draw			();
}

void CUIMapLocationHint::SetInfoStr(LPCSTR text)
{
	SetInfoMode				(1);
	CUIStatic* S			= m_info["simple_text"];
	S->SetTextST			(text);
	S->AdjustHeightToText	();
	float new_w				= S->GetWndPos().x + S->GetWndSize().x + 20.0f;

	float new_h				= _max(64.0f, S->GetWndPos().y+S->GetWndSize().y+20.0f);
	SetWndSize				(Fvector2().set(new_w, new_h));
}

void CUIMapLocationHint::SetInfoMSpot(CMapSpot* spot)
{
	CMapLocation* ml		= spot->MapLocation();

	CGameTask* gt			= Level().GameTaskManager().HasGameTask(ml, true);
	if(gt)
		SetInfoTask			(gt);
	else
		SetInfoStr			(ml->GetHint());
}

void CUIMapLocationHint::SetInfoTask(CGameTask* task)
{
	SetInfoMode						(2);
	CUIStatic* S					= m_info["t_icon"];
	
	S->InitTexture					(task->m_icon_texture_name.c_str());
	S->SetStretchTexture			(true);

	S								= m_info["t_caption"];
	S->SetTextST					(task->m_Title.c_str());
	S->AdjustHeightToText			();
	//float new_w						= S->GetWndPos().x + S->GetWndSize().x + 20.0f;

	S					= m_info["t_time"];
	S->SetText			( InventoryUtilities::GetTimeAndDateAsString( task->m_ReceiveTime ).c_str() );
	
	Fvector2 pos		= S->GetWndPos();
	pos.y				= m_info["t_caption"]->GetWndPos().y + m_info["t_caption"]->GetWndSize().y + 7;
	S->SetWndPos		(pos);

	S					= m_info["t_time_rem"];
	bool b_rem			= (task->m_ReceiveTime!=task->m_TimeToComplete);
	S->Show				(b_rem);
	if(b_rem)
	{
		string512		buff, buff2;
		InventoryUtilities::GetTimePeriodAsString(buff, sizeof(buff), Level().GetGameTime(), task->m_TimeToComplete);
		
		strconcat		(sizeof(buff2),buff2, CStringTable().translate("ui_st_time_remains").c_str(), " ", buff);
		S->SetText		(buff2);
	}
	pos				= S->GetWndPos();
	pos.y			= m_info["t_time"]->GetWndPos().y + m_info["t_time"]->GetWndSize().y + 7;
	S->SetWndPos	(pos);

	S						= m_info["t_hint_text"];
	S->SetTextST			(task->m_Description.c_str());
	S->AdjustHeightToText	();
//	pos						= S->GetWndPos();
	if ( b_rem )
	{
		S = m_info["t_time_rem"];
	}
	else
	{
		S = m_info["t_time"];
	}
	pos.x					= m_posx_icon;
	pos.y					= S->GetWndPos().y + S->GetWndSize().y + 10;
	m_info["t_hint_text"]->SetWndPos(pos);
		
	
	if ( task->GetTaskType() == eTaskTypeStoryline )
	{
		m_info["t_icon"]->Show( true );
		float w = m_info["t_time"]->GetWidth();

		Fvector2 pos = m_info["t_icon"]->GetWndPos();
		pos.x = m_posx_icon;
		m_info["t_icon"]->SetWndPos( pos );
		
		pos	  = m_info["t_caption"]->GetWndPos();
		pos.x = m_posx_caption;
		m_info["t_caption"]->SetWndPos( pos );
		m_info["t_caption"]->SetWidth( w );

		pos   = m_info["t_time"]->GetWndPos();
		pos.x = m_posx_caption;
		m_info["t_time"]->SetWndPos( pos );
		
		pos   = m_info["t_time_rem"]->GetWndPos();
		pos.x = m_posx_caption;
		m_info["t_time_rem"]->SetWndPos( pos );

		pos   = m_info["t_hint_text"]->GetWndPos();
		pos.y = _max( pos.y, m_info["t_icon"]->GetWndPos().y + m_info["t_icon"]->GetWndSize().y + 7 );
		m_info["t_hint_text"]->SetWndPos( pos );
	}
	else if ( task->GetTaskType() == eTaskTypeAdditional )
	{
		m_info["t_icon"]->Show( false );
		float w = m_info["t_hint_text"]->GetWidth();

		Fvector2 pos = m_info["t_caption"]->GetWndPos();
		pos.x        = m_posx_icon;
		m_info["t_caption"]->SetWndPos( pos );
		m_info["t_caption"]->SetWidth( w );

		pos   = m_info["t_time"]->GetWndPos();
		pos.x = m_posx_icon;
		m_info["t_time"]->SetWndPos( pos );

		pos   = m_info["t_time_rem"]->GetWndPos();
		pos.x = m_posx_icon;
		m_info["t_time_rem"]->SetWndPos( pos );
	}

	pos.x = m_info["t_hint_text"]->GetWndPos().x + m_info["t_hint_text"]->GetWndSize().x + 20.0f;
	pos.y = m_info["t_hint_text"]->GetWndPos().y + m_info["t_hint_text"]->GetWndSize().y + 20.0f;
	SetWndSize( pos );
}