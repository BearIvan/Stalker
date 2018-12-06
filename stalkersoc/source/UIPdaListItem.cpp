#include "StdAfx.h"

#include "ui/UIPdaListItem.h"
#include "actor.h"
#include "ui/UIInventoryUtilities.h"
#include "string_table.h"

#include "ui/xrUIXmlParser.h"
#include "ui/UIXmlInit.h"

#include "character_info.h"

#include "ui/UIFrameWindow.h"
#include "InventoryOwner.h"
#include "ui/UICharacterInfo.h"
#include "ui/UIStatic.h"

#define			PDA_CONTACT_CHAR		"pda_character.xml"

CUIPdaListItem::CUIPdaListItem()
{
	UIMask = NULL;
	UIInfo = NULL;
}

CUIPdaListItem::~CUIPdaListItem()
{
}

void CUIPdaListItem::Init(float x, float y, float width, float height)
{
	inherited::Init								(x, y, width, height);

	CUIXml uiXml;
	uiXml.Load				(CONFIG_PATH, UI_PATH, PDA_CONTACT_CHAR);

	CUIXmlInit xml_init;
	UIInfo = xr_new<CUICharacterInfo>			();
	UIInfo->SetAutoDelete						(true);
	AttachChild									(UIInfo);
	UIInfo->Init								(0, 0, width, height, PDA_CONTACT_CHAR);

	if (uiXml.NavigateToNode					("mask_frame_window", 0))
	{
		UIMask = xr_new<CUIFrameWindow>			();
		UIMask->SetAutoDelete					(true);
		xml_init.InitFrameWindow				(uiXml, "mask_frame_window", 0, UIMask);
		UIInfo->UIIcon().SetMask				(UIMask);
	}

	xml_init.InitAutoStaticGroup				(uiXml,"pda_char_auto_statics", 0, this);
}

void CUIPdaListItem::InitCharacter(CInventoryOwner* pInvOwner)
{
	VERIFY										(pInvOwner);
	UIInfo->InitCharacter						(pInvOwner->object_id());
}
