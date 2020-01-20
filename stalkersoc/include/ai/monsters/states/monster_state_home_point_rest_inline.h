#pragma once

#include "../monster_home.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterRestMoveToHomePointAbstract CStateMonsterRestMoveToHomePoint<_Object>

TEMPLATE_SPECIALIZATION
void CStateMonsterRestMoveToHomePointAbstract::initialize()
{
	inherited::initialize	();
	m_target_node			= inherited::object->Home->get_place();
}

TEMPLATE_SPECIALIZATION
void CStateMonsterRestMoveToHomePointAbstract::execute()
{
	inherited::object->path().set_target_point		(ai().level_graph().vertex_position(m_target_node), m_target_node);
	inherited::object->anim().accel_activate		(EAccelType(inherited::object->Home->is_aggressive() ? eAT_Aggressive : eAT_Calm));
	inherited::object->anim().accel_set_braking	(true);
	inherited::object->path().set_rebuild_time		(0);
	inherited::object->path().set_distance_to_end	(0.f);
	inherited::object->path().set_use_covers		(false);
	
	inherited::object->set_action					(inherited::object->Home->is_aggressive() ? ACT_RUN : ACT_WALK_FWD);
	inherited::object->set_state_sound				(inherited::object->Home->is_aggressive() ? MonsterSound::eMonsterSoundAggressive : MonsterSound::eMonsterSoundIdle);
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterRestMoveToHomePointAbstract::check_start_conditions()
{
	return (!inherited::object->Home->at_home());
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterRestMoveToHomePointAbstract::check_completion()
{
	return ((inherited::object->ai_location().level_vertex_id() == m_target_node) && !inherited::object->control().path_builder().is_moving_on_path());
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterRestMoveToHomePointAbstract
