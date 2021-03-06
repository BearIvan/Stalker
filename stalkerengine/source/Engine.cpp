// Engine.cpp: implementation of the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Engine.h"
#include "dedicated_server_only.h"

CEngine Engine;
xrDispatchTable PSGP;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEngine::CEngine()
{

}

CEngine::~CEngine()
{

}

extern void msCreate(LPCSTR name);

PROTECT_API void CEngine::Initialize(void)
{
    // Bind PSGP
    R_ASSERT(BearManagerProjects::CheckProject(TEXT("stalker_cpu_pipe")));
    xrBinder* bindCPU = BearManagerProjects::GetFunctionInProject<xrBinder*>(TEXT("stalker_cpu_pipe"),TEXT("xrBind_PSGP"));
    R_ASSERT(bindCPU);
    bindCPU(&PSGP);

    // Other stuff
    Engine.Sheduler.Initialize();
    //
#ifdef DEBUG
    msCreate("game");
#endif
}

typedef void __cdecl ttapi_Done_func(void);

void CEngine::Destroy()
{
    Engine.Sheduler.Destroy();
#ifdef DEBUG_MEMORY_MANAGER
    extern void dbg_dump_leaks_prepare();
    if (Memory.debug_mode) dbg_dump_leaks_prepare();
#endif // DEBUG_MEMORY_MANAGER
    Engine.External.Destroy();

    if (hPSGP)
    {
       /* ttapi_Done_func* ttapi_Done = (ttapi_Done_func*)GetProcAddress(hPSGP, "ttapi_Done");
        R_ASSERT(ttapi_Done);
        if (ttapi_Done)
            ttapi_Done();*/

        FreeLibrary(hPSGP);
        hPSGP = 0;
        ZeroMemory(&PSGP, sizeof(PSGP));
    }
}
