#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterFindEnemyAngryAbstract CStateMonsterFindEnemyAngry<_Object>

TEMPLATE_SPECIALIZATION
CStateMonsterFindEnemyAngryAbstract::CStateMonsterFindEnemyAngry(_Object *obj) : inherited(obj)
{
}

TEMPLATE_SPECIALIZATION
CStateMonsterFindEnemyAngryAbstract::~CStateMonsterFindEnemyAngry()
{
}

TEMPLATE_SPECIALIZATION
void CStateMonsterFindEnemyAngryAbstract::execute()
{
	inherited::object->set_action					(ACT_STAND_IDLE);
	inherited::object->anim().SetSpecParams		(ASP_THREATEN);
	inherited::object->set_state_sound				(MonsterSound::eMonsterSoundAggressive);
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterFindEnemyAngryAbstract::check_completion()
{	
	if (inherited::time_state_started + 4000 > Device.dwTimeGlobal) return false;
	return true;
}

