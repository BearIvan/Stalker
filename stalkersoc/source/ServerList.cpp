#include "StdAfx.h"
#include "ui/ServerList.h"
#include "ui/UIXmlInit.h"
#include "string_table.h"
#include "engine/xr_ioconsole.h"
#include "ui/UIEditBox.h"
#include "ui/UIColorAnimatorWrapper.h"
#include "ui/UIListItemAdv.h"
#include "ui/UIMessageBoxEx.h"
#include "ui/UIMessageBox.h"
#include "ui/TeamInfo.h"
#include "MainMenu.h"

CGameSpy_Browser* g_gs_browser = NULL;

CServerList::CServerList()
{
	

	for (int i = 0; i<LST_COLUMN_COUNT; i++)
		AttachChild(&m_header_frames[i]);

	for (int i = 0; i<LST_COLUMN_COUNT; i++)
		AttachChild(&m_header[i]);

	for (int i = 0; i<4; i++)
		AttachChild(&m_header2[i]);


    m_pAnimation				= xr_new<CUIColorAnimatorWrapper>("ui_mm_mp_srvinfo");
	m_pAnimation->Cyclic		(false);

	AttachChild(&m_edit_gs_filter);


	for (int i = 0; i<3; i++)
	{
		m_list[i].ShowSelectedItem	();
		AttachChild					(&m_frame[i]);
		AttachChild					(&m_list[i]);
	}

	m_bShowServerInfo				= false;
	m_bAnimation					= false;

	m_sort_func						= "none";
	m_message_box					= xr_new<CUIMessageBoxEx>();
	m_message_box->Init				("message_box_password");
	m_message_box->SetMessageTarget	(this);
	
	m_b_local						= false;

	m_last_retreived_index			= u32(-1);
	m_need_refresh_fr				= u32(-1);
}

CServerList::~CServerList()
{
	xr_delete			(m_pAnimation);
	xr_delete			(m_message_box);

	DestroySrvItems		();
};

void CServerList::Init(float x, float y, float width, float height)
{
	CUIWindow::Init(x,y,width,height);
}

void CServerList::Update()
{
	
//	if (m_GSBrowser) m_GSBrowser->Update();
	
	if (m_need_refresh_fr<Device.dwFrame+10)
		RefreshList_internal();

	if (m_bAnimation)
	{
		m_pAnimation->Update();

		m_frame[LST_SRV_PROP].SetColor(XrColor::subst_alpha(0xffffffff, XrColor::color_get_A(m_pAnimation->GetColor())));
		m_frame[LST_PLAYERS].SetColor(XrColor::subst_alpha(0xffffffff, XrColor::color_get_A(m_pAnimation->GetColor())));

		for (int i = 0; i<4; i++)
		{
			m_header2[i].SetColor(XrColor::subst_alpha(0xffffffff, XrColor::color_get_A(m_pAnimation->GetColor())));
			m_header2[i].SetTextColor(XrColor::subst_alpha(m_header2[i].GetTextColor(), XrColor::color_get_A(m_pAnimation->GetColor())));
		}

		if (m_pAnimation->Done())
		{
			m_bAnimation = false;
			if (m_bShowServerInfo)
				AfterAppear();
			else
				AfterDisappear();
		}
	}
	CUIWindow::Update();
}

bool CServerList::NeedToRefreshCurServer	()
{
	int SvId = m_list[LST_SERVER].GetSelectedItem();
	if (-1 == SvId)
		return false;
	return false;
};

void CServerList::SendMessage(CUIWindow* pWnd, s16 msg, void* pData){
	if (m_bShowServerInfo && (LIST_ITEM_CLICKED == msg) && (&m_list[LST_SERVER] == pWnd) )
	{
		if (NeedToRefreshCurServer())
		{
			RefreshQuick();
		}
		else
		{
			ClearDetailedServerInfo();
			FillUpDetailedServerInfo();
		}
	}else if (BUTTON_CLICKED == msg)
	{
		if (pWnd == &m_header[1]){
			SetSortFunc("server_name",true);
		}
		else if (pWnd == &m_header[2]){
			SetSortFunc("map",true);
		}
		else if (pWnd == &m_header[3]){
			SetSortFunc("game_type",true);
		}
		else if (pWnd == &m_header[4]){
			SetSortFunc("player",true);
		}
		else if (pWnd == &m_header[5]){
			SetSortFunc("ping",true);
		}
		else if (pWnd == &m_header[6]){
			SetSortFunc("version",true);
		}
	}else if( EDIT_TEXT_COMMIT == msg && pWnd==&m_edit_gs_filter)
	{
		RefreshGameSpyList(m_b_local);
	}else if (MESSAGE_BOX_YES_CLICKED == msg)
	{

		int sel = m_list[LST_SERVER].GetSelectedItem();
		if (-1 == sel)
			return;

		CUIListItemServer* item		= smart_cast<CUIListItemServer*>(m_list[LST_SERVER].GetItem(sel));
		xr_string					command;

		item->CreateConsoleCommand	(command, m_playerName.c_str(), m_message_box->m_pMessageBox->GetUserPassword(), m_message_box->GetPassword() );
		Console->Execute			(command.c_str());
	}
}

void CServerList::BeforeAppear()
{
    UpdateSizes				();
	UpdateVisibility		();
}

void CServerList::AfterAppear()
{
	FillUpDetailedServerInfo();
	UpdateVisibility		();
}

void CServerList::BeforeDisapear()
{
	ClearDetailedServerInfo	();
	UpdateVisibility		();
}

void CServerList::AfterDisappear()
{
	UpdateSizes				();
	UpdateVisibility		();
}

void CServerList::FillUpDetailedServerInfo()
{	
	int si = m_list[LST_SERVER].GetSelectedItem();

		
    if (-1 != si)
	{
		CUIListItemServer* pItem = (CUIListItemServer*)m_list[LST_SERVER].GetItem(si);
		R_ASSERT					(pItem);

	
	}
	else
		ClearDetailedServerInfo		();
}

void CServerList::ClearDetailedServerInfo()
{
	m_list[LST_SRV_PROP].RemoveAll	();
	m_list[LST_PLAYERS].RemoveAll	();
}

void CServerList::ShowServerInfo()
{
	if (!m_bShowServerInfo && NeedToRefreshCurServer())
	{
		RefreshQuick();
	}
	m_bShowServerInfo				= !m_bShowServerInfo;
	m_bAnimation					= true;
	m_pAnimation->Reverese			(!m_bShowServerInfo);
	m_pAnimation->Reset				();
	if (m_bShowServerInfo)
		BeforeAppear				();
	else
		BeforeDisapear				();
}

void CServerList::UpdateSizes()
{
	m_list[LST_SERVER].SetHeight	(m_bShowServerInfo? m_fListH[1]:m_fListH[0]);
	m_frame[LST_SERVER].SetHeight	(m_bShowServerInfo? m_fListH[1]:m_fListH[0]);
	Fvector2 pos					= m_edit_gs_filter.GetWndPos();
	pos.y							= m_bShowServerInfo?m_fEditPos[1]:m_fEditPos[0];
	m_edit_gs_filter.SetWndPos		(pos);
}

void CServerList::UpdateVisibility()
{
	m_list[LST_SRV_PROP].Show		(m_bShowServerInfo ? !m_bAnimation : false);
	m_list[LST_PLAYERS].Show		(m_bShowServerInfo ? !m_bAnimation: false);
	m_frame[LST_SRV_PROP].Show		(m_bShowServerInfo ? true : m_bAnimation);
	m_frame[LST_PLAYERS].Show		(m_bShowServerInfo ? true : m_bAnimation);

	for (int i = 0; i< 4; i++)
		m_header2[i].Show(m_bShowServerInfo ? true : m_bAnimation);
}

void CServerList::SetFilters(SServerFilters& sf)
{
	m_sf = sf;
	RefreshList();
}

void CServerList::SetPlayerName(LPCSTR name)
{
	m_playerName = name;
}

bool CServerList::IsValidItem(ServerInfo& item)
{
	bool result = false;

	

	return result;
}

void CServerList::InitFromXml(CUIXml& xml_doc, LPCSTR path)
{
	CUIXmlInit::InitWindow			(xml_doc, path, 0, this);
	string256 buf;
	CUIXmlInit::InitListWnd			(xml_doc, strconcat(sizeof(buf),buf,path,":list"),							0, &m_list[LST_SERVER]);
	m_fListH[0] =					m_list[LST_SERVER].GetHeight();
	m_fListH[1] =					xml_doc.ReadAttribFlt(buf,0,"height2");
	CUIXmlInit::InitListWnd			(xml_doc, strconcat(sizeof(buf),buf,path,":list_server_properties"),		0, &m_list[LST_SRV_PROP]);
	CUIXmlInit::InitListWnd			(xml_doc, strconcat(sizeof(buf),buf,path,":list_players_list"),				0, &m_list[LST_PLAYERS]);
	CUIXmlInit::InitFrameWindow		(xml_doc, strconcat(sizeof(buf),buf,path,":frame"),							0, &m_frame[LST_SERVER]);
	CUIXmlInit::InitFrameWindow		(xml_doc, strconcat(sizeof(buf),buf,path,":list_server_properties:frame"),	0, &m_frame[LST_SRV_PROP]);
	CUIXmlInit::InitFrameWindow		(xml_doc, strconcat(sizeof(buf),buf,path,":list_players_list:frame"),		0, &m_frame[LST_PLAYERS]);
	CUIXmlInit::InitFont			(xml_doc, strconcat(sizeof(buf),buf,path,":list_item:text"),				0, m_itemInfo.color, m_itemInfo.font);
	CUIXmlInit::InitEditBox			(xml_doc, strconcat(sizeof(buf),buf,path,":edit_gs_filter"),				0, &m_edit_gs_filter);
	m_fEditPos[0] =					m_edit_gs_filter.GetWndPos().y;
	m_fEditPos[1] =					xml_doc.ReadAttribFlt(buf,0,"y2");
	CUIXmlInit::InitLabel			(xml_doc, strconcat(sizeof(buf),buf,path,":cap_server_properties"),			0, &m_header2[0]);
	CUIXmlInit::InitLabel			(xml_doc, strconcat(sizeof(buf),buf,path,":cap_players_list"),				0, &m_header2[1]);
	CUIXmlInit::InitLabel			(xml_doc, strconcat(sizeof(buf),buf,path,":cap_frags"),						0, &m_header2[2]);
	CUIXmlInit::InitLabel			(xml_doc, strconcat(sizeof(buf),buf,path,":cap_death"),						0, &m_header2[3]);
	
	m_itemInfo.size.icon			= xml_doc.ReadAttribFlt( strconcat(sizeof(buf),buf, path, ":sizes"), 0, "icon");
	m_itemInfo.size.server			= xml_doc.ReadAttribFlt( buf, 0, "server");
	m_itemInfo.size.map				= xml_doc.ReadAttribFlt( buf, 0, "map");
	m_itemInfo.size.game			= xml_doc.ReadAttribFlt( buf, 0, "game");
	m_itemInfo.size.players			= xml_doc.ReadAttribFlt( buf, 0, "players");
	m_itemInfo.size.ping			= xml_doc.ReadAttribFlt( buf, 0, "ping");
	m_itemInfo.size.version			= xml_doc.ReadAttribFlt( buf, 0, "version");

	// init header elements
	for (int i = 0; i<LST_COLUMN_COUNT; i++)
	{
		CUIXmlInit::Init3tButton	(xml_doc,strconcat(sizeof(buf),buf,path,":header"),			0, &m_header[i]);
		CUIXmlInit::InitFrameLine	(xml_doc,strconcat(sizeof(buf),buf,path,":header_frames"),	0, &m_header_frames[i]);
	}
	m_header[0].Enable				(false);
	InitHeader						();
	UpdateSizes						();
	UpdateVisibility				();
}

