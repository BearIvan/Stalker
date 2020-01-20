#pragma once

#include "../../../alife_simulator.h"
#include "../../../alife_object_registry.h"
#include "../../../xrServer_Objects_ALife_Monsters.h"
#include "../../../alife_monster_brain.h"

#include "state_move_to_point.h"
#include "state_custom_action.h"
#include "monster_state_smart_terrain_task_graph_walk.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterSmartTerrainTaskAbstract CStateMonsterSmartTerrainTask<_Object>

TEMPLATE_SPECIALIZATION
CStateMonsterSmartTerrainTaskAbstract::CStateMonsterSmartTerrainTask(_Object *obj) : inherited(obj)
{
	inherited::add_state(eStateSmartTerrainTaskGamePathWalk,	xr_new<CStateMonsterSmartTerrainTaskGraphWalk<_Object> >(obj));
	inherited::add_state(eStateSmartTerrainTaskLevelPathWalk,	xr_new<CStateMonsterMoveToPointEx<_Object> >			(obj));
	inherited::add_state(eStateSmartTerrainTaskWaitCapture,	xr_new<CStateMonsterCustomAction<_Object> >				(obj));
}

TEMPLATE_SPECIALIZATION
CStateMonsterSmartTerrainTaskAbstract::~CStateMonsterSmartTerrainTask	()
{
}

TEMPLATE_SPECIALIZATION
void CStateMonsterSmartTerrainTaskAbstract::initialize()
{
	inherited::initialize();

	// save current task
	CSE_ALifeMonsterAbstract		*monster = smart_cast<CSE_ALifeMonsterAbstract*>(ai().alife().objects().object(inherited::object->ID()));
	VERIFY							(monster);
	VERIFY							(monster->m_smart_terrain_id != 0xffff);

	m_current_task					= monster->brain().smart_terrain().task(monster);
}


TEMPLATE_SPECIALIZATION
bool CStateMonsterSmartTerrainTaskAbstract::check_start_conditions()
{
	if (!ai().get_alife()) 			return false;

	CSE_ALifeMonsterAbstract		*monster = smart_cast<CSE_ALifeMonsterAbstract*>(ai().alife().objects().object(inherited::object->ID()));
	VERIFY							(monster);
	
	CSE_ALifePsyDogPhantom			*phantom = smart_cast<CSE_ALifePsyDogPhantom*>(monster);
	if (phantom)					return false;
	

	monster->brain().select_task	();
	
	// there is no any available smart terrains
	if (monster->m_smart_terrain_id == 0xffff) return false;

	// we dont need to reach task
	if (monster->m_task_reached) return false;

	return true;
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterSmartTerrainTaskAbstract::check_completion()
{
	CSE_ALifeMonsterAbstract		*monster = smart_cast<CSE_ALifeMonsterAbstract*>(ai().alife().objects().object(inherited::object->ID()));
	VERIFY							(monster);

	if (monster->m_smart_terrain_id == 0xffff) return true;

	// if we already reach the task
	if (monster->m_task_reached) return true;

	return false;
}

TEMPLATE_SPECIALIZATION
void CStateMonsterSmartTerrainTaskAbstract::setup_substates()
{
	state_ptr state = inherited::get_state_current();

	if (inherited::current_substate == eStateSmartTerrainTaskLevelPathWalk) {
		SStateDataMoveToPointEx data;

		data.vertex				= m_current_task->level_vertex_id();
		data.point				= ai().level_graph().vertex_position(data.vertex);
		data.action.action		= ACT_WALK_FWD;
		data.action.time_out	= 0;		// do not use time out
		data.completion_dist	= 0.f;		// get exactly to the point
		data.time_to_rebuild	= 0;		// do not rebuild
		data.accelerated		= true;
		data.braking			= false;
		data.accel_type 		= eAT_Calm;
		data.action.sound_type	= MonsterSound::eMonsterSoundIdle;
		data.action.sound_delay = inherited::object->db().m_dwIdleSndDelay;

		state->fill_data_with(&data, sizeof(SStateDataMoveToPointEx));
		return;
	}

	if (inherited::current_substate == eStateSmartTerrainTaskWaitCapture) {
		SStateDataAction	data;

		data.action			= ACT_REST;
		data.sound_type		= MonsterSound::eMonsterSoundIdle;
		data.sound_delay	= inherited::object->db().m_dwIdleSndDelay;
		data.time_out		= 0;

		state->fill_data_with(&data, sizeof(SStateDataAction));
		return;
	}
}

TEMPLATE_SPECIALIZATION
void CStateMonsterSmartTerrainTaskAbstract::reselect_state()
{
	if (inherited::prev_substate == u32(-1)) {
		if (inherited::get_state(eStateSmartTerrainTaskGamePathWalk)->check_start_conditions()) {
			inherited::select_state(eStateSmartTerrainTaskGamePathWalk);
		} else {
			inherited::select_state(eStateSmartTerrainTaskLevelPathWalk);
		}
		return;
	}

	if (inherited::prev_substate == eStateSmartTerrainTaskGamePathWalk) {
		inherited::select_state(eStateSmartTerrainTaskLevelPathWalk);
		return;
	}

	if (inherited::prev_substate == eStateSmartTerrainTaskLevelPathWalk) {
		inherited::select_state(eStateSmartTerrainTaskWaitCapture);
		return;
	}

	inherited::select_state(eStateSmartTerrainTaskWaitCapture);
}

TEMPLATE_SPECIALIZATION
void CStateMonsterSmartTerrainTaskAbstract::check_force_state()
{
	// check smart terrain became busy
	CSE_ALifeMonsterAbstract		*monster = smart_cast<CSE_ALifeMonsterAbstract*>(ai().alife().objects().object(inherited::object->ID()));
	VERIFY							(monster);

	if ((monster->m_smart_terrain_id == 0xffff) || monster->m_task_reached) {
		inherited::select_state(eStateSmartTerrainTaskWaitCapture);
		return;
	}

	// check if task has changed
	CALifeSmartTerrainTask			*task = monster->brain().smart_terrain().task(monster);
	if (!task || (m_current_task != task)) {
		if (inherited::current_substate != u32(-1)) inherited::get_state_current()->critical_finalize();
		
		inherited::current_substate = u32(-1);
		inherited::prev_substate	 = u32(-1);
		
		m_current_task	= task;
	}
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterSmartTerrainTaskAbstract

