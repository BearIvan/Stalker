#pragma once

#include "bloodsucker_vampire_execute.h"
#include "../states/state_hide_from_point.h"
#include "bloodsucker_vampire_approach.h"
#include "bloodsucker_vampire_hide.h"
#include "../../../clsid_game.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateBloodsuckerVampireAbstract CStateBloodsuckerVampire<_Object>

#define RUN_AWAY_DISTANCE			50.f

TEMPLATE_SPECIALIZATION
CStateBloodsuckerVampireAbstract::CStateBloodsuckerVampire(_Object *obj) : inherited(obj)
{
	inherited::add_state	(eStateVampire_ApproachEnemy,	xr_new<CStateBloodsuckerVampireApproach<_Object> >	(obj));
	inherited::add_state	(eStateVampire_Execute,			xr_new<CStateBloodsuckerVampireExecute<_Object> >	(obj));
	inherited::add_state	(eStateVampire_RunAway,			xr_new<CStateMonsterHideFromPoint<_Object> >		(obj));
	inherited::add_state	(eStateVampire_Hide,			xr_new<CStateBloodsuckerVampireHide<_Object> >		(obj));
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireAbstract::reinit()
{
	inherited::reinit	();
	
	m_time_last_vampire	= 0;
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireAbstract::initialize()
{
	inherited::initialize						();
	inherited::object->start_invisible_predator			();

	enemy	= inherited::object->EnemyMan.get_enemy		();

	inherited::object->sound().play						(CAI_Bloodsucker::eVampireStartHunt);
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireAbstract::reselect_state()
{
	u32 state_id = u32(-1);
		
	// check if we can start execute
	if (inherited::prev_substate == eStateVampire_ApproachEnemy) {
		if (inherited::get_state(eStateVampire_Execute)->check_start_conditions())		state_id = eStateVampire_Execute;
	}

	// check if we executed 
	if (inherited::prev_substate == eStateVampire_Execute)
		state_id = eStateVampire_Hide;
	
	// check if reach time in vampire state is out - then hide
	if (inherited::prev_substate == eStateVampire_ApproachEnemy) 
		state_id = eStateVampire_Hide;

	// check if we hiding - then hide again
	if ( inherited::prev_substate == eStateVampire_Hide) 
		state_id = eStateVampire_Hide;

	// else just 
	if (state_id == u32(-1)) state_id = eStateVampire_ApproachEnemy;

	inherited::select_state(state_id);	
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireAbstract::check_force_state()
{
	// check if we can start execute
	if (inherited::prev_substate == eStateVampire_ApproachEnemy) {
		if (inherited::get_state(eStateVampire_Execute)->check_start_conditions())
			inherited::current_substate = u32(-1);
	}
}


TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireAbstract::finalize()
{
	inherited::finalize();

	inherited::object->stop_invisible_predator	();
	m_time_last_vampire				= Device.dwTimeGlobal;
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireAbstract::critical_finalize()
{
	inherited::critical_finalize	();
	
	inherited::object->stop_invisible_predator	();
	m_time_last_vampire				= Device.dwTimeGlobal;
}

TEMPLATE_SPECIALIZATION
bool CStateBloodsuckerVampireAbstract::check_start_conditions()
{
	if (!inherited::object->WantVampire()) return false;
	if (inherited::object->berserk_always) return false;
	
	// �������� �� ���� �������
	const CEntityAlive *enemy1 = inherited::object->EnemyMan.get_enemy();
	if (enemy1->CLS_ID != CLSID_OBJECT_ACTOR)		return false;
	if (!inherited::object->EnemyMan.see_enemy_now())			return false;
	if (inherited::object->CControlledActor::is_controlling())	return false;

	const CActor *actor = smart_cast<const CActor *>(enemy1);
	VERIFY(actor);
	if (actor->input_external_handler_installed()) return false;

	if (m_time_last_vampire + inherited::object->m_vampire_min_delay > Device.dwTimeGlobal) return false;

	return true;
}

TEMPLATE_SPECIALIZATION
bool CStateBloodsuckerVampireAbstract::check_completion()
{
	// ���� ������
	if ((inherited::current_substate == eStateVampire_Hide) && 
		inherited::get_state_current()->check_completion())	return true;

	// ���� ���� ���������
	if (enemy != inherited::object->EnemyMan.get_enemy())		return true;
	
	// ���� ������ ��� ��������� ������ ��������
	if ((inherited::current_substate != eStateVampire_Execute) && 
		inherited::object->CControlledActor::is_controlling())	return true;

	return false;
}


TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireAbstract::setup_substates()
{
	state_ptr state = inherited::get_state_current();

	if (inherited::current_substate == eStateVampire_RunAway) {

		SStateHideFromPoint		data;
		data.point				= inherited::object->EnemyMan.get_enemy_position();
		data.accelerated		= true;
		data.braking			= false;
		data.accel_type			= eAT_Aggressive;
		data.distance			= RUN_AWAY_DISTANCE;
		data.action.action		= ACT_RUN;
		data.action.sound_type	= MonsterSound::eMonsterSoundAggressive;
		data.action.sound_delay = inherited::object->db().m_dwAttackSndDelay;
		data.action.time_out	= 15000;

		state->fill_data_with(&data, sizeof(SStateHideFromPoint));

		return;
	}
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateBloodsuckerVampireAbstract

