////////////////////////////////////////////////////////////////////////////
//	Module 		: UIHelper.cpp
//	Created 	: 17.01.2008
//	Author		: Evgeniy Sokolov
//	Description : UI Helper class implementation
////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "UI/UIHelper.h"
#include "UI/UIXmlInit.h"

#include "UI/UIStatic.h"
#include "UI/UIProgressBar.h"
#include "UI/UIFrameLineWnd.h"
#include "UI/UIFrameWindow.h"
#include "UI/UI3tButton.h"
#include "UI/UICheckButton.h"
#include "UI/UIHint.h"
#include "UI/UIDragDropListEx.h"

#include "UI/UIWndCallback.h"

CUIStatic* UIHelper::CreateStatic( CUIXml& xml, LPCSTR ui_path, CUIWindow* parent )
{
	CUIStatic* ui			= xr_new<CUIStatic>();
	parent->AttachChild		( ui );
	ui->SetAutoDelete		( true );
	CUIXmlInit::InitStatic	( xml, ui_path, 0, ui );
	return ui;
}

CUIProgressBar* UIHelper::CreateProgressBar( CUIXml& xml, LPCSTR ui_path, CUIWindow* parent )
{
	CUIProgressBar* ui			= xr_new<CUIProgressBar>();
	parent->AttachChild			( ui );
	ui->SetAutoDelete			( true );
	CUIXmlInit::InitProgressBar ( xml, ui_path, 0, ui );
	return ui;
}

CUIFrameLineWnd* UIHelper::CreateFrameLine( CUIXml& xml, LPCSTR ui_path, CUIWindow* parent )
{
	CUIFrameLineWnd* ui			= xr_new<CUIFrameLineWnd>();
	parent->AttachChild			( ui );
	ui->SetAutoDelete			( true );
	CUIXmlInit::InitFrameLine	( xml, ui_path, 0, ui );
	return ui;
}

CUIFrameWindow* UIHelper::CreateFrameWindow( CUIXml& xml, LPCSTR ui_path, CUIWindow* parent )
{
	CUIFrameWindow* ui			= xr_new<CUIFrameWindow>();
	parent->AttachChild			( ui );
	ui->SetAutoDelete			( true );
	CUIXmlInit::InitFrameWindow	( xml, ui_path, 0, ui );
	return ui;
}

CUI3tButton* UIHelper::Create3tButton( CUIXml& xml, LPCSTR ui_path, CUIWindow* parent )
{
	CUI3tButton* ui			= xr_new<CUI3tButton>();
	parent->AttachChild		( ui );
	ui->SetAutoDelete		( true );
	CUIXmlInit::Init3tButton( xml, ui_path, 0, ui );
	return ui;
}

CUI3tButtonEx* UIHelper::Create3tButtonEx( CUIXml& xml, LPCSTR ui_path, CUIWindow* parent )
{
	CUI3tButtonEx* ui		= xr_new<CUI3tButtonEx>();
	parent->AttachChild		( ui );
	ui->SetAutoDelete		( true );
	ui->init_from_xml		( xml, ui_path );
	//CUIXmlInit::Init3tButton( xml, ui_path, 0, ui );
	return ui;
}

CUICheckButton* UIHelper::CreateCheck( CUIXml& xml, LPCSTR ui_path, CUIWindow* parent )
{
	CUICheckButton* ui		= xr_new<CUICheckButton>();
	parent->AttachChild		( ui );
	ui->SetAutoDelete		( true );
	CUIXmlInit::InitCheck	( xml, ui_path, 0, ui );
	return ui;
}

UIHint* UIHelper::CreateHint( CUIXml& xml, LPCSTR ui_path /*, CUIWindow* parent*/ )
{
	UIHint* ui				= xr_new<UIHint>();
//	parent->AttachChild		( ui );
	ui->SetAutoDelete		( true );
	ui->init_from_xml		( xml, ui_path );
	return ui;
}

CUIDragDropListEx* UIHelper::CreateDragDropListEx( CUIXml& xml, LPCSTR ui_path, CUIWindow* parent )
{
	CUIDragDropListEx* ui			= xr_new<CUIDragDropListEx>();
	parent->AttachChild				( ui );
	ui->SetAutoDelete				( true );
	CUIXmlInit::InitDragDropListEx	( xml, ui_path, 0, ui );
	return ui;
}
