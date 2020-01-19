#include "game_cl_single.h"
#include "UIGameSP.h"
#include "clsid_game.h"
#include "actor.h"

using namespace luabind;

template<>
inline void CScriptGameDifficulty::script_register(lua_State *L)
{
	module(L)
		[
			class_<enum_exporter<ESingleGameDifficulty> >("game_difficulty")
			.enum_("game_difficulty")
			[
				value("novice",				int(egdNovice			)),
				value("stalker",			int(egdStalker			)),
				value("veteran",			int(egdVeteran			)),
				value("master",				int(egdMaster			))
			]
		];
}