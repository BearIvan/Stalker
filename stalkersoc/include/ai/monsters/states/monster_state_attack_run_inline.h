#pragma once

#include "../ai_monster_squad.h"
#include "../ai_monster_squad_manager.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterAttackRunAbstract CStateMonsterAttackRun<_Object>

TEMPLATE_SPECIALIZATION
void CStateMonsterAttackRunAbstract::initialize()
{
	inherited::initialize();
	inherited::object->path().prepare_builder	();	
}

TEMPLATE_SPECIALIZATION
void CStateMonsterAttackRunAbstract::execute()
{
	// установка параметров функциональных блоков
	inherited::object->set_action						(ACT_RUN);
	inherited::object->anim().accel_activate			(eAT_Aggressive);
	inherited::object->anim().accel_set_braking		(false);
	inherited::object->path().set_target_point			(inherited::object->EnemyMan.get_enemy_position(), inherited::object->EnemyMan.get_enemy_vertex());
	inherited::object->path().set_rebuild_time			(inherited::object->get_attack_rebuild_time());
	inherited::object->path().set_use_covers			();
	inherited::object->path().set_cover_params			(0.1f, 30.f, 1.f, 30.f);
	inherited::object->path().set_try_min_time			(false);
	inherited::object->set_state_sound					(MonsterSound::eMonsterSoundAggressive);
	inherited::object->path().extrapolate_path			(true);

	
	// обработать squad инфо	
	inherited::object->path().set_use_dest_orient		(false);

	CMonsterSquad *squad	= monster_squad().get_squad(inherited::object);
	if (squad && squad->SquadActive()) {
		// Получить команду
		SSquadCommand command;
		squad->GetCommand(inherited::object, command);
		
		if (command.type == SC_ATTACK) {
			inherited::object->path().set_use_dest_orient	(true);
			inherited::object->path().set_dest_direction	(command.direction);
		} 
	}
}

TEMPLATE_SPECIALIZATION
void CStateMonsterAttackRunAbstract::finalize()
{
	inherited::finalize					();
	inherited::object->path().extrapolate_path	(false);
}

TEMPLATE_SPECIALIZATION
void CStateMonsterAttackRunAbstract::critical_finalize()
{
	inherited::critical_finalize		();
	inherited::object->path().extrapolate_path	(false);
}


TEMPLATE_SPECIALIZATION
bool CStateMonsterAttackRunAbstract::check_completion()
{
	float m_fDistMin	= inherited::object->MeleeChecker.get_min_distance		();
	float dist			= inherited::object->MeleeChecker.distance_to_enemy	(inherited::object->EnemyMan.get_enemy());

	if (dist < m_fDistMin)	return true;

	return false;
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterAttackRunAbstract::check_start_conditions()
{
	float m_fDistMax	= inherited::object->MeleeChecker.get_max_distance		();
	float dist			= inherited::object->MeleeChecker.distance_to_enemy	(inherited::object->EnemyMan.get_enemy());

	if (dist > m_fDistMax)	return true;

	return false;
}

