#include "pch_script.h"

//UI-controls
#include "ui/UIScriptWnd.h"
#include "ui/UIButton.h"
#include "ui/UIMessageBox.h"
#include "ui/UIPropertiesBox.h"
#include "ui/UICheckButton.h"
#include "ui/UIRadioButton.h"
#include "ui/UIStatic.h"
#include "ui/UIEditBox.h"
#include "ui/UIFrameWindow.h"
#include "ui/UIFrameLineWnd.h"
#include "ui/UIProgressBar.h"
#include "ui/UITabControl.h"

#include "ui/uiscriptwnd_script.h"

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