void CServerList::ConnectToSelected()
{
	int sel = m_list[LST_SERVER].GetSelectedItem();
	if (-1 == sel)
		return;

}

void CServerList::InitHeader()
{
	Fvector2				pos;
	pos.set					(0,0);

	m_header[0].SetWidth	(m_itemInfo.size.icon);
	pos.x					+= m_itemInfo.size.icon;
	m_header[1].SetWidth	(m_itemInfo.size.server);
	m_header[1].SetWndPos	(pos);
	m_header[1].SetTextST	("server name");
	pos.x					+= m_itemInfo.size.server;
	m_header[2].SetWidth	(m_itemInfo.size.map);
	m_header[2].SetWndPos	(pos);
	m_header[2].SetTextST	("map");
	pos.x					+= m_itemInfo.size.map;
	m_header[3].SetWidth	(m_itemInfo.size.game);
	m_header[3].SetWndPos	(pos);
	m_header[3].SetTextST	("game type");
	pos.x					+= m_itemInfo.size.game;
	m_header[4].SetWidth	(m_itemInfo.size.players);
	m_header[4].SetWndPos	(pos);
	m_header[4].SetTextST	("players");
	pos.x					+= m_itemInfo.size.players;
	m_header[5].SetWidth	(m_itemInfo.size.ping);
	m_header[5].SetWndPos	(pos);
	m_header[5].SetTextST	("ping");
	pos.x					+= m_itemInfo.size.ping;
	m_header[6].SetWidth	(m_itemInfo.size.version);
	m_header[6].SetWndPos	(pos);
	m_header[6].SetTextST	("version");

	for(int i=0; i<LST_COLUMN_COUNT;++i)
	{
		m_header_frames[i].SetWndPos	(m_header[i].GetWndPos());
		m_header_frames[i].SetWidth		(m_header[i].GetWndSize().x);
		m_header_frames[i].Enable		(true);
		m_header_frames[i].Show			(true);
		m_header_frames[i].SetVisible	(true);
	}
}

void CServerList::NetRadioChanged(bool Local)
{
	m_edit_gs_filter.Enable	(!Local);
	m_b_local				= Local;
}

void CServerList::RefreshGameSpyList(bool Local)
{
	SetSortFunc			("",		false);
	SetSortFunc			("ping",	false);
	

	ResetCurItem		();
	RefreshList			();
}


void CServerList::AddServerToList	(ServerInfo* pServerInfo)
{
	if (!IsValidItem(*pServerInfo)) return;
	CUIListWnd& _list			= m_list[LST_SERVER];
	
	CUIListItemServer* item		= GetFreeItem();

	float w						= _list.GetItemWidth();
	float h						= _list.GetItemHeight();
	SrvInfo2LstSrvInfo			(pServerInfo);
	item->Init					(m_itemInfo, 0, 0, w, h);

	_list.AddItem<CUIListItemServer>(item);
};

void	CServerList::UpdateServerInList(ServerInfo* pServerInfo, int index)
{
	int sz = m_list[LST_SERVER].GetItemsCount();

	for (int i = 0; i< sz; i++)
	{
		CUIListItemServer* pItem = static_cast<CUIListItemServer*>(m_list[LST_SERVER].GetItem(i));
		if (pItem->Get_gs_index() == index)
		{
			UpdateServerInList(pServerInfo, pItem);
			return;
		}
	}

    R_ASSERT2(false, "CServerList::UpdateServerInList - invalid index");

};

void	CServerList::UpdateServerInList(ServerInfo* pServerInfo, CUIListItemServer* pItem)
{
	SrvInfo2LstSrvInfo(pServerInfo);
	pItem->SetParams(m_itemInfo);
};

