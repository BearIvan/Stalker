#pragma once

//#include "bloodsucker_attack_state_hide.h"
#include "../states/state_move_to_point.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CBloodsuckerStateAttackAbstract CBloodsuckerStateAttack<_Object>

TEMPLATE_SPECIALIZATION
CBloodsuckerStateAttackAbstract::CBloodsuckerStateAttack(_Object *obj) : inherited_attack(obj)
{
	//inherited_attack::add_state(eStateAttack_Hide,	xr_new<CBloodsuckerStateAttackHide<_Object> > (obj));
	inherited_attack::add_state	(eStateAttack_Hide,	xr_new<CStateMonsterMoveToPointEx<_Object> >(obj));
}

TEMPLATE_SPECIALIZATION
CBloodsuckerStateAttackAbstract::~CBloodsuckerStateAttack()
{
}

TEMPLATE_SPECIALIZATION
void CBloodsuckerStateAttackAbstract::initialize()
{
	inherited_attack::initialize	();
	m_time_stop_invis		= 0;
}

TEMPLATE_SPECIALIZATION
void CBloodsuckerStateAttackAbstract::finalize()
{
	inherited_attack::finalize();
	inherited_attack::object->stop_invisible_predator();
}

TEMPLATE_SPECIALIZATION
void CBloodsuckerStateAttackAbstract::critical_finalize()
{
	inherited_attack::critical_finalize();
	inherited_attack::object->stop_invisible_predator();
}

#define	INVIS_ACTIVATE_DELAY	3000
#define INVIS_DIST_TO_ENEMY		5.f

TEMPLATE_SPECIALIZATION
void CBloodsuckerStateAttackAbstract::execute()
{
	bool selected = false;

	if (inherited_attack::check_home_point()) {
		inherited_attack::select_state	(eStateAttack_MoveToHomePoint);
		selected		= true;
	} else if (check_hiding()) {
		inherited_attack::select_state	(eStateAttack_Hide);
		selected		= true;
	} else if (inherited_attack::check_steal_state()) {
		inherited_attack::select_state	(eStateAttack_Steal);
		selected		= true;
	} else if (inherited_attack::check_camp_state()) {
		inherited_attack::select_state	(eStateAttackCamp);
		selected		= true;
	} else if (inherited_attack::check_find_enemy_state()) {
		inherited_attack::select_state	(eStateAttack_FindEnemy);
		selected		= true;
	} else if (inherited_attack::check_run_attack_state()) {
		inherited_attack::select_state	(eStateAttack_RunAttack);
		selected		= true;
	}

	if (!selected) {
		// определить тип атаки
		bool b_melee = false; 

		if (inherited_attack::prev_substate == eStateAttack_Melee) {
			if (!inherited_attack::get_state_current()->check_completion()) {
				b_melee = true;
			}
		} else if (inherited_attack::get_state(eStateAttack_Melee)->check_start_conditions()) {
			b_melee = true;
		}
		
		if (!b_melee && (inherited_attack::prev_substate == eStateAttack_Melee)) {
			inherited_attack::select_state	(eStateAttack_Hide);
		} else
		// установить целевое состояние
		if (b_melee) {  
			// check if enemy is behind me for a long time
			// [TODO] make specific state and replace run_away state (to avoid ratation jumps)
			//if (check_behinder()) 
			//	inherited_attack::select_state(eStateAttack_RunAway);
			//else 
				inherited_attack::select_state(eStateAttack_Melee);
		}
		else inherited_attack::select_state(eStateAttack_Run);
	}

	// clear behinder var if not melee state selected
	if (inherited_attack::current_substate != eStateAttack_Melee) inherited_attack::m_time_start_check_behinder = 0;
	update_invisibility				();
	
	inherited_attack::get_state_current()->execute	();
	inherited_attack::prev_substate = inherited_attack::current_substate;

	// Notify squad	
	CMonsterSquad *squad	= monster_squad().get_squad(inherited_attack::object);
	if (squad) {
		SMemberGoal			goal;

		goal.type			= MG_AttackEnemy;
		goal.entity			= const_cast<CEntityAlive*>(inherited_attack::object->EnemyMan.get_enemy());

		squad->UpdateGoal	(inherited_attack::object, goal);
	}
	//////////////////////////////////////////////////////////////////////////
}



TEMPLATE_SPECIALIZATION
void CBloodsuckerStateAttackAbstract::update_invisibility()
{
	if (inherited_attack::object->state_invisible) {
		// check conditions to stop invis
		if (inherited_attack::current_substate == eStateAttack_Melee) {
			inherited_attack::object->stop_invisible_predator	();
			m_time_stop_invis				= time();		
		}
	} else {
		// check conditions to start invis
		if (inherited_attack::current_substate == eStateAttack_Hide) {
			inherited_attack::object->start_invisible_predator();
		} else 
		if ((inherited_attack::current_substate == eStateAttack_Run) && (inherited_attack::object->EnemyMan.get_enemy()->Position().distance_to(inherited_attack::object->Position()) > INVIS_DIST_TO_ENEMY)) {
			if (m_time_stop_invis + INVIS_ACTIVATE_DELAY < time()) 
				inherited_attack::object->start_invisible_predator();
		}
	}
}

TEMPLATE_SPECIALIZATION
bool CBloodsuckerStateAttackAbstract::check_hiding()
{
	if (inherited_attack::current_substate == eStateAttack_Hide) 
		if (!inherited_attack::get_state(eStateAttack_Melee)->check_start_conditions()) 
			if (!inherited_attack::get_state_current()->check_completion()) {
				//inherited_attack::object->path().set_use_dest_orient	(true);
				//inherited_attack::object->path().set_dest_direction	(Fvector().sub(inherited_attack::object->EnemyMan.get_enemy()->Position(),m_dir_point));
				return true;
			}

	return false;


	//if (inherited_attack::current_substate == eStateAttack_Melee) {
	//	if (inherited_attack::prev_substate != eStateAttack_Melee) {
	//		inherited_attack::object->stop_invisible_predator	();
	//		m_time_stop_invis				= time();
	//	}
	//	
	//	if (inherited_attack::get_state_current()->check_completion()) ret_value = true;
	//} else 
	//if (inherited_attack::current_substate == eStateAttack_Run) {
	//	if (inherited_attack::object->EnemyMan.get_enemy()->Position().distance_to(inherited_attack::object->Position()) > INVIS_DIST_TO_ENEMY) {
	//		if (!inherited_attack::object->state_invisible && (m_time_stop_invis + INVIS_ACTIVATE_DELAY < time())) 
	//			inherited_attack::object->start_invisible_predator();
	//	}
	//} 
	//		
	//return ret_value;
}

TEMPLATE_SPECIALIZATION
void CBloodsuckerStateAttackAbstract::setup_substates()
{
	auto state = this->get_state_current();

	if (inherited_attack::current_substate == eStateAttack_Hide) {

		SStateDataMoveToPointEx data;

		Fvector target_dir		= Random.randI(2) ? inherited_attack::object->XFORM().i : Fvector().set(inherited_attack::object->XFORM().i).invert();
		m_dir_point				= Fvector().mad(inherited_attack::object->Position(), target_dir, 2.5f);

		data.vertex				= 0;
		data.point				= m_dir_point;
		data.action.action		= ACT_RUN;
		data.action.time_out	= 1500;		// do not use time out
		data.completion_dist	= 1.f;		// get exactly to the point
		data.time_to_rebuild	= inherited_attack::object->get_attack_rebuild_time();		
		data.accelerated		= true;
		data.braking			= false;
		data.accel_type 		= eAT_Aggressive;
		data.action.sound_type	= MonsterSound::eMonsterSoundIdle;
		data.action.sound_delay = inherited_attack::object->db().m_dwIdleSndDelay;

		state->fill_data_with(&data, sizeof(SStateDataMoveToPointEx));

		return;
	}
}

#undef TEMPLATE_SPECIALIZATION
#undef CBloodsuckerStateAttackAbstract

