#pragma once

#include "../../../ai_space.h"
#include "../monster_cover_manager.h"
#include "../../../cover_point.h"
#include "../../../level.h"
#include "../../../level_debug.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>
#define CStateControllerHideLiteAbstract CStateControlHideLite<_Object>

TEMPLATE_SPECIALIZATION
void CStateControllerHideLiteAbstract::initialize()
{
	inherited::initialize			();

	select_target_point				();
	inherited::object->path().prepare_builder	();

}

TEMPLATE_SPECIALIZATION
void CStateControllerHideLiteAbstract::execute()
{
	inherited::object->path().set_target_point		(target.position, target.node);
	inherited::object->path().set_rebuild_time		(0);
	inherited::object->path().set_distance_to_end	(0.f);
	inherited::object->path().set_use_covers		(true);

	inherited::object->anim().accel_activate		(eAT_Aggressive);
	inherited::object->anim().accel_set_braking	(false);

	inherited::object->sound().play				(MonsterSound::eMonsterSoundAggressive, 0,0,inherited::object->db().m_dwAttackSndDelay);
	inherited::object->custom_dir().head_look_point(inherited::object->EnemyMan.get_enemy_position());

	inherited::object->custom_anim().set_body_state(CControllerAnimation::eTorsoRun,CControllerAnimation::eLegsTypeRun);
}

TEMPLATE_SPECIALIZATION
bool CStateControllerHideLiteAbstract::check_start_conditions()
{
	return true;
}

TEMPLATE_SPECIALIZATION
void CStateControllerHideLiteAbstract::reinit()
{
	inherited::reinit();
	m_time_finished = 0;
}


TEMPLATE_SPECIALIZATION
void CStateControllerHideLiteAbstract::finalize()
{
	inherited::finalize();
	m_time_finished	= Device.dwTimeGlobal;
}


TEMPLATE_SPECIALIZATION
bool CStateControllerHideLiteAbstract::check_completion()
{
	if ((inherited::object->ai_location().level_vertex_id() == target.node) && 
		!inherited::object->control().path_builder().is_moving_on_path()) return true;
	
	return (!inherited::object->EnemyMan.see_enemy_now());
}

TEMPLATE_SPECIALIZATION
void CStateControllerHideLiteAbstract::select_target_point()
{
#ifdef DEBUG
	DBG().level_info(this).clear();
#endif

	const CCoverPoint	*point = inherited::object->CoverMan->find_cover(inherited::object->EnemyMan.get_enemy_position(),10.f,30.f);
	//VERIFY(point);
	if (point) {
		target.node					= point->level_vertex_id	();
		target.position				= point->position			();
	} else {
		target.node					= 0;
		target.position				= ai().level_graph().vertex_position(target.node);			
	}
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateControllerHideLiteAbstract
