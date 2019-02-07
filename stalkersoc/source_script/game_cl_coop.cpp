#include "pch_script.h"
#include "coop/game_cl_coop.h"
#include "game_cl_single.h"
#include "UIGameCP.h"
#include "clsid_game.h"
#include "actor.h"
#include "Level.h"
#include "HUDManager.h"
game_cl_Coop::game_cl_Coop()
{

}

CUIGameCustom * game_cl_Coop::createGameUI()
{
	CLASS_ID clsid = CLSID_GAME_UI_COOP;
	CUIGameCP*			pUIGame = smart_cast<CUIGameCP*> (NEW_INSTANCE(clsid));
	R_ASSERT(pUIGame);
	pUIGame->SetClGame(this);
	pUIGame->Init();
	return pUIGame;
}

char * game_cl_Coop::getTeamSection(int Team)
{
	return NULL;
}
