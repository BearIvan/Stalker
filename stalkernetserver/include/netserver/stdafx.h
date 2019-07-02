// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
// Third generation by Oles.

#ifndef stdafxH
#define stdafxH

#pragma once

#include "tools/xrCore.h"

#include <DPlay/dplay8.h>

#include "NET_Shared.h"	

#define _RELEASE(x)			{ if(x) { (x)->Release();       (x)=NULL; } }
#define _SHOW_REF(msg, x)   { if(x) { x->AddRef(); Msg(msg,u32(x->Release()));}}

#endif //stdafxH
