#pragma once

#include "../../../sound_player.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStatePoltergeistAttackHiddenAbstract CStatePoltergeistAttackHidden<_Object>

#define FLY_AROUND_DIST		15.f
#define DIST_TO_PATH_END	1.5f


TEMPLATE_SPECIALIZATION
void CStatePoltergeistAttackHiddenAbstract::initialize()
{
	inherited::initialize();
	select_target_point();
	inherited::object->path().prepare_builder();
}

TEMPLATE_SPECIALIZATION
void CStatePoltergeistAttackHiddenAbstract::execute()
{
	// проверить на завершение пути
	if (inherited::object->control().path_builder().detail().time_path_built() > inherited::time_state_started) {
		if (inherited::object->control().path_builder().is_path_end(DIST_TO_PATH_END)) select_target_point();
	}
	
	inherited::object->path().set_target_point		(m_target.point, m_target.node);
	inherited::object->path().set_rebuild_time		(5000);
	inherited::object->path().set_distance_to_end	(3.f);
	inherited::object->path().set_use_covers		(false);

	inherited::object->anim().m_tAction			= ACT_RUN;
	inherited::object->anim().accel_activate	(eAT_Aggressive);
	inherited::object->anim().accel_set_braking (false);
	inherited::object->sound().play				(MonsterSound::eMonsterSoundAggressive, 0,0,inherited::object->db().m_dwAttackSndDelay);
}

TEMPLATE_SPECIALIZATION
void CStatePoltergeistAttackHiddenAbstract::select_target_point()
{
	float dist = inherited::object->Position().distance_to(inherited::object->EnemyMan.get_enemy_position());
	if (dist > FLY_AROUND_DIST) {
		m_target.point	= inherited::object->EnemyMan.get_enemy_position();
		m_target.node	= inherited::object->EnemyMan.get_enemy_vertex();
	} else {
		m_target.point	= random_position(inherited::object->EnemyMan.get_enemy_position(), FLY_AROUND_DIST / 2);
		m_target.node	= u32(-1);
	}
}

#undef TEMPLATE_SPECIALIZATION
#undef CStatePoltergeistAttackHiddenAbstract