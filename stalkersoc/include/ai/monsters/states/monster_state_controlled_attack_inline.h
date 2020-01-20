#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterControlledAttackAbstract CStateMonsterControlledAttack<_Object>

TEMPLATE_SPECIALIZATION
CStateMonsterControlledAttackAbstract::CStateMonsterControlledAttack(_Object *obj) : inherited(obj)
{
}

TEMPLATE_SPECIALIZATION
void CStateMonsterControlledAttackAbstract::initialize()
{
	inherited::initialize();
	inherited::object->EnemyMan.force_enemy(get_enemy());
}

TEMPLATE_SPECIALIZATION
void CStateMonsterControlledAttackAbstract::execute()
{
	inherited::object->EnemyMan.force_enemy(get_enemy());
	inherited::execute();
}

TEMPLATE_SPECIALIZATION
void CStateMonsterControlledAttackAbstract::finalize()
{
	inherited::finalize();
	inherited::object->EnemyMan.unforce_enemy();
}

TEMPLATE_SPECIALIZATION
void CStateMonsterControlledAttackAbstract::critical_finalize()
{
	inherited::critical_finalize();
	inherited::object->EnemyMan.unforce_enemy();
}

TEMPLATE_SPECIALIZATION
const CEntityAlive *CStateMonsterControlledAttackAbstract::get_enemy()
{
	CControlledEntityBase *entity = smart_cast<CControlledEntityBase *>(inherited::object);
	VERIFY(entity);
	return smart_cast<const CEntityAlive*>(entity->get_data().m_object);
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterControlledAttackAbstract
