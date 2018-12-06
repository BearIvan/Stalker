#pragma once
#pragma warning ( disable :  4995  ) 

// you must define ENGINE_BUILD then building the engine itself
// and not define it if you are about to build DLL
#ifndef NO_ENGINE_API
#ifdef	ENGINE_BUILD
#define DLL_API			__declspec(dllimport)
#define ENGINE_API		__declspec(dllexport)
#else
#define DLL_API			__declspec(dllexport)
#define ENGINE_API		__declspec(dllimport)
#endif
#else
#define ENGINE_API
#define DLL_API
#endif // NO_ENGINE_API

// Our headers
/*#include "engine.h"
#include "defines.h"*/
#ifndef NO_XRLOG
//#include "log.h"
#endif
#include "tools\xrCore.h"
//#include "device.h"
//#include "fs.h"
#include "defines.h"
//#include "xrXRC.h"
#include "cdb\xrCDB.h"
#define ECORE_API
#ifndef DEBUG
#	define LUABIND_NO_ERROR_CHECKING
#endif


#define LUABIND_DONT_COPY_STRINGS

