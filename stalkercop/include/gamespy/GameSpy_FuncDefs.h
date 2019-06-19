#pragma once

#include "GameSpy/Common/gsCommon.h"
#include "GameSpy/Common/gsAvailable.h"
#include "GameSpy/ghttp/ghttp.h"

#include "GameSpy/qr2/qr2.h"
#include "GameSpy/gcdkey/gcdkeyc.h"
#include "GameSpy/gcdkey/gcdkeys.h"
#include "GameSpy/serverbrowsing/sb_serverbrowsing.h"

#include "GameSpy/pt/pt.h"
#include "GameSpy/gp/gp.h"
#include "GameSpy/sake/sake.h"
#include "GameSpy/sc/sc.h"

#undef max
#undef min
extern "C" {

#define GAMESPY_TFN_DECL(r, f, p) typedef DLL_API r __cdecl t_fn_xrGS_##f p

};

#define GAMESPY_FN_VAR_DECL(r, f, p) GAMESPY_TFN_DECL(r, f, p); t_fn_xrGS_##f* xrGS_##f;
#define GAMESPY_LOAD_FN(f)    f = (t_fn_##f*)GetProcAddress(hGameSpyDLL, #f); R_ASSERT2(f, "No such func in xrGameSpy.dll");
