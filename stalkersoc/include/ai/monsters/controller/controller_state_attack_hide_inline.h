#pragma once

#include "../../../ai_space.h"
#include "../monster_cover_manager.h"
#include "../../../cover_point.h"
#include "../../../level.h"
#include "../../../level_debug.h"


#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>
#define CStateControllerHideAbstract CStateControlHide<_Object>

TEMPLATE_SPECIALIZATION
void CStateControllerHideAbstract::initialize()
{
	inherited::initialize();
	
	m_cover_reached		= false;
	select_target_point();
	inherited::object->path().prepare_builder();
	
}

TEMPLATE_SPECIALIZATION
void CStateControllerHideAbstract::execute()
{
	if (m_state_fast_run) {
		if (target.position.distance_to(inherited::object->Position()) < 5.f) {
			m_state_fast_run			= false;
			inherited::object->set_mental_state	(CController::eStateDanger);
		}
	}
	
	inherited::object->set_action					(ACT_RUN);
	
	inherited::object->path().set_target_point		(target.position, target.node);
	inherited::object->path().set_rebuild_time		(0);
	inherited::object->path().set_distance_to_end	(0.f);
	inherited::object->path().set_use_covers		(false);

	inherited::object->anim().accel_activate		(eAT_Aggressive);
	inherited::object->anim().accel_set_braking	(false);
	
	inherited::object->sound().play				(MonsterSound::eMonsterSoundAggressive, 0,0,inherited::object->db().m_dwAttackSndDelay);
	
	if (inherited::object->HitMemory.get_last_hit_time() > inherited::object->EnemyMan.get_enemy_time_last_seen()) {
		Fvector pos;
		pos.mad(inherited::object->Position(),inherited::object->HitMemory.get_last_hit_dir(),5.f);
		pos.y += 1.5f;
		inherited::object->custom_dir().head_look_point(pos);
	} else 
		inherited::object->custom_dir().head_look_point(inherited::object->EnemyMan.get_enemy_position());

	inherited::object->custom_anim().set_body_state(CControllerAnimation::eTorsoRun,CControllerAnimation::eLegsTypeRun);
}

TEMPLATE_SPECIALIZATION
bool CStateControllerHideAbstract::check_start_conditions()
{
	return true;
}

TEMPLATE_SPECIALIZATION
void CStateControllerHideAbstract::finalize()
{
	inherited::finalize();
	inherited::object->set_mental_state	(CController::eStateDanger);
}

TEMPLATE_SPECIALIZATION
void CStateControllerHideAbstract::critical_finalize()
{
	inherited::finalize();
	inherited::object->set_mental_state	(CController::eStateDanger);
}

TEMPLATE_SPECIALIZATION
bool CStateControllerHideAbstract::check_completion()
{
	return ((inherited::object->ai_location().level_vertex_id() == target.node) && !inherited::object->control().path_builder().is_moving_on_path());
}

TEMPLATE_SPECIALIZATION
void CStateControllerHideAbstract::select_target_point()
{
#ifdef DEBUG
	DBG().level_info(this).clear();
#endif

	const CCoverPoint	*point = inherited::object->CoverMan->find_cover(inherited::object->EnemyMan.get_enemy_position(),10.f,30.f);
	if (point) {
		target.node					= point->level_vertex_id	();
		target.position				= point->position			();
	} else {
		target.node					= 0;
		target.position				= ai().level_graph().vertex_position(target.node);			
	}

	m_state_fast_run = (target.position.distance_to(inherited::object->Position()) > 20.f);
	if (m_state_fast_run && (Random.randI(100) < 50)) 
		inherited::object->set_mental_state	(CController::eStateIdle);
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateControllerHideAbstract
