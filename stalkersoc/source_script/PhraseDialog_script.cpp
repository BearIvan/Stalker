#include "pch_script.h"
#include "PhraseDialog_script.h"
#include "PhraseDialog.h"

using namespace luabind;

void CPhraseScript::AddPrecondition	(LPCSTR str)
{
	m_Preconditions.push_back	(str);
}
void CPhraseScript::AddAction		(LPCSTR str)
{
	m_ScriptActions.push_back	(str);
}
void CPhraseScript::AddHasInfo		(LPCSTR str)
{
	m_HasInfo.push_back	(str);
}
void CPhraseScript::AddDontHasInfo	(LPCSTR str)
{
	m_DontHasInfo.push_back	(str);
}
void CPhraseScript::AddGiveInfo		(LPCSTR str)
{
	m_GiveInfo.push_back	(str);
}
void CPhraseScript::AddDisableInfo	(LPCSTR str)
{
	m_DisableInfo.push_back	(str);
}

CPhrase * AddPhrase_script2(CPhraseDialog*pd,LPCSTR text, int phrase_id, int prev_phrase_id, int goodwil_level)
{

	BearCore::BearString1024  phrase;
	BearCore::BearString1024  prev_phrase;
	if (phrase_id == -1)
		phrase[0] = 0;
	else
		BearCore::BearString::Printf(phrase, TEXT("%d"), phrase_id);
	if (prev_phrase_id == -1)
		prev_phrase[0] = 0;
	else
		BearCore::BearString::Printf(prev_phrase, TEXT("%d"), prev_phrase_id);
	return pd->AddPhrase(text, phrase, prev_phrase, goodwil_level);
}

#pragma optimize("s",on)
void CPhraseDialogExporter::script_register(lua_State *L)
{
	module(L)
	[
		class_<CPhrase>("CPhrase")
		.def("GetPhraseScript",		&CPhrase::GetPhraseScript),

		class_<CPhraseDialog>("CPhraseDialog")
		.def("AddPhrase",			&CPhraseDialog::AddPhrase_script )
		.def("AddPhrase", &AddPhrase_script2),

		class_<CPhraseScript>("CPhraseScript")
		.def("AddPrecondition",		&CPhraseScript::AddPrecondition)
		.def("AddAction",			&CPhraseScript::AddAction)
		.def("AddHasInfo",			&CPhraseScript::AddHasInfo)
		.def("AddDontHasInfo",		&CPhraseScript::AddDontHasInfo)
		.def("AddGiveInfo",			&CPhraseScript::AddGiveInfo)
		.def("AddDisableInfo",		&CPhraseScript::AddDisableInfo)
	];
}
