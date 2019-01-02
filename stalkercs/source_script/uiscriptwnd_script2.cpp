#include "pch_script.h"

//UI-controls
#include "UI/UIListWnd.h"
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

#pragma optimize("s",on)
export_class &script_register_ui_window2(export_class &instance)
{
	instance
		.def("GetStatic",		(CUIStatic* (BaseType::*)(LPCSTR)) &BaseType::GetControl<CUIStatic>)
		.def("GetEditBox",		(CUIEditBox* (BaseType::*)(LPCSTR)) &BaseType::GetControl<CUIEditBox>)
		.def("GetDialogWnd",	(CUIDialogWnd* (BaseType::*)(LPCSTR)) &BaseType::GetControl<CUIDialogWnd>)
		.def("GetFrameWindow",	(CUIFrameWindow* (BaseType::*)(LPCSTR)) &BaseType::GetControl<CUIFrameWindow>)
		.def("GetFrameLineWnd",	(CUIFrameLineWnd* (BaseType::*)(LPCSTR)) &BaseType::GetControl<CUIFrameLineWnd>)
		.def("GetProgressBar",	(CUIProgressBar* (BaseType::*)(LPCSTR)) &BaseType::GetControl<CUIProgressBar>)
		.def("GetTabControl",	(CUITabControl* (BaseType::*)(LPCSTR)) &BaseType::GetControl<CUITabControl>)
		.def("GetListWnd",		(CUIListWnd* (BaseType::*)(LPCSTR)) &BaseType::GetControl<CUIListWnd>)

		.def("OnKeyboard",		&BaseType::OnKeyboard, &WrapType::OnKeyboard_static)
		.def("Update",			&BaseType::Update, &WrapType::Update_static)
		.def("Dispatch",		&BaseType::Dispatch, &WrapType::Dispatch_static)

	;return	(instance);
}
