#pragma once

#include "chimera_state_threaten_steal.h"
#include "chimera_state_threaten_walk.h"
#include "chimera_state_threaten_roar.h"


#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateChimeraThreatenAbstract CStateChimeraThreaten<_Object>

TEMPLATE_SPECIALIZATION
CStateChimeraThreatenAbstract::CStateChimeraThreaten(_Object *obj) : inherited(obj)
{
	inherited::add_state(eStateWalk,		xr_new<CStateChimeraThreatenWalk<_Object> >	(obj));
	inherited::add_state(eStateThreaten,	xr_new<CStateChimeraThreatenRoar<_Object> >	(obj));
	inherited::add_state(eStateSteal,		xr_new<CStateChimeraThreatenSteal<_Object> >(obj));
}

TEMPLATE_SPECIALIZATION
CStateChimeraThreatenAbstract::~CStateChimeraThreaten()
{
}

TEMPLATE_SPECIALIZATION
void CStateChimeraThreatenAbstract::reinit()
{
	inherited::reinit	();

	m_last_time_threaten = 0;
}


#define MIN_DIST_TO_ENEMY	3.f
#define MORALE_THRESHOLD	0.8f
#define THREATEN_DELAY		10000

TEMPLATE_SPECIALIZATION
bool CStateChimeraThreatenAbstract::check_start_conditions()
{
	if (inherited::object->tfGetRelationType(inherited::object->EnemyMan.get_enemy()) == ALife::eRelationTypeWorstEnemy) return false;
	if (inherited::object->Position().distance_to(inherited::object->EnemyMan.get_enemy_position()) < MIN_DIST_TO_ENEMY) return false;
	if (inherited::object->HitMemory.is_hit())						return false;
	if (inherited::object->hear_dangerous_sound)					return false;
	if (m_last_time_threaten + THREATEN_DELAY > Device.dwTimeGlobal) return false;

	return true;
}

TEMPLATE_SPECIALIZATION
bool CStateChimeraThreatenAbstract::check_completion()
{
	if (inherited::object->Position().distance_to(inherited::object->EnemyMan.get_enemy_position()) < MIN_DIST_TO_ENEMY) return true;
	if (inherited::object->HitMemory.is_hit()) return true;
	if (inherited::object->tfGetRelationType(inherited::object->EnemyMan.get_enemy()) == ALife::eRelationTypeWorstEnemy) return true;

	return false;
}

TEMPLATE_SPECIALIZATION
void CStateChimeraThreatenAbstract::initialize()
{
	inherited::initialize	();
	inherited::object->SetUpperState	();
}

TEMPLATE_SPECIALIZATION
void CStateChimeraThreatenAbstract::reselect_state()
{
	if (inherited::prev_substate == u32(-1)) {
		inherited::select_state(eStateThreaten);
		return;
	}

	if (inherited::prev_substate == eStateSteal) {
		inherited::select_state(eStateThreaten);
		return;
	}

	if (inherited::prev_substate == eStateThreaten) {
		if (inherited::get_state(eStateSteal)->check_start_conditions()) {
			inherited::select_state(eStateSteal);
			return;
		} else if (inherited::get_state(eStateWalk)->check_start_conditions()) {
			inherited::select_state(eStateWalk);
			return;
		}
	}

	inherited::select_state(eStateThreaten);
}

TEMPLATE_SPECIALIZATION
void CStateChimeraThreatenAbstract::finalize()
{
	inherited::finalize		();
	inherited::object->SetUpperState	(false);
	m_last_time_threaten	 = Device.dwTimeGlobal;
}

TEMPLATE_SPECIALIZATION
void CStateChimeraThreatenAbstract::critical_finalize()
{
	inherited::critical_finalize();
	inherited::object->SetUpperState	(false);
	m_last_time_threaten	 = Device.dwTimeGlobal;
}


#undef TEMPLATE_SPECIALIZATION
#undef CStateChimeraThreatenAbstract
