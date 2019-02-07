#pragma once
#include "game_cl_base.h"

class game_cl_Coop:public game_cl_GameState
{
public :
	game_cl_Coop();
	virtual			CUIGameCustom*		createGameUI			();
	virtual			char*				getTeamSection			(int Team);
	virtual			bool				IsServerControlHits		()	{return true;};

	virtual		ALife::_TIME_ID			GetEnvironmentGameTime	()				{return GetGameTime();};
	virtual		float					GetEnvironmentGameTimeFactor		()	{return GetGameTimeFactor();};
};



