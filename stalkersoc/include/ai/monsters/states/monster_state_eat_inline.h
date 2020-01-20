#pragma once

#include "state_data.h"
#include "state_move_to_point.h"
#include "state_hide_from_point.h"
#include "state_custom_action.h"
#include "monster_state_eat_eat.h"
#include "monster_state_eat_drag.h"
#include "../../../PhysicsShell.h"
#include "../../../PHMovementControl.h"
#include "../../../CharacterPhysicsSupport.h"
#ifdef _DEBUG
#	include "../../../level.h"
#	include "../../../level_debug.h"
#endif

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterEatAbstract CStateMonsterEat<_Object>

TEMPLATE_SPECIALIZATION
CStateMonsterEatAbstract::CStateMonsterEat(_Object *obj) : inherited(obj)
{
	inherited::add_state	(eStateEat_CorpseApproachRun,	xr_new<CStateMonsterMoveToPoint<_Object> >(obj));
	inherited::add_state	(eStateEat_CorpseApproachWalk,	xr_new<CStateMonsterMoveToPoint<_Object> >(obj));
	inherited::add_state	(eStateEat_CheckCorpse,			xr_new<CStateMonsterCustomAction<_Object> >(obj));
	inherited::add_state	(eStateEat_Eat,					xr_new<CStateMonsterEating<_Object> >(obj));
	inherited::add_state	(eStateEat_WalkAway,			xr_new<CStateMonsterHideFromPoint<_Object> >(obj));
	inherited::add_state	(eStateEat_Rest,				xr_new<CStateMonsterCustomAction<_Object> >(obj));
	inherited::add_state	(eStateEat_Drag,				xr_new<CStateMonsterDrag<_Object> >(obj));
}

TEMPLATE_SPECIALIZATION
CStateMonsterEatAbstract::~CStateMonsterEat()
{
}

TEMPLATE_SPECIALIZATION
void CStateMonsterEatAbstract::reinit()
{
	inherited::reinit();
	
	m_time_last_eat = 0;
}

TEMPLATE_SPECIALIZATION
void CStateMonsterEatAbstract::initialize()
{
	inherited::initialize();
	corpse = inherited::object->CorpseMan.get_corpse();

	monster_squad().get_squad(inherited::object)->lock_corpse(inherited::object->CorpseMan.get_corpse());
}

TEMPLATE_SPECIALIZATION
void CStateMonsterEatAbstract::finalize()
{
	inherited::finalize();

	monster_squad().get_squad(inherited::object)->unlock_corpse(inherited::object->CorpseMan.get_corpse());
}

TEMPLATE_SPECIALIZATION
void CStateMonsterEatAbstract::critical_finalize()
{
	inherited::critical_finalize();

	monster_squad().get_squad(inherited::object)->unlock_corpse(inherited::object->CorpseMan.get_corpse());
}


TEMPLATE_SPECIALIZATION
void CStateMonsterEatAbstract::reselect_state()
{
	if (inherited::prev_substate == u32(-1)) {inherited::select_state(eStateEat_CorpseApproachRun);return;}
	if (inherited::prev_substate == eStateEat_CorpseApproachRun) { inherited::select_state(eStateEat_CheckCorpse); return; }
	
	if (inherited::prev_substate == eStateEat_CheckCorpse) { 
		if (inherited::object->ability_can_drag()) inherited::select_state(eStateEat_Drag);
		else {							
			if (inherited::get_state(eStateEat_Eat)->check_start_conditions())
				inherited::select_state(eStateEat_Eat);					
			else 
				inherited::select_state(eStateEat_CorpseApproachWalk);
		}
		return; 
	}

	if (inherited::prev_substate == eStateEat_Drag)		{ 
		if (inherited::get_state(eStateEat_Eat)->check_start_conditions())
			inherited::select_state(eStateEat_Eat);					
		else 
			inherited::select_state(eStateEat_CorpseApproachWalk);
		return; 
	}	

	if (inherited::prev_substate == eStateEat_Eat){
		m_time_last_eat = time();

		if (!hungry()) 
			inherited::select_state(eStateEat_WalkAway); 
		else 
			inherited::select_state(eStateEat_CorpseApproachWalk);
		return;
	}

	if (inherited::prev_substate == eStateEat_CorpseApproachWalk){
		if (inherited::get_state(eStateEat_Eat)->check_start_conditions())
			inherited::select_state(eStateEat_Eat); 
		else 
			inherited::select_state(eStateEat_CorpseApproachWalk);
		return;
	}

	if (inherited::prev_substate == eStateEat_WalkAway)	{ inherited::select_state(eStateEat_Rest);		return; }
	if (inherited::prev_substate == eStateEat_Rest)		{ inherited::select_state(eStateEat_Rest);		return; }
}

TEMPLATE_SPECIALIZATION
void CStateMonsterEatAbstract::setup_substates()
{
	state_ptr state = inherited::get_state_current();

	if (inherited::current_substate == eStateEat_CorpseApproachRun) {

		// Определить позицию ближайшей боны у трупа
		Fvector nearest_bone_pos;
		const CEntityAlive *corpse1 = inherited::object->CorpseMan.get_corpse();
		if ((corpse1->m_pPhysicsShell == NULL) || (!corpse1->m_pPhysicsShell->isActive())) {
			nearest_bone_pos	= corpse1->Position(); 
		} else nearest_bone_pos = inherited::object->character_physics_support()->movement()->PHCaptureGetNearestElemPos(corpse1);

#ifdef _DEBUG
		DBG().level_info(this).clear		();
		Fvector pos1;
		pos1.set(nearest_bone_pos);
		pos1.y+=20.f;

		DBG().level_info(this).add_item		(nearest_bone_pos, pos1, COLOR_GREEN);
#endif
		SStateDataMoveToPoint data;
		data.point			= nearest_bone_pos;
		data.vertex			= u32(-1);
		data.action.action	= ACT_RUN;
		data.accelerated	= true;
		data.braking		= true;
		data.accel_type 	= eAT_Calm;
		data.completion_dist= inherited::object->db().m_fDistToCorpse;
		data.action.sound_type	= MonsterSound::eMonsterSoundIdle;
		data.action.sound_delay = inherited::object->db().m_dwIdleSndDelay;

		state->fill_data_with(&data, sizeof(SStateDataMoveToPoint));
		return;
	}

	if (inherited::current_substate == eStateEat_CheckCorpse) {
		SStateDataAction data;
		data.action			= ACT_STAND_IDLE;
		data.spec_params	= 0;
		data.time_out		= 1500;
		data.sound_type	= MonsterSound::eMonsterSoundEat;
		data.sound_delay = inherited::object->db().m_dwEatSndDelay;

		state->fill_data_with(&data, sizeof(SStateDataAction));

		return;
	}
	
	if (inherited::current_substate == eStateEat_WalkAway) {
		SStateHideFromPoint data;
		
		data.point					= inherited::object->CorpseMan.get_corpse_position();
		data.action.action			= ACT_WALK_FWD;
		data.distance				= 15.f;	
		data.accelerated			= true;
		data.braking				= true;
		data.accel_type				= eAT_Calm;
		data.cover_min_dist			= 20.f;
		data.cover_max_dist			= 30.f;
		data.cover_search_radius	= 25.f;
		data.action.sound_type	= MonsterSound::eMonsterSoundIdle;
		data.action.sound_delay = inherited::object->db().m_dwIdleSndDelay;

		state->fill_data_with(&data, sizeof(SStateHideFromPoint));

		return;
	}

	if (inherited::current_substate == eStateEat_Rest) {
		SStateDataAction data;
		data.action			= ACT_REST;
		data.spec_params	= 0;
		data.time_out		= 8500;
		data.sound_type	= MonsterSound::eMonsterSoundIdle;
		data.sound_delay = inherited::object->db().m_dwIdleSndDelay;

		state->fill_data_with(&data, sizeof(SStateDataAction));
		return;
	}

	if (inherited::current_substate == eStateEat_CorpseApproachWalk) {
		
		// Определить позицию ближайшей боны у трупа
		Fvector nearest_bone_pos;
		const CEntityAlive *corpse1 = inherited::object->CorpseMan.get_corpse();
		if ((corpse1->m_pPhysicsShell == NULL) || (!corpse1->m_pPhysicsShell->isActive())) {
			nearest_bone_pos	= corpse1->Position(); 
		} else nearest_bone_pos = inherited::object->character_physics_support()->movement()->PHCaptureGetNearestElemPos(corpse1);
		
		SStateDataMoveToPoint data;
		data.point			= nearest_bone_pos;
		data.vertex			= u32(-1);
		data.action.action	= ACT_WALK_FWD;
		data.accelerated	= true;
		data.braking		= true;
		data.accel_type 	= eAT_Calm;
		data.completion_dist= inherited::object->db().m_fDistToCorpse;
		data.action.sound_type	= MonsterSound::eMonsterSoundIdle;
		data.action.sound_delay = inherited::object->db().m_dwIdleSndDelay;

		state->fill_data_with(&data, sizeof(SStateDataMoveToPoint));
		return;
	}
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterEatAbstract::check_completion()
{
	if (corpse != inherited::object->CorpseMan.get_corpse()) return true;
	if (!hungry()) return true;

	return false;
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterEatAbstract::check_start_conditions()
{
	return (
		inherited::object->CorpseMan.get_corpse() && 
		inherited::object->Home->at_home(inherited::object->CorpseMan.get_corpse()->Position()) &&
		hungry() && 
		!monster_squad().get_squad(inherited::object)->is_locked_corpse(inherited::object->CorpseMan.get_corpse())
	);
		
}

#define TIME_NOT_HUNGRY 20000

TEMPLATE_SPECIALIZATION
bool CStateMonsterEatAbstract::hungry()
{
	return ((m_time_last_eat == 0) || (m_time_last_eat + TIME_NOT_HUNGRY < time()));
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterEatAbstract
