
#pragma once

//new

#include "engine/IGame_Persistent.h"
class PropValue;
class PropItem;
DEFINE_VECTOR			(PropItem*,PropItemVec,PropItemIt);

struct GameTypeChooser
{
    Flags16	m_GameType;
#ifndef XRGAME_EXPORTS
		void	FillProp		(LPCSTR pref, PropItemVec& items);
#endif // #ifndef XRGAME_EXPORTS

#ifdef _EDITOR
	bool 	LoadStream		(IReader&F);
	bool 	LoadLTX			(CInifile& ini, LPCSTR sect_name, bool bOldFormat);
	void 	SaveStream		(IWriter&);
	void 	SaveLTX			(CInifile& ini, LPCSTR sect_name);
#endif
	void	SetDefaults		()				{m_GameType.one();}
	bool	MatchType		(const u16 t) const		{return (t==eGameIDNoGame) || !!m_GameType.test(t);};
};

