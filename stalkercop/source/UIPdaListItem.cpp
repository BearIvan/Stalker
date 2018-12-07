#include "stdafx.h"

#include "ui\uiPdaListItem.h"
#include "actor.h"
#include "ui\uiInventoryUtilities.h"
#include "string_table.h"

#include "ui/xrUIXmlParser.h"
#include "ui\uiXmlInit.h"

#include "character_info.h"

#include "ui\uiFrameWindow.h"
#include "InventoryOwner.h"
#include "ui\uiCharacterInfo.h"
#include "ui\uiStatic.h"

#define			PDA_CONTACT_CHAR		"pda_character.xml"

CUIPdaListItem::CUIPdaListItem()
{
	UIInfo = NULL;
}

CUIPdaListItem::~CUIPdaListItem()
{
}

void CUIPdaListItem::InitPdaListItem(Fvector2 pos, Fvector2 size)
{
	inherited::SetWndPos						(pos);
	inherited::SetWndSize						(size);

	CUIXml										uiXml;
	uiXml.Load									(CONFIG_PATH, UI_PATH, PDA_CONTACT_CHAR);

	CUIXmlInit xml_init;
	UIInfo = xr_new<CUICharacterInfo>			();
	UIInfo->SetAutoDelete						(true);
	AttachChild									(UIInfo);
	UIInfo->InitCharacterInfo					(Fvector2().set(0,0), size, PDA_CONTACT_CHAR);

	xml_init.InitAutoStaticGroup				(uiXml,"pda_char_auto_statics", 0, this);
}

void CUIPdaListItem::InitCharacter(CInventoryOwner* pInvOwner)
{
	VERIFY										(pInvOwner);
	UIInfo->InitCharacter						(pInvOwner->object_id());
}
