#include "pch_script.h"

//UI-controls
#include "UI/UIScriptWnd.h"
#include "UI/UIButton.h"
#include "UI/UIMessageBox.h"
#include "UI/UIPropertiesBox.h"
#include "UI/UICheckButton.h"
#include "UI/UIRadioButton.h"
#include "UI/UIStatic.h"
#include "UI/UIEditBox.h"
#include "UI/UIFrameWindow.h"
#include "UI/UIFrameLineWnd.h"
#include "UI/UIProgressBar.h"
#include "UI/UITabControl.h"

#include "UI/UIscriptwnd_script.h"

using namespace luabind;

extern export_class &script_register_ui_window1(export_class &);
extern export_class &script_register_ui_window2(export_class &);

#pragma optimize("s",on)
void CUIDialogWndEx::script_register(lua_State *L)
{
	export_class				instance("CUIScriptWnd");

	module(L)
	[
		script_register_ui_window2(
			script_register_ui_window1(
				instance
			)
		)
		.def("Load",			&BaseType::Load)
	];
}

export_class &script_register_ui_window1(export_class &instance)
{
	instance
		.def(					constructor<>())

		.def("AddCallback",		(void(BaseType::*)(LPCSTR, s16, const luabind::functor<void>&))&BaseType::AddCallback)
		.def("AddCallback",		(void(BaseType::*)(LPCSTR, s16, const luabind::functor<void>&, const luabind::object&))&BaseType::AddCallback)

		.def("Register",		(void (BaseType::*)(CUIWindow*))&BaseType::Register)
		.def("Register",		(void (BaseType::*)(CUIWindow*,LPCSTR))&BaseType::Register)

		.def("GetButton",		(CUIButton* (BaseType::*)(LPCSTR)) &BaseType::GetControl<CUIButton>)
		.def("GetMessageBox",	(CUIMessageBox* (BaseType::*)(LPCSTR)) &BaseType::GetControl<CUIMessageBox>)
		.def("GetPropertiesBox",(CUIPropertiesBox* (BaseType::*)(LPCSTR)) &BaseType::GetControl<CUIPropertiesBox>)
		.def("GetCheckButton",	(CUICheckButton* (BaseType::*)(LPCSTR)) &BaseType::GetControl<CUICheckButton>)
		.def("GetRadioButton",	(CUIRadioButton* (BaseType::*)(LPCSTR)) &BaseType::GetControl<CUIRadioButton>)
//		.def("GetRadioGroup",	(CUIRadioGroup* (BaseType::*)(LPCSTR)) &BaseType::GetControl<CUIRadioGroup>)

	;return	(instance);
}
