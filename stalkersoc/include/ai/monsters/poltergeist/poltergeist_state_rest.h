#pragma once

#include "../states/monster_state_rest.h"

template<typename _Object>
class	CPoltergeistStateRest : public CStateMonsterRest<_Object> {
protected:
	typedef CStateMonsterRest<_Object>		inherited;
public:
						CPoltergeistStateRest		(_Object *obj) : inherited(obj) {}
	virtual	void		execute					();
};

template<typename _Object>
void CPoltergeistStateRest<_Object>::execute()
{
	// check alife control
	bool captured_by_smart_terrain = false;

	if (inherited::prev_substate == eStateSmartTerrainTask) {
		if (!inherited::get_state(eStateSmartTerrainTask)->check_completion()) 
			captured_by_smart_terrain = true;
	} else if (inherited::get_state(eStateSmartTerrainTask)->check_start_conditions()) 
		captured_by_smart_terrain = true;

	if (captured_by_smart_terrain) inherited::select_state(eStateSmartTerrainTask);
	else {
		// check restrictions
		bool move_to_restrictor = false;

		if (inherited::prev_substate == eStateCustomMoveToRestrictor) {
			if (!inherited::get_state(eStateCustomMoveToRestrictor)->check_completion()) 
				move_to_restrictor = true;
		} else if (inherited::get_state(eStateCustomMoveToRestrictor)->check_start_conditions()) 
			move_to_restrictor = true;

		if (move_to_restrictor) inherited::select_state(eStateCustomMoveToRestrictor);
		else {
			// check home point
			bool move_to_home_point = false;

			if (inherited::prev_substate == eStateRest_MoveToHomePoint) {
				if (!inherited::get_state(eStateRest_MoveToHomePoint)->check_completion()) 
					move_to_home_point = true;
			} else if (inherited::get_state(eStateRest_MoveToHomePoint)->check_start_conditions()) 
				move_to_home_point = true;

			if (move_to_home_point) inherited::select_state	(eStateRest_MoveToHomePoint);
			else					inherited::select_state	(eStateRest_WalkGraphPoint);
		}
	}

	inherited::get_state_current()->execute();
	inherited::prev_substate = inherited::current_substate;
}

