// Stats.h: interface for the CStats class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATS_H__4C8D1860_0EE2_11D4_B4E3_4854E82A090D__INCLUDED_)
#define AFX_STATS_H__4C8D1860_0EE2_11D4_B4E3_4854E82A090D__INCLUDED_
#pragma once

class ENGINE_API CGameFont;
#include "XrRender/FactoryPtr.h"
#include "XrRender/StatsRender.h"
DECLARE_MESSAGE(Stats);

class ENGINE_API CStatsPhysics
{
public:
	XrTimerStat	 ph_collision; // collision
	XrTimerStat		 ph_core; // integrate
	XrTimerStat	 Physics; // movement+collision
};

class ENGINE_API CStats :
    public pureRender,
    public CStatsPhysics
{
public:
    CGameFont* pFont;

    float fFPS, fRFPS, fTPS; // FPS, RenderFPS, TPS
    float fMem_calls;
    u32 dwMem_calls;
    u32 dwSND_Played, dwSND_Allocated; // Play/Alloc
    float fShedulerLoad;

	XrTimerStat	 EngineTOTAL; //
	XrTimerStat	 Sheduler; //
	XrTimerStat	 UpdateClient; //
	bsize UpdateClient_updated; //
	bsize UpdateClient_crows; //
	bsize UpdateClient_active; //
	bsize UpdateClient_total; //
	bsize Particles_starting; // starting
	bsize Particles_active; // active
	bsize Particles_destroy; // destroying
    // CStatTimer Physics; // movement+collision
    // CStatTimer ph_collision; // collision
    // CStatTimer ph_core; // collision
	XrTimerStat	 AI_Think; // thinking
	XrTimerStat	 AI_Range; // query: range
	XrTimerStat	 AI_Path; // query: path
	XrTimerStat	 AI_Node; // query: node
	XrTimerStat	 AI_Vis; // visibility detection - total
	XrTimerStat	 AI_Vis_Query; // visibility detection - portal traversal and frustum culling
	XrTimerStat	 AI_Vis_RayTests; // visibility detection - ray casting

	XrTimerStat	 RenderTOTAL; //
	XrTimerStat	 RenderTOTAL_Real;
	XrTimerStat	 RenderCALC; // portal traversal, frustum culling, entities "renderable_Render"
	XrTimerStat	 RenderCALC_HOM; // HOM rendering
	XrTimerStat	 Animation; // skeleton calculation
	XrTimerStat	 RenderDUMP; // actual primitive rendering
	XrTimerStat	 RenderDUMP_Wait; // ...waiting something back (queries results, etc.)
	XrTimerStat	 RenderDUMP_Wait_S; // ...frame-limit sync
	XrTimerStat	 RenderDUMP_RT; // ...render-targets
	XrTimerStat	 RenderDUMP_SKIN; // ...skinning
	XrTimerStat	 RenderDUMP_HUD; // ...hud rendering
	XrTimerStat	 RenderDUMP_Glows; // ...glows vis-testing,sorting,render
	XrTimerStat	 RenderDUMP_Lights; // ...d-lights building/rendering
	XrTimerStat	 RenderDUMP_WM; // ...wallmark sorting, rendering
    u32 RenderDUMP_WMS_Count;// ...number of static wallmark
    u32 RenderDUMP_WMD_Count;// ...number of dynamic wallmark
    u32 RenderDUMP_WMT_Count;// ...number of wallmark tri
	XrTimerStat	 RenderDUMP_DT_VIS; // ...details visibility detection
	XrTimerStat	 RenderDUMP_DT_Render;// ...details rendering
	XrTimerStat	 RenderDUMP_DT_Cache;// ...details slot cache access
    u32 RenderDUMP_DT_Count;// ...number of DT-elements
	XrTimerStat	 RenderDUMP_Pcalc; // ...projectors building
	XrTimerStat	 RenderDUMP_Scalc; // ...shadows building
	XrTimerStat	 RenderDUMP_Srender; // ...shadows render

	XrTimerStat	 Sound; // total time taken by sound subsystem (accurate only in single-threaded mode)
	XrTimerStat	 Input; // total time taken by input subsystem (accurate only in single-threaded mode)
	XrTimerStat	 clRAY; // total: ray-testing
	XrTimerStat	 clBOX; // total: box query
	XrTimerStat	 clFRUSTUM; // total: frustum query

	XrTimerStat	 netClient1;
	XrTimerStat	 netClient2;
	XrTimerStat	 netServer;
	XrTimerStat	 netClientCompressor;
	XrTimerStat	 netServerCompressor;



	XrTimerStat	 TEST0; // debug counter
	XrTimerStat	 TEST1; // debug counter
	XrTimerStat	 TEST2; // debug counter
	XrTimerStat	 TEST3; // debug counter

    shared_str eval_line_1;
    shared_str eval_line_2;
    shared_str eval_line_3;

    void Show(void);
    virtual void OnRender();
    void OnDeviceCreate(void);
    void OnDeviceDestroy(void);
public:
    xr_vector <shared_str> errors;
    CRegistrator <pureStats> seqStats;
public:
    CStats();
    ~CStats();

    IC CGameFont* Font() { return pFont; }

private:
    FactoryPtr<IStatsRender> m_pRender;
};

enum
{
    st_sound = (1 << 0),
    st_sound_min_dist = (1 << 1),
    st_sound_max_dist = (1 << 2),
    st_sound_ai_dist = (1 << 3),
    st_sound_info_name = (1 << 4),
    st_sound_info_object = (1 << 5),
};

extern Flags32 g_stats_flags;

#endif // !defined(AFX_STATS_H__4C8D1860_0EE2_11D4_B4E3_4854E82A090D__INCLUDED_)
