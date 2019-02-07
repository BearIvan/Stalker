#pragma once
#include "uigamecustom.h"
#include "ui/UIDialogWnd.h"
#include "tools/net_utils.h"
#include "game_graph_space.h"

class CUIInventoryWnd;
class CUITradeWnd;			
class CUIPdaWnd;			
class CUITalkWnd;			
class CUICarBodyWnd;
class CInventory;

class game_cl_Coop;
class CChangeLevelWnd;
class CUIMessageBox;
class CInventoryBox;
class CInventoryOwner;

class CUIGameCP : public CUIGameCustom
{
private:
	game_cl_Coop*		m_game;
	typedef CUIGameCustom inherited;
public:
	CUIGameCP									();
	virtual				~CUIGameCP				();

	virtual	void		reset_ui				();
	virtual	void		shedule_Update			(u32 dt);
	virtual void		SetClGame				(game_cl_GameState* g);
	virtual bool		IR_OnKeyboardPress		(int dik);
	virtual bool		IR_OnKeyboardRelease	(int dik);

	void				StartTalk				();
	void				StartCarBody			(CInventoryOwner* pOurInv, CInventoryOwner* pOthers);
	void				StartCarBody			(CInventoryOwner* pOurInv, CInventoryBox* pBox);
	virtual void		ReInitShownUI			();
	void				ChangeLevel				(GameGraph::_GRAPH_ID game_vert_id, u32 level_vert_id, Fvector pos, Fvector ang, Fvector pos2, Fvector ang2, bool b);

	virtual void		HideShownDialogs		();

	CUIInventoryWnd*	InventoryMenu;
	CUIPdaWnd*			PdaMenu;
	CUITalkWnd*			TalkMenu;
	CUICarBodyWnd*		UICarBodyMenu;
	CChangeLevelWnd*	UIChangeLevelWnd;
};
