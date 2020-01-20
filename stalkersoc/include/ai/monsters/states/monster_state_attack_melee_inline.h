#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterAttackMeleeAbstract CStateMonsterAttackMelee<_Object>

TEMPLATE_SPECIALIZATION
CStateMonsterAttackMeleeAbstract::CStateMonsterAttackMelee(_Object *obj) : inherited(obj)
{
}

TEMPLATE_SPECIALIZATION
CStateMonsterAttackMeleeAbstract::~CStateMonsterAttackMelee()
{
}


TEMPLATE_SPECIALIZATION
void CStateMonsterAttackMeleeAbstract::execute()
{
	inherited::object->set_action			(ACT_ATTACK);
	if (inherited::object->control().direction().is_face_target(inherited::object->EnemyMan.get_enemy(), XrMath::PI_DIV_3))
		inherited::object->dir().face_target	(inherited::object->EnemyMan.get_enemy(), 800);
	else 
		inherited::object->dir().face_target	(inherited::object->EnemyMan.get_enemy(), 0, deg(15));

	inherited::object->set_state_sound		(MonsterSound::eMonsterSoundAggressive);
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterAttackMeleeAbstract::check_start_conditions()
{
	return (
		inherited::object->MeleeChecker.can_start_melee(inherited::object->EnemyMan.get_enemy()) &&
		inherited::object->EnemyMan.see_enemy_now()
	);
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterAttackMeleeAbstract::check_completion()
{
	return (inherited::object->MeleeChecker.should_stop_melee(inherited::object->EnemyMan.get_enemy()));
}


