// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
// Third generation by Oles.

#ifndef stdafxH
#define stdafxH

#pragma once

#include "tools/xrCore.h"
#ifdef WINDOWS
#ifdef MSVC
#include <DPlay/dplay8.h>
#else
#include <dplay8.h>
#endif
#endif
#include "NET_Shared.h"	
#ifndef _RELEASE
#define _RELEASE(x)			{ if(x) { (x)->Release();       (x)=NULL; } }
#endif
#ifndef _SHOW_REF
#define _SHOW_REF(msg, x)   { if(x) { x->AddRef(); Msg(msg,u32(x->Release()));}}
#endif
#endif //stdafxH
