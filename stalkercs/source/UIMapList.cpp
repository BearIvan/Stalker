#include "StdAfx.h"
#include "UI/UIMapList.h"
#include "UI/UIListBox.h"
#include "UI/UIFrameWindow.h"
#include "UI/UIFrameLineWnd.h"
#include "UI/UI3tButton.h"
#include "UI/UISpinText.h"
#include "UI/UIXmlInit.h"
#include "UI/UIMapInfo.h"
#include "UI/UIComboBox.h"
#include "UI/UIListBoxItem.h"
#include "engine/xr_ioconsole.h"
#include "string_table.h"
#include "UI/CExtraContentFilter.h"

#include "engine/object_broker.h"
#include "game_base.h"

extern ENGINE_API string512		g_sLaunchOnExit_app;
extern ENGINE_API string512		g_sLaunchOnExit_params;
extern ENGINE_API string_path	g_sLaunchWorkingFolder;

LPCSTR GameTypeToString(EGameIDs gt, bool bShort);

extern	void	GetPlayerName_FromRegistry	(char* name, u32 const name_size);

CUIMapList::CUIMapList(){
	m_pMapInfo		= NULL;
	m_pMapPic		= NULL;
	m_pModeSelector	= NULL;
	m_pWeatherSelector=NULL;
	m_pList1		= xr_new<CUIListBox>();
	m_pList2		= xr_new<CUIListBox>();
	m_pFrame1		= xr_new<CUIFrameWindow>();
	m_pFrame2		= xr_new<CUIFrameWindow>();
	m_pLbl1			= xr_new<CUIFrameLineWnd>();
	m_pLbl2			= xr_new<CUIFrameLineWnd>();
	m_pBtnLeft		= xr_new<CUI3tButtonEx>();
	m_pBtnRight		= xr_new<CUI3tButtonEx>();
	m_pBtnUp		= xr_new<CUI3tButtonEx>();
	m_pBtnDown		= xr_new<CUI3tButtonEx>();

	m_pExtraContentFilter = xr_new<CExtraContentFilter>();

	m_pList1->SetAutoDelete(true);
	m_pList2->SetAutoDelete(true);
	m_pFrame1->SetAutoDelete(true);
	m_pFrame2->SetAutoDelete(true);
	m_pLbl1->SetAutoDelete(true);
	m_pLbl2->SetAutoDelete(true);
	m_pBtnLeft->SetAutoDelete(true);
	m_pBtnRight->SetAutoDelete(true);
	m_pBtnUp->SetAutoDelete(true);
	m_pBtnDown->SetAutoDelete(true);

	AttachChild(m_pLbl1);
	AttachChild(m_pLbl2);
	AttachChild(m_pFrame1);
	AttachChild(m_pFrame2);
	AttachChild(m_pList1);
	AttachChild(m_pList2);
	AttachChild(m_pBtnLeft);
	AttachChild(m_pBtnRight);
	AttachChild(m_pBtnUp);
	AttachChild(m_pBtnDown);
}

CUIMapList::~CUIMapList(){
	delete_data(m_pExtraContentFilter);
}

void CUIMapList::StartDedicatedServer(){

	string_path ModuleFileName;		
	GetModuleFileName(NULL, ModuleFileName, sizeof(ModuleFileName));

	char* ModuleName	= NULL;
	GetFullPathName		(ModuleFileName, sizeof(g_sLaunchWorkingFolder), g_sLaunchWorkingFolder, &ModuleName);
	//removing module name from WorkingDirectory that contain full path...
	ModuleName[0]		= 0;
			
	BearCore::BearString::Copy			(g_sLaunchOnExit_app, g_sLaunchWorkingFolder);
	strcat_s			(g_sLaunchOnExit_app, "dedicated\\xrEngine.exe");

	BearCore::BearString::Copy			(g_sLaunchOnExit_params, g_sLaunchOnExit_app);
	strcat_s			(g_sLaunchOnExit_params, " -i -fsltx ..\\fsgame.ltx -nosound -");
	strcat_s			(g_sLaunchOnExit_params, GetCommandLine(""));
	Msg					("Going to quit before starting dedicated server");
	Msg					("Working folder is:%s", g_sLaunchWorkingFolder);
	Msg					("%s %s",g_sLaunchOnExit_app, g_sLaunchOnExit_params);
	Console->Execute	("quit");
}


