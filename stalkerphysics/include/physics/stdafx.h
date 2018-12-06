
#pragma once

#define	MTL_EXPORT_API
#define ENGINE_API
#define DLL_API		
#define ECORE_API
//#include "engine/stdafx.h"

#include "tools/xrCore.h"

//#include "../StalkerServerEntities16/smart_cast.h"
//#include "engine/pure.h"
//#include "engine/engineapi.h"
//#include "engine/eventapi.h"


#include "cdb/xrcdb.h"
#include "sound/sound.h"
//#include "engine/IGame_Level.h"


#include "xrPhysics.h"

#include "api/Stalkerapi.h"
#ifdef	DEBUG
#include "d3d9types.h"
#endif
//IC IGame_Level &GLevel()
//{
//	DEBUGFATALERROR1( g_pGameLevel );
//	return *g_pGameLevel;
//}
#define DEBUGFATALERROR1 VERIFY
#define DEBUGFATALERROR12 VERIFY2
class CGameMtlLibrary;
IC CGameMtlLibrary &GMLibrary()
{
	DEBUGFATALERROR1(PGMLib);
	return *PGMLib;
}