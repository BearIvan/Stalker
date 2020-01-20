#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>
#define CStateChimeraThreatenWalkAbstract CStateChimeraThreatenWalk<_Object>

TEMPLATE_SPECIALIZATION
void CStateChimeraThreatenWalkAbstract::initialize()
{
	inherited::initialize();

	inherited::object->SetUpperState	();

	inherited::data.point				= inherited::object->EnemyMan.get_enemy_position	();
	inherited::data.vertex				= inherited::object->EnemyMan.get_enemy_vertex		();

	inherited::data.action.action		= ACT_WALK_FWD;

	inherited::data.accelerated		= true;
	inherited::data.braking			= false;
	inherited::data.accel_type 		= eAT_Calm;

	inherited::data.completion_dist	= 2.f;
	inherited::data.action.sound_type	= MonsterSound::eMonsterSoundIdle;
	inherited::data.action.sound_delay = inherited::object->db().m_dwIdleSndDelay;
	inherited::data.time_to_rebuild	= 1500;
}


TEMPLATE_SPECIALIZATION
void CStateChimeraThreatenWalkAbstract::execute()
{
	inherited::data.point				= inherited::object->EnemyMan.get_enemy_position	();
	inherited::data.vertex				= inherited::object->EnemyMan.get_enemy_vertex		();

	inherited::execute();
}

#define DISTANCE_TO_ENEMY		5.f

TEMPLATE_SPECIALIZATION
bool CStateChimeraThreatenWalkAbstract::check_completion()
{	
	if (inherited::check_completion()) return true;

	float dist_to_enemy = inherited::object->EnemyMan.get_enemy_position().distance_to(inherited::object->Position());
	if (dist_to_enemy < DISTANCE_TO_ENEMY) return true;

	return false;
}

#define MAX_DISTANCE_TO_ENEMY	8.f

TEMPLATE_SPECIALIZATION
bool CStateChimeraThreatenWalkAbstract::check_start_conditions()
{
	float dist_to_enemy = inherited::object->EnemyMan.get_enemy_position().distance_to(inherited::object->Position());
	if (dist_to_enemy < MAX_DISTANCE_TO_ENEMY) return true;
	return false;
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateChimeraThreatenWalkAbstract

