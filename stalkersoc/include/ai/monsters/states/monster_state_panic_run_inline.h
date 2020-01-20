#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterPanicRunAbstract CStateMonsterPanicRun<_Object>

#define MIN_UNSEEN_TIME			15000
#define MIN_DIST_TO_ENEMY		15.f

TEMPLATE_SPECIALIZATION
void CStateMonsterPanicRunAbstract::initialize()
{
	inherited::initialize();
	
	inherited::object->path().prepare_builder		();	
}

TEMPLATE_SPECIALIZATION
void CStateMonsterPanicRunAbstract::execute()
{
	inherited::object->set_action							(ACT_RUN);
	inherited::object->set_state_sound						(MonsterSound::eMonsterSoundPanic);
	inherited::object->anim().accel_activate			(eAT_Aggressive);
	inherited::object->anim().accel_set_braking			(false);
	inherited::object->path().set_retreat_from_point	(inherited::object->EnemyMan.get_enemy_position());
	inherited::object->path().set_generic_parameters	();
}
TEMPLATE_SPECIALIZATION
bool CStateMonsterPanicRunAbstract::check_completion()
{
	float dist_to_enemy = inherited::object->Position().distance_to(inherited::object->EnemyMan.get_enemy_position());
	u32 time_delta	= Device.dwTimeGlobal - inherited::object->EnemyMan.get_enemy_time_last_seen();

	if (dist_to_enemy < MIN_DIST_TO_ENEMY)  return false;
	if (time_delta	  < MIN_UNSEEN_TIME)	return false;

	return true;
}

#undef DIST_TO_PATH_END
#undef MIN_DIST_TO_ENEMY
#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterPanicRunAbstract
