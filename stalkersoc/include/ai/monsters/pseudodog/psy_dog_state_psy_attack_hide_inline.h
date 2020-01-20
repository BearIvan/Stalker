#pragma once

#include "../../../ai_space.h"
#include "../monster_cover_manager.h"
#include "../../../cover_point.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>
#define CStatePsyDogHideAbstract CStatePsyDogHide<_Object>

TEMPLATE_SPECIALIZATION
void CStatePsyDogHideAbstract::initialize()
{
	inherited::initialize();

	select_target_point();
	inherited::object->path().prepare_builder();

}

TEMPLATE_SPECIALIZATION
void CStatePsyDogHideAbstract::execute()
{
	inherited::object->set_action					(ACT_RUN);
	inherited::object->path().set_target_point		(target.position, target.node);
	inherited::object->path().set_rebuild_time		(0);
	inherited::object->path().set_distance_to_end	(0);
	inherited::object->path().set_use_covers		(false);

	inherited::object->anim().accel_activate		(eAT_Aggressive);
	inherited::object->anim().accel_set_braking	(false);

	inherited::object->sound().play				(MonsterSound::eMonsterSoundAggressive, 0,0,inherited::object->db().m_dwAttackSndDelay);
}

TEMPLATE_SPECIALIZATION
bool CStatePsyDogHideAbstract::check_start_conditions()
{
	return true;
}

TEMPLATE_SPECIALIZATION
bool CStatePsyDogHideAbstract::check_completion()
{
	return ((inherited::object->ai_location().level_vertex_id() == target.node) && !inherited::object->control().path_builder().is_moving_on_path());
}

TEMPLATE_SPECIALIZATION
void CStatePsyDogHideAbstract::select_target_point()
{
	const CCoverPoint	*point = inherited::object->CoverMan->find_cover(inherited::object->EnemyMan.get_enemy_position(),10.f,30.f);
	if (point && (inherited::object->Position().distance_to(point->position()) > 2.f)) {
		target.node					= point->level_vertex_id	();
		target.position				= point->position			();
	} else {
		const CCoverPoint	*point1 = inherited::object->CoverMan->find_cover(inherited::object->Position(),10.f,30.f);
		if (point1 && (inherited::object->Position().distance_to(point1->position()) > 2.f)) {
			target.node					= point1->level_vertex_id	();
			target.position				= point1->position			();
		} else {
			target.node					= 0;
			target.position				= ai().level_graph().vertex_position(target.node);
		}
	}
}

#undef TEMPLATE_SPECIALIZATION
#undef CStatePsyDogHideAbstract
