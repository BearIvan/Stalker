#include "StdAfx.h"
#include "GameSpy\GameSpy_Patching.h"
#include "GameSpy\GameSpy_Base_Defs.h"
#include "MainMenu.h"

CGameSpy_Patching::CGameSpy_Patching()
{
	m_hGameSpyDLL = NULL;

	LPCSTR			g_name =
#ifdef MIXED
		"stalker_gamespy_mixed.dll"
#elif DEBUG
		"stalker_gamespy_debug.dll"
#else
		"stalker_gamespy.dll"
#endif


		;
	Log				("Loading DLL:",g_name);
	m_hGameSpyDLL			= LoadLibrary	(g_name);
	if (0==m_hGameSpyDLL)	R_CHK			(GetLastError());
	R_ASSERT2		(m_hGameSpyDLL,"GameSpy DLL raised exception during loading or there is no game DLL at all");

	LoadGameSpy(m_hGameSpyDLL);
};
CGameSpy_Patching::CGameSpy_Patching(HMODULE hGameSpyDLL)
{
	m_hGameSpyDLL = NULL;

	LoadGameSpy(hGameSpyDLL);
};
CGameSpy_Patching::~CGameSpy_Patching()
{
	if (m_hGameSpyDLL)
	{
		FreeLibrary(m_hGameSpyDLL);
		m_hGameSpyDLL = NULL;
	}
};
void	CGameSpy_Patching::LoadGameSpy(HMODULE hGameSpyDLL)
{	
	GAMESPY_LOAD_FN(xrGS_ptCheckForPatchA);
}

bool g_bInformUserThatNoPatchFound = true;
void __cdecl GS_ptPatchCallback ( PTBool available, PTBool mandatory, const char * versionName, int fileID, const char * downloadURL,  void * param )
{
	if (!MainMenu()) return;
	if (!available)
	{
		Msg("No new patches are available.");
		if (g_bInformUserThatNoPatchFound)
			MainMenu()->OnNoNewPatchFound();		
		return;
	};
	Msg("Found NewPatch: %s - %s", versionName, downloadURL);
	MainMenu()->OnNewPatchFound(versionName, downloadURL);
};

void	CGameSpy_Patching::CheckForPatch(bool InformOfNoPatch)
{
	g_bInformUserThatNoPatchFound = InformOfNoPatch;
	bool res =  xrGS_ptCheckForPatchA(
		GS_ptPatchCallback,
		PTFalse,
		this
	);	
	if (!res)
	{
		Msg("! Unable to send query for patch!");

	}
};