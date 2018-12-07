#include "pch_script.h"

//UI-controls
//#include "ui/UIListWnd.h"
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

#include "ui/UIscriptwnd_script.h"

using namespace luabind;

#pragma optimize("s",on)
export_class &script_register_ui_window2(export_class &instance)
{
	instance
		.def("OnKeyboard",		&BaseType::OnKeyboardAction, &WrapType::OnKeyboard_static)
		.def("Update",			&BaseType::Update, &WrapType::Update_static)
		.def("Dispatch",		&BaseType::Dispatch, &WrapType::Dispatch_static)

	;return	(instance);
}