void	CServerList::RefreshList()
{
	m_need_refresh_fr	= Device.dwFrame;
}

void	CServerList::RefreshList_internal()
{
	m_need_refresh_fr				= u32(-1);
	SaveCurItem						();
	m_list[LST_SERVER].RemoveAll	();
	ClearSrvItems					();
	
	return;
};

void CServerList::RefreshQuick()
{
	int SvId = m_list[LST_SERVER].GetSelectedItem();
	if (-1 == SvId)
		return;

}

bool g_bSort_Ascending = true;
void CServerList::SetSortFunc(LPCSTR func_name, bool make_sort)
{
	if (!xr_strcmp(m_sort_func, func_name))
	{
		g_bSort_Ascending = !g_bSort_Ascending;
	}
	else
		g_bSort_Ascending = true;

	m_sort_func = func_name;

	if (make_sort)
		RefreshList();
}

void CServerList::SrvInfo2LstSrvInfo(const ServerInfo* pServerInfo)
{
	
}

bool CServerList::sort_by_ServerName(int p1, int p2)
{
	return false;
}

bool CServerList::sort_by_Map(int p1, int p2)
{
	return false;
}

bool CServerList::sort_by_GameType(int p1, int p2)
{
	return false;
}

bool CServerList::sort_by_Players(int p1, int p2)
{
	return false;
}

bool CServerList::sort_by_Ping(int p1, int p2)
{
	return false;
}

bool CServerList::sort_by_Version(int p1, int p2)
{
	return false;
}

void CServerList::SaveCurItem()
{
	int SvId = m_list[LST_SERVER].GetSelectedItem();
	if (-1 == SvId){
		m_cur_item = -1;
		return;
	}

	CUIListItemServer* pItem = (CUIListItemServer*)m_list[LST_SERVER].GetItem(SvId);
	R_ASSERT(pItem);

	m_cur_item = pItem->GetInfo()->info.Index;
}

void CServerList::RestoreCurItem()
{
	if (-1 == m_cur_item)		return;

	int index = m_list[LST_SERVER].FindItemWithValue(m_cur_item);

	m_list[LST_SERVER].SetSelectedItem(index);	
	m_list[LST_SERVER].ScrollToPos(index);
}

void CServerList::ResetCurItem()
{
	m_list[LST_SERVER].SetSelectedItem(-1);
	m_list[LST_SERVER].ScrollToBegin();
}


void CServerList::DestroySrvItems()
{
	m_last_retreived_index	= u32(-1);

	m_list[LST_SERVER].RemoveAll				();
	SrvItems_It it		= m_items_cache.begin	();
	SrvItems_It it_e	= m_items_cache.end		();

	for(;it!=it_e;++it)
		xr_delete		( (*it).m_ui_item );
}

void CServerList::ClearSrvItems()
{
	SrvItems_It it			= m_items_cache.begin();
	SrvItems_It it_e		= m_items_cache.end();
	for(;it!=it_e;++it)
		(*it).m_busy = false;

	m_last_retreived_index	= u32(-1);
}

CUIListItemServer* CServerList::GetFreeItem()
{
	SrvItems_It it			= m_items_cache.begin();
	SrvItems_It it_e		= m_items_cache.end();
	
	if(m_last_retreived_index != u32(-1))
		std::advance		(it, m_last_retreived_index);

	for(;it!=it_e;++it)
	{
		if( false==(*it).m_busy )
		{
			(*it).m_busy			= true;
			m_last_retreived_index	= (u32)(it-m_items_cache.begin());
			return					(*it).m_ui_item;
		}
	}
	m_items_cache.resize	(m_items_cache.size()+1);
	SrvItem& Res			= m_items_cache.back();
	Res.m_ui_item			= xr_new<CUIListItemServer>();
	Res.m_busy				= true;
	m_last_retreived_index	= m_items_cache.size()-1; 
    return					Res.m_ui_item;
}
