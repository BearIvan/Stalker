#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>
#define CStateMonsterHittedHideAbstract CStateMonsterHittedHide<_Object>

#define GOOD_DISTANCE_TO_ENEMY	10.f
#define GOOD_DISTANCE_IN_COVER	15.f
#define MIN_HIDE_TIME			3.f
#define DIST_TO_PATH_END		1.5f

TEMPLATE_SPECIALIZATION
void CStateMonsterHittedHideAbstract::initialize()
{
	inherited::initialize();
	inherited::object->path().prepare_builder	();	
}

TEMPLATE_SPECIALIZATION
void CStateMonsterHittedHideAbstract::execute()
{
	inherited::object->set_action									(ACT_RUN);
	inherited::object->set_state_sound								(MonsterSound::eMonsterSoundPanic);
	inherited::object->anim().accel_activate					(eAT_Aggressive);
	inherited::object->anim().accel_set_braking					(false);
	inherited::object->path().set_retreat_from_point	(inherited::object->HitMemory.get_last_hit_position());
	inherited::object->path().set_generic_parameters	();

}

TEMPLATE_SPECIALIZATION
bool CStateMonsterHittedHideAbstract::check_start_conditions()
{
	if (inherited::object->HitMemory.is_hit() && !inherited::object->EnemyMan.get_enemy()) return true;
	return false;
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterHittedHideAbstract::check_completion()
{
	float dist = inherited::object->Position().distance_to(inherited::object->HitMemory.get_last_hit_position());

	// good dist  
	if (dist < GOOD_DISTANCE_IN_COVER) return false;
	// +hide more than 3 sec
	if (inherited::time_state_started + MIN_HIDE_TIME > Device.dwTimeGlobal) return false;

	return true;
}

#undef GOOD_DISTANCE_IN_COVER
#undef DIST_TO_PATH_END
#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterHittedHideAbstract
