#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>
#define CStateChimeraThreatenStealAbstract CStateChimeraThreatenSteal<_Object>

TEMPLATE_SPECIALIZATION
void CStateChimeraThreatenStealAbstract::initialize()
{
	inherited::initialize();
	
	inherited::object->SetUpperState	(false);

	inherited::data.action.action		= ACT_STEAL;
	
	inherited::data.accelerated		= true;
	inherited::data.braking			= false;
	inherited::data.accel_type 		= eAT_Calm;
	
	inherited::data.completion_dist	= 2.f;
	inherited::data.action.sound_type	= MonsterSound::eMonsterSoundIdle;
	inherited::data.action.sound_delay = inherited::object->db().m_dwIdleSndDelay;
}

TEMPLATE_SPECIALIZATION
void CStateChimeraThreatenStealAbstract::finalize()
{
	inherited::finalize();
	inherited::object->SetUpperState	();
}


TEMPLATE_SPECIALIZATION
void CStateChimeraThreatenStealAbstract::execute()
{
	inherited::data.point				= inherited::object->EnemyMan.get_enemy_position	();
	inherited::data.vertex				= inherited::object->EnemyMan.get_enemy_vertex		();
	inherited::data.time_to_rebuild	= inherited::object->get_attack_rebuild_time		();

	inherited::execute();
}

#define MIN_DISTANCE_TO_ENEMY	8.f

TEMPLATE_SPECIALIZATION
bool CStateChimeraThreatenStealAbstract::check_completion()
{	
	if (inherited::check_completion()) return true;
	
	float dist_to_enemy = inherited::object->EnemyMan.get_enemy_position().distance_to(inherited::object->Position());
	if (dist_to_enemy < MIN_DISTANCE_TO_ENEMY) return true;

	return false;
}

TEMPLATE_SPECIALIZATION
bool CStateChimeraThreatenStealAbstract::check_start_conditions()
{
	float dist_to_enemy = inherited::object->EnemyMan.get_enemy_position().distance_to(inherited::object->Position());
	if (dist_to_enemy > MIN_DISTANCE_TO_ENEMY) return true;
	return false;
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateChimeraThreatenStealAbstract
#undef MIN_DISTANCE_TO_ENEMY