void CUIMapList::SendMessage(CUIWindow* pWnd, s16 msg, void* pData ){
	if (BUTTON_CLICKED == msg)
	{
		if (m_pBtnLeft == pWnd)
			OnBtnLeftClick();
		else if (m_pBtnRight == pWnd)
			OnBtnRightClick();
		else if (m_pBtnUp == pWnd)
			OnBtnUpClick();
		else if (m_pBtnDown == pWnd)
			OnBtnDownClick();
		else if (m_pModeSelector == pWnd)
			OnModeChange();
	}
	else if (WINDOW_LBUTTON_DB_CLICK == msg)
	{
		if (m_pList1 ==pWnd)
			OnBtnRightClick();
		else if (m_pList2 ==pWnd)
			OnBtnLeftClick();
	}
	else if (LIST_ITEM_CLICKED == msg)
	{
		if (pWnd == m_pList1)
            OnListItemClicked();
	}

		
}

void CUIMapList::OnListItemClicked()
{
	xr_string map_name = "intro\\intro_map_pic_";
	
	CUIListBoxItem* itm				= m_pList1->GetSelectedItem();
	u32 _idx						= (u32)(__int64)(itm->GetData());
	const SGameTypeMaps::SMapItm& M	= GetMapNameInt(GetCurGameType(), _idx);

	map_name						+=	M.map_name.c_str();
	xr_string full_name				= map_name + ".dds";

	Frect orig_rect = m_pMapPic->GetOriginalRect();

	if (FS.ExistFile("%textures%",full_name.c_str()))
		m_pMapPic->InitTexture		(map_name.c_str());
	else
		m_pMapPic->InitTexture		("ui\\ui_noise");

	m_pMapPic->SetOriginalRect(orig_rect);

	m_pMapInfo->InitMap				(M.map_name.c_str(), M.map_ver.c_str());
}

xr_token g_GameModes[];

void CUIMapList::OnModeChange()
{
	UpdateMapList(GetCurGameType());
}

EGameIDs CUIMapList::GetCurGameType()
{
	LPCSTR text = m_pModeSelector->GetTokenText();

	if (0 == xr_strcmp(text, get_token_name(g_GameModes,eGameIDDeathmatch)) )
		return	eGameIDDeathmatch;
	else if (0 == xr_strcmp(text, get_token_name(g_GameModes,eGameIDTeamDeathmatch)) )
		return	eGameIDTeamDeathmatch;
	else if (0 == xr_strcmp(text, get_token_name(g_GameModes,eGameIDArtefactHunt)) )
		return	eGameIDArtefactHunt;
	else if (0 == xr_strcmp(text, get_token_name(g_GameModes,eGameIDCaptureTheArtefact)) )
		return	eGameIDCaptureTheArtefact;
	else
		NODEFAULT;

#ifdef DEBUG
	return EGameIDs(u32(-1));
#endif
}

const char* CUIMapList::GetCommandLine(LPCSTR player_name){

	CUIListBoxItem* itm				= m_pList2->GetItemByIDX(0);
	if (!itm)	
		return						NULL;

	u32 _idx						= (u32)(__int64)(itm->GetData());
	const SGameTypeMaps::SMapItm& M	= GetMapNameInt	(GetCurGameType(), _idx);

	m_command.clear();
	m_command = "start server(";
	m_command += M.map_name.c_str();
	m_command += "/";
	m_command += GameTypeToString(GetCurGameType(),true);
	m_command += m_srv_params;
	m_command += "/ver=";
	m_command += M.map_ver.c_str();
	m_command += "/estime=";
	
	u32 id		= m_pWeatherSelector->m_list_box.GetSelectedItem()->GetTAG();

	m_command	+= m_mapWeather[id].weather_time.c_str();
	m_command	+= ")";


	m_command	+= " client(localhost/name=";
	if (player_name == NULL || 0 == xr_strlen(player_name))
	{
		string64	player_name2;
		GetPlayerName_FromRegistry( player_name2, sizeof(player_name2) );

		if ( xr_strlen(player_name2) == 0 )
		{
			BearCore::BearString::Copy( player_name2, xr_strlen(XrCore::UserName) ? XrCore::UserName : XrCore::CompName );
		}
		VERIFY( xr_strlen(player_name2) );
		
		m_command += player_name2;
	}
	else
	{
		m_command += player_name;
	}
	m_command	  += ")";

    return m_command.c_str();
}
#include "UIGameCustom.h"
void CUIMapList::LoadMapList()
{

	GAME_WEATHERS game_weathers = gMapListHelper.GetGameWeathers();
	GAME_WEATHERS_CIT it		= game_weathers.begin();
	GAME_WEATHERS_CIT it_e		= game_weathers.end();
	
	u32 cnt=0;
	for( ;it!=it_e; ++it, ++cnt)
	{
		AddWeather			( (*it).m_weather_name, (*it).m_start_time, cnt);
	}
	if( game_weathers.size() )
		m_pWeatherSelector->SetItem(0);
}

void	CUIMapList::SaveMapList()
{
	string_path					temp;


	if(m_pList2->GetSize()<=1){
		FS.Delete("%user%", MAP_ROTATION_LIST);
		return;
	}

	IWriter*	pW =XRayBearWriter::Create( FS.Write	("%user%", MAP_ROTATION_LIST,0));
	if (!pW){
		Msg("! Cant create map rotation file [%s]", temp);
		return;
	}
	
	string_path					map_name;
	for(u32 idx=0; idx<m_pList2->GetSize(); ++idx)
	{
		CUIListBoxItem* itm				= m_pList2->GetItemByIDX(idx);
		u32 _idx						= (u32)(__int64)(itm->GetData());
		const SGameTypeMaps::SMapItm& M	= GetMapNameInt(GetCurGameType(), _idx);

		sprintf_s						(map_name, "sv_addmap %s/ver=%s", M.map_name.c_str(), M.map_ver.c_str() );
		pW->w_string					(map_name);
	}

	XRayBearWriter::Destroy(pW);
}

void CUIMapList::SetWeatherSelector(CUIComboBox* ws){
	m_pWeatherSelector = ws;
}

void CUIMapList::SetModeSelector(CUISpinText* ms){
	m_pModeSelector = ms;
}

void CUIMapList::SetMapPic(CUIStatic* map_pic){
	m_pMapPic = map_pic;
}

void CUIMapList::SetMapInfo(CUIMapInfo* map_info){
	m_pMapInfo = map_info;	
}

void CUIMapList::SetServerParams(LPCSTR params){
	m_srv_params = params;
}

#include "UI/UIlistboxitem.h"
void CUIMapList::AddWeather(const shared_str& WeatherType, const shared_str& WeatherTime, u32 _id)
{
	R_ASSERT2					(m_pWeatherSelector, "m_pWeatherSelector == NULL");
	m_pWeatherSelector->AddItem_	(*WeatherType, 0)->SetTAG(_id);

	m_mapWeather.resize(m_mapWeather.size()+1);
	m_mapWeather.back().weather_name = WeatherType;
	m_mapWeather.back().weather_time = WeatherTime;
}

void CUIMapList::InitFromXml(CUIXml& xml_doc, const char* path){
	CUIXmlInit::InitWindow(xml_doc, path, 0, this);
	string256 buf;
	CUIXmlInit::InitFrameLine	(xml_doc, strconcat(sizeof(buf),buf, path, ":header_1"),	0, m_pLbl1);
	CUIXmlInit::InitFrameLine	(xml_doc, strconcat(sizeof(buf),buf, path, ":header_2"),	0, m_pLbl2);
	CUIXmlInit::InitFrameWindow	(xml_doc, strconcat(sizeof(buf),buf, path, ":frame_1"),		0, m_pFrame1);
	CUIXmlInit::InitFrameWindow	(xml_doc, strconcat(sizeof(buf),buf, path, ":frame_2"),		0, m_pFrame2);
	CUIXmlInit::InitListBox		(xml_doc, strconcat(sizeof(buf),buf, path, ":list_1"),		0, m_pList1);
	CUIXmlInit::InitListBox		(xml_doc, strconcat(sizeof(buf),buf, path, ":list_2"),		0, m_pList2);
	CUIXmlInit::Init3tButtonEx	(xml_doc, strconcat(sizeof(buf),buf, path, ":btn_left"),	0, m_pBtnLeft);
	CUIXmlInit::Init3tButtonEx	(xml_doc, strconcat(sizeof(buf),buf, path, ":btn_right"),	0, m_pBtnRight);
	CUIXmlInit::Init3tButtonEx	(xml_doc, strconcat(sizeof(buf),buf, path, ":btn_up"),		0, m_pBtnUp);
	CUIXmlInit::Init3tButtonEx	(xml_doc, strconcat(sizeof(buf),buf, path, ":btn_down"),	0, m_pBtnDown);
}

void CUIMapList::UpdateMapList(EGameIDs GameType)
{
	typedef buffer_vector<shared_str>	MapList;

	m_pList1->Clear				();

	const SGameTypeMaps& M		= gMapListHelper.GetMapListFor(GameType);
	u32 cnt						= M.m_map_names.size();
	for (u32 i=0; i<cnt; ++i)
	{
		CUIListBoxItem* itm		= m_pList1->AddItem( CStringTable().translate(M.m_map_names[i].map_name).c_str() );
		itm->SetData			( (void*)(__int64)i );
		itm->Enable				(true);//m_pExtraContentFilter->IsDataEnabled(M.m_map_names[i].map_name.c_str()));
	}
	
	u32 list_size				= m_pList2->GetSize();
	if ( list_size == 0 )
	{
		m_pList2->Clear			();
		return;
	}

	MapList		map_list( _alloca( sizeof(shared_str) * list_size ), list_size );

	for ( u32 i = 0; i < list_size; ++i )
	{
		LPCSTR st = m_pList2->GetText( i );
		map_list.push_back( st );
	}
	m_pList2->Clear();

	MapList::const_iterator itb = map_list.begin();
	MapList::const_iterator ite = map_list.end();
	for ( ; itb != ite; ++itb )
	{
		CUIListBoxItem* itm1 = GetMapItem_fromList1( *itb );
		if ( itm1 )
		{
			CUIListBoxItem* itm2 = m_pList2->AddItem( (*itb).c_str() );
			itm2->SetData( itm1->GetData() );
			itm2->Enable( true );
		}
	}
}

CUIListBoxItem* CUIMapList::GetMapItem_fromList1( shared_str const& map_name )
{
	shared_str map_name1;
	for ( u32 i = 0; i < m_pList1->GetSize(); ++i )
	{
		map_name1._set( m_pList1->GetText( i ) );
		if ( map_name1 == map_name )
		{
			return smart_cast<CUIListBoxItem*>( m_pList1->GetItem(i) );
		}
	}
	return NULL;
}

void CUIMapList::ClearList()
{
	m_pList1->Clear();
	m_pList2->Clear();
}	

void CUIMapList::OnBtnLeftClick(){
	m_pList2->RemoveWindow(m_pList2->GetSelected());
}

void CUIMapList::Update(){
	CUIWindow::Update();
}

void CUIMapList::OnBtnRightClick()
{
	CUIListBoxItem* itm1			= m_pList1->GetSelectedItem();
	if (!itm1) return;
	CUIListBoxItem* itm2			= m_pList2->AddItem( itm1->m_text.GetText() );
	itm2->SetData					(itm1->GetData());
}

void CUIMapList::OnBtnUpClick()
{
	m_pList2->MoveSelectedUp();
}

void CUIMapList::OnBtnDownClick()
{
	m_pList2->MoveSelectedDown();
}

bool CUIMapList::IsEmpty()
{
	return 0 == m_pList2->GetSize();
}

const SGameTypeMaps::SMapItm& CUIMapList::GetMapNameInt(EGameIDs _type, u32 idx)
{
	const SGameTypeMaps& M		= gMapListHelper.GetMapListFor(_type);
	R_ASSERT					(M.m_map_names.size()>idx);
	return						M.m_map_names[idx];
}
