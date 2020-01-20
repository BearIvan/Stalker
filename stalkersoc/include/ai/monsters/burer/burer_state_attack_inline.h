#pragma once

#include "burer_state_attack_tele.h"
#include "burer_state_attack_gravi.h"
#include "burer_state_attack_melee.h"
#include "../states/state_look_point.h"
#include "../states/state_move_to_restrictor.h"
#include "burer_state_attack_run_around.h"

#define GRAVI_PERCENT		80
#define TELE_PERCENT		50
#define RUN_AROUND_PERCENT	20

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateBurerAttackAbstract CStateBurerAttack<_Object>

TEMPLATE_SPECIALIZATION
CStateBurerAttackAbstract::CStateBurerAttack(_Object *obj) : inherited(obj)
{
	inherited::add_state(eStateBurerAttack_Tele,		xr_new<CStateBurerAttackTele<_Object> >		(obj));
	inherited::add_state(eStateBurerAttack_Gravi,		xr_new<CStateBurerAttackGravi<_Object> >	(obj));
	inherited::add_state(eStateBurerAttack_Melee,		xr_new<CStateBurerAttackMelee<_Object> >	(obj));
	
	inherited::add_state(eStateBurerAttack_FaceEnemy,	xr_new<CStateMonsterLookToPoint<_Object> >	(obj));
	inherited::add_state(eStateBurerAttack_RunAround,	xr_new<CStateBurerAttackRunAround<_Object> >(obj));

	inherited::add_state(eStateCustomMoveToRestrictor,	xr_new<CStateMonsterMoveToRestrictor<_Object> >(obj));
}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackAbstract::initialize()
{
	inherited::initialize	();
	m_force_gravi			= false;
}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackAbstract::reselect_state()
{
	if (inherited::get_state(eStateBurerAttack_Melee)->check_start_conditions()) {
		inherited::select_state(eStateBurerAttack_Melee);
		return;
	}

	if (m_force_gravi) {
		m_force_gravi = false;

		if (inherited::get_state(eStateBurerAttack_Gravi)->check_start_conditions()) {
			inherited::select_state		(eStateBurerAttack_Gravi);
			return;
		}
	}

	if (inherited::get_state(eStateCustomMoveToRestrictor)->check_start_conditions()) {
		inherited::select_state(eStateCustomMoveToRestrictor);
		return;
	}

	bool enable_gravi	= false;//inherited::get_state(eStateBurerAttack_Gravi)->check_start_conditions	();
	bool enable_tele	= inherited::get_state(eStateBurerAttack_Tele)->check_start_conditions		();

	if (!enable_gravi && !enable_tele) {
		if (inherited::prev_substate == eStateBurerAttack_RunAround) 
			inherited::select_state(eStateBurerAttack_FaceEnemy);
		else 	
			inherited::select_state(eStateBurerAttack_RunAround);
		return;
	}

	if (enable_gravi && enable_tele) {

		u32 rnd_val = ::Random.randI(GRAVI_PERCENT + TELE_PERCENT + RUN_AROUND_PERCENT);
		u32 cur_val = GRAVI_PERCENT;

		if (rnd_val < cur_val) {
			inherited::select_state(eStateBurerAttack_Gravi);
			return;
		}

		cur_val += TELE_PERCENT;
		if (rnd_val < cur_val) {
			inherited::select_state(eStateBurerAttack_Tele);
			return;
		}

		inherited::select_state(eStateBurerAttack_RunAround);
		return;
	}

	if ((inherited::prev_substate == eStateBurerAttack_RunAround) || (inherited::prev_substate == eStateBurerAttack_FaceEnemy)) {
		if (enable_gravi) inherited::select_state(eStateBurerAttack_Gravi);
		else inherited::select_state(eStateBurerAttack_Tele);
	} else {
		inherited::select_state(eStateBurerAttack_RunAround);
	}
}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackAbstract::setup_substates()
{
	auto state = this->get_state_current();

	if (inherited::current_substate == eStateBurerAttack_FaceEnemy) {
		SStateDataLookToPoint data;
		
		data.point				= inherited::object->EnemyMan.get_enemy()->Position(); 
		data.action.action		= ACT_STAND_IDLE;
		data.action.sound_type	= MonsterSound::eMonsterSoundAggressive;
		data.action.sound_delay = inherited::object->db().m_dwAttackSndDelay;

		state->fill_data_with	(&data, sizeof(SStateDataLookToPoint));
		return;
	}

}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackAbstract::check_force_state()
{
	// check if we can start execute
	if ((inherited::current_substate == eStateCustomMoveToRestrictor) || (inherited::prev_substate == eStateBurerAttack_RunAround)) {
		if (inherited::get_state(eStateBurerAttack_Gravi)->check_start_conditions()) {
			inherited::current_substate	= u32(-1);
			m_force_gravi		= true;
		}
	}
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateBurerAttackAbstract
