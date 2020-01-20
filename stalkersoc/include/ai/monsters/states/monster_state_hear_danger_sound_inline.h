#pragma once

#include "state_hide_from_point.h"
#include "state_look_unprotected_area.h"
#include "state_custom_action.h"
#include "monster_state_home_point_danger.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterHearDangerousSoundAbstract CStateMonsterHearDangerousSound<_Object>

TEMPLATE_SPECIALIZATION
CStateMonsterHearDangerousSoundAbstract::CStateMonsterHearDangerousSound(_Object *obj) : inherited(obj)
{
	inherited::add_state	(eStateHearDangerousSound_Hide,				xr_new<CStateMonsterHideFromPoint<_Object> >(obj));
	inherited::add_state	(eStateHearDangerousSound_FaceOpenPlace,	xr_new<CStateMonsterLookToUnprotectedArea<_Object> >(obj));
	inherited::add_state	(eStateHearDangerousSound_StandScared,		xr_new<CStateMonsterCustomAction<_Object> >(obj));
	inherited::add_state	(eStateHearDangerousSound_Home,				xr_new<CStateMonsterDangerMoveToHomePoint<_Object> >(obj));
}

TEMPLATE_SPECIALIZATION
void CStateMonsterHearDangerousSoundAbstract::reselect_state()
{
	if (inherited::get_state(eStateHearDangerousSound_Home)->check_start_conditions())	{
		inherited::select_state(eStateHearDangerousSound_Home);
		return;
	}

	if (inherited::prev_substate == u32(-1)){
		inherited::select_state(eStateHearDangerousSound_Hide);
		return;
	}

	if (inherited::prev_substate == eStateHearDangerousSound_Hide) {
		inherited::select_state(eStateHearDangerousSound_FaceOpenPlace);
		return;
	}

	inherited::select_state(eStateHearDangerousSound_StandScared);
}

TEMPLATE_SPECIALIZATION
void CStateMonsterHearDangerousSoundAbstract::setup_substates()
{
	auto state =this-> get_state_current();

	if (inherited::current_substate == eStateHearDangerousSound_Hide) {
		SStateHideFromPoint data;

		Fvector run_away_point;
		Fvector dir;
		dir.sub	(inherited::object->Position(), inherited::object->SoundMemory.GetSound().position);
		dir.normalize_safe();
		run_away_point.mad(inherited::object->Position(), dir, 1.f);
		
		data.point				= run_away_point;
		data.accelerated		= true;
		data.braking			= false;
		data.accel_type			= eAT_Aggressive;
		data.distance			= 40.f;
		data.action.action		= ACT_RUN;
		data.action.sound_type	= MonsterSound::eMonsterSoundPanic;
		data.action.sound_delay = inherited::object->db().m_dwAttackSndDelay;

		state->fill_data_with(&data, sizeof(SStateHideFromPoint));

		return;
	}

	if (inherited::current_substate == eStateHearDangerousSound_FaceOpenPlace) {
		SStateDataAction data;
		data.action			= ACT_STAND_IDLE;
		data.spec_params	= ASP_STAND_SCARED;
		data.time_out		= 2000;
		data.sound_type	= MonsterSound::eMonsterSoundPanic;
		data.sound_delay = inherited::object->db().m_dwAttackSndDelay;

		state->fill_data_with(&data, sizeof(SStateDataAction));
		
		return;
	}

	if (inherited::current_substate == eStateHearDangerousSound_StandScared) {
		SStateDataAction data;
		data.action			= ACT_STAND_IDLE;
		data.spec_params	= ASP_STAND_SCARED;
		data.sound_type	= MonsterSound::eMonsterSoundPanic;
		data.sound_delay = inherited::object->db().m_dwAttackSndDelay;

		state->fill_data_with(&data, sizeof(SStateDataAction));

		return;
	}
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterHearDangerousSoundAbstract 
