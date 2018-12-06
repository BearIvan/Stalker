#include "StdAfx.h"
#include "ui/UICellItemFactory.h"
#include "ui/UICellCustomItems.h"



CUICellItem*	create_cell_item(CInventoryItem* itm)
{

	CWeaponAmmo* pAmmo					= smart_cast<CWeaponAmmo*>(itm);
	if(pAmmo)							return xr_new<CUIAmmoCellItem>(pAmmo);

	CWeapon* pWeapon					= smart_cast<CWeapon*>(itm);
	if(pWeapon)							return xr_new<CUIWeaponCellItem>(pWeapon);

	return xr_new<CUIInventoryCellItem>(itm);
}
