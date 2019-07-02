#pragma once
 
#include "engine/stdafx.h"
#include "DPlay/dplay8.h"

#if XRAY_EXCEPTIONS
#	define	THROW(expr)				do {if (!(expr)) {string4096	assertion_info; ::Debug.gather_info(_TRE(#expr),   0,   0,0,DEBUG_INFO,assertion_info); throw assertion_info;}} while(0)
#	define	THROW2(expr,msg0)		do {if (!(expr)) {string4096	assertion_info; ::Debug.gather_info(_TRE(#expr),msg0,   0,0,DEBUG_INFO,assertion_info); throw assertion_info;}} while(0)
#	define	THROW3(expr,msg0,msg1)	do {if (!(expr)) {string4096	assertion_info; ::Debug.gather_info(_TRE(#expr),msg0,msg1,0,DEBUG_INFO,assertion_info); throw assertion_info;}} while(0)
#else
#	define	THROW					VERIFY
#	define	THROW2					VERIFY2
#	define	THROW3					VERIFY3
#endif
#define DEBUGFATALERROR1 VERIFY
#define DEBUGFATALERROR12 VERIFY2
#include "engine/gamefont.h"
#include "engine/xr_object.h"
#include "engine/igame_level.h"
#include "physics/xrphysics.h"
#include "engine/xr_input.h"
#include "smart_cast.h"

#ifdef _MSC_VER
# pragma warning(disable: 4589 4459)
#else
#error "Добавь warning ignore"
#endif