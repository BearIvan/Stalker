#pragma once

#include "../../../clsid_game.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterStealAbstract CStateMonsterSteal<_Object>

#define STEAL_MIN_DISTANCE		4.f
#define STEAL_MAX_DISTANCE		15.f
#define STEAL_MAX_PATH_ANGLE	XrMath::PI_DIV_6


TEMPLATE_SPECIALIZATION
CStateMonsterStealAbstract::CStateMonsterSteal(_Object *obj) : inherited(obj)
{
}

TEMPLATE_SPECIALIZATION
void CStateMonsterStealAbstract::initialize()
{
	inherited::initialize					();
	inherited::object->path().prepare_builder	();	
}

TEMPLATE_SPECIALIZATION
void CStateMonsterStealAbstract::execute()
{
	inherited::object->set_action						(ACT_STEAL);
	inherited::object->anim().accel_activate			(eAT_Calm);
	inherited::object->anim().accel_set_braking		(false);
	inherited::object->path().set_target_point			(inherited::object->EnemyMan.get_enemy_position(), inherited::object->EnemyMan.get_enemy_vertex());
	inherited::object->path().set_generic_parameters	();
	inherited::object->set_state_sound					(MonsterSound::eMonsterSoundSteal);
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterStealAbstract::check_completion()
{
	return (!check_conditions());
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterStealAbstract::check_start_conditions()
{
	return (check_conditions());
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterStealAbstract::check_conditions()
{
	// if i see enemy
	if (!inherited::object->EnemyMan.see_enemy_now())						return false;
	
	// This is the only enemy
	if (inherited::object->EnemyMan.get_enemies_count() > 1)				return false;
	
	// There is extended info about enemy?
	if (!inherited::object->EnemyMan.get_flags().is(FLAG_ENEMY_STATS_NOT_READY)) {
		// Enemy is not moving fast
		if (inherited::object->EnemyMan.get_flags().is(FLAG_ENEMY_GO_FARTHER_FAST))		return false;

		// Enemy doesn't know about me
		if (!inherited::object->EnemyMan.get_flags().is(FLAG_ENEMY_DOESNT_KNOW_ABOUT_ME))	return false;
	}

	// Don't hear dangerous sounds
	if (inherited::object->hear_dangerous_sound)							return false;
	
	// Don't get hitted
	if (inherited::object->HitMemory.is_hit())								return false;

	// Path with minimal deviation
	//if (inherited::object->control().path_builder().detail().time_path_built() >= time_state_started) {
	//	if (inherited::object->path().get_path_angle() > STEAL_MAX_PATH_ANGLE)	return false;
	//}
	
	// check distance to enemy
	float dist = inherited::object->MeleeChecker.distance_to_enemy(inherited::object->EnemyMan.get_enemy());
	if (dist < STEAL_MIN_DISTANCE)								return false;
	else if (dist > STEAL_MAX_DISTANCE)							return false;

	return true;
}
