#pragma once

#include "../states/state_move_to_point.h"
#include "../states/state_look_point.h"
#include "../states/state_custom_action.h"
#include "../../../cover_point.h"
#include "../monster_cover_manager.h"
#include "../monster_home.h"

#include "../../../actor.h"
#include "../../../actor_memory.h"
#include "../../../visual_memory_manager.h"


#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateBloodsuckerPredatorAbstract CStateBloodsuckerPredator<_Object>

TEMPLATE_SPECIALIZATION
CStateBloodsuckerPredatorAbstract::CStateBloodsuckerPredator(_Object *obj) : inherited(obj)
{
	inherited::add_state	(eStatePredator_MoveToCover,	xr_new<CStateMonsterMoveToPointEx<_Object> >(obj));
	inherited::add_state	(eStatePredator_LookOpenPlace,	xr_new<CStateMonsterLookToPoint<_Object> >	(obj));
	inherited::add_state	(eStatePredator_Camp,			xr_new<CStateMonsterCustomAction<_Object> >	(obj));
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerPredatorAbstract::reinit()
{
	inherited::reinit	();
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerPredatorAbstract::initialize()
{
	inherited::initialize						();

	inherited::object->predator_start						();

	select_camp_point							();
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerPredatorAbstract::reselect_state()
{
	if (inherited::prev_substate == u32(-1)) {
		inherited::select_state(eStatePredator_MoveToCover);
		return;
	}
	
	if (inherited::prev_substate == eStatePredator_MoveToCover) {
		inherited::select_state(eStatePredator_LookOpenPlace);
		return;
	}

	if (inherited::prev_substate == eStatePredator_LookOpenPlace) {
		inherited::select_state(eStatePredator_Camp);
		return;
	}

	inherited::select_state(eStatePredator_Camp);
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerPredatorAbstract::finalize()
{
	inherited::finalize							();

	inherited::object->predator_stop						();
	inherited::object->predator_unfreeze					();

	CMonsterSquad *squad = monster_squad().get_squad(inherited::object);
	squad->unlock_cover(m_target_node);

}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerPredatorAbstract::critical_finalize()
{
	inherited::critical_finalize				();

	inherited::object->predator_stop						();
	inherited::object->predator_unfreeze					();

	CMonsterSquad *squad = monster_squad().get_squad(inherited::object);
	squad->unlock_cover(m_target_node);
}

TEMPLATE_SPECIALIZATION
bool CStateBloodsuckerPredatorAbstract::check_start_conditions()
{
	if (Actor()->memory().visual().visible_now(inherited::object)) return false;
	return true;
}

TEMPLATE_SPECIALIZATION
bool CStateBloodsuckerPredatorAbstract::check_completion()
{
	if (inherited::object->HitMemory.get_last_hit_time() > inherited::time_state_started) return true;
	if (inherited::object->EnemyMan.get_enemy() && inherited::object->EnemyMan.see_enemy_now() && (inherited::object->Position().distance_to(inherited::object->EnemyMan.get_enemy()->Position()) < 4.f)) return true;

	return false;
}


TEMPLATE_SPECIALIZATION
void CStateBloodsuckerPredatorAbstract::setup_substates()
{
	state_ptr state = inherited::get_state_current();
	
	if (inherited::current_substate == eStatePredator_Camp) {
		inherited::object->predator_freeze	();
		m_time_start_camp		= time();

	} else {
		inherited::object->predator_unfreeze();
	}

	if (inherited::current_substate == eStatePredator_MoveToCover) {
		SStateDataMoveToPointEx data;

		data.vertex				= m_target_node;
		data.point				= ai().level_graph().vertex_position(data.vertex);
		data.action.action		= ACT_RUN;
		data.action.time_out	= 0;		// do not use time out
		data.completion_dist	= 0.f;		// get exactly to the point
		data.time_to_rebuild	= 0;		// do not rebuild
		data.accelerated		= true;
		data.braking			= true;
		data.accel_type 		= eAT_Aggressive;
		data.action.sound_type	= MonsterSound::eMonsterSoundIdle;
		data.action.sound_delay = inherited::object->db().m_dwIdleSndDelay;

		state->fill_data_with(&data, sizeof(SStateDataMoveToPointEx));
		return;
	}

	if (inherited::current_substate == eStatePredator_LookOpenPlace) {

		SStateDataLookToPoint	data;

		Fvector dir;
		inherited::object->CoverMan->less_cover_direction(dir);

		data.point.mad			(inherited::object->Position(),dir,10.f);
		data.action.action		= ACT_STAND_IDLE;
		data.action.time_out	= 2000;		
		data.action.sound_type	= MonsterSound::eMonsterSoundIdle;
		data.action.sound_delay = inherited::object->db().m_dwIdleSndDelay;
		data.face_delay			= 0;

		state->fill_data_with(&data, sizeof(SStateDataLookToPoint));
		return;
	}

	if (inherited::current_substate == eStatePredator_Camp) {
		
		SStateDataAction data;

		data.action		= ACT_STAND_IDLE;
		data.time_out	= 0;			// do not use time out
		data.sound_type	= MonsterSound::eMonsterSoundIdle;
		data.sound_delay = inherited::object->db().m_dwIdleSndDelay;

		state->fill_data_with(&data, sizeof(SStateDataAction));

		return;
	}
}

#define TIME_TO_RESELECT_CAMP	15000

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerPredatorAbstract::check_force_state()
{
	if ((inherited::current_substate == eStatePredator_Camp) && (m_time_start_camp + TIME_TO_RESELECT_CAMP < time())) {
		if (inherited::current_substate != u32(-1)) 
			inherited::get_state_current()->critical_finalize();

		inherited::prev_substate		= u32(-1);
		inherited::current_substate	= u32(-1);

		CMonsterSquad *squad = monster_squad().get_squad(inherited::object);
		squad->unlock_cover	(m_target_node);

		select_camp_point	();
	}
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerPredatorAbstract::select_camp_point()
{
	m_target_node = u32(-1);
	if (inherited::object->Home->has_home()) {
		m_target_node							= inherited::object->Home->get_place_in_cover();
		if (m_target_node == u32(-1)) {
			m_target_node						= inherited::object->Home->get_place();
		}
	} 

	if (m_target_node == u32(-1)) {
		const CCoverPoint	*point = inherited::object->CoverMan->find_cover(inherited::object->Position(),10.f,30.f);
		if (point) {
			m_target_node				= point->level_vertex_id	();
		} 
	}

	if (m_target_node == u32(-1)) 
		m_target_node = inherited::object->ai_location().level_vertex_id();


	CMonsterSquad *squad = monster_squad().get_squad(inherited::object);
	squad->lock_cover(m_target_node);
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateBloodsuckerPredatorAbstract

