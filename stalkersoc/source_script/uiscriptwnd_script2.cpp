#include "pch_script.h"

//UI-controls
#include "ui/UIListWnd.h"
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
