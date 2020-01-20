#pragma once

#include "monster_state_controlled_attack.h"
#include "monster_state_controlled_follow.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterControlledAbstract CStateMonsterControlled<_Object>

TEMPLATE_SPECIALIZATION
CStateMonsterControlledAbstract::CStateMonsterControlled(_Object *obj) : inherited(obj)
{
	inherited::add_state	(eStateControlled_Attack,		xr_new<CStateMonsterControlledAttack<_Object> >	(obj));
	inherited::add_state	(eStateControlled_Follow,		xr_new<CStateMonsterControlledFollow<_Object> >	(obj));	
}

TEMPLATE_SPECIALIZATION
void CStateMonsterControlledAbstract::execute()
{
	switch (inherited::object->get_data().m_task) {
		case eTaskFollow:	inherited::select_state(eStateControlled_Follow);	break;
		case eTaskAttack:	{
			// проверить валидность данных атаки
			const CEntity *enemy = inherited::object->get_data().m_object;
			if (!enemy || enemy->getDestroy() || !enemy->g_Alive()) {
				inherited::object->get_data().m_object = inherited::object->get_controller();
				inherited::select_state(eStateControlled_Follow);
			} else 
				inherited::select_state(eStateControlled_Attack);	break;
		}
		default:			NODEFAULT;
	} 
	
	this->get_state_current()->execute();

	inherited::prev_substate = inherited::current_substate;

}

#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterControlledAbstract
