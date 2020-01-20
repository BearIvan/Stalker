#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterRestSleepAbstract CStateMonsterRestSleep<_Object>

TEMPLATE_SPECIALIZATION
CStateMonsterRestSleepAbstract::CStateMonsterRestSleep(_Object *obj) : inherited(obj)
{
}

TEMPLATE_SPECIALIZATION
CStateMonsterRestSleepAbstract::~CStateMonsterRestSleep	()
{
}

TEMPLATE_SPECIALIZATION
void CStateMonsterRestSleepAbstract::initialize()
{
	inherited::initialize	();
	inherited::object->fall_asleep		();
}

TEMPLATE_SPECIALIZATION
void CStateMonsterRestSleepAbstract::execute()
{
	inherited::object->set_action				(ACT_SLEEP);
	inherited::object->set_state_sound			(MonsterSound::eMonsterSoundIdle);	
}

TEMPLATE_SPECIALIZATION
void CStateMonsterRestSleepAbstract::finalize()
{
	inherited::finalize	();
	inherited::object->wake_up		();
}

TEMPLATE_SPECIALIZATION
void CStateMonsterRestSleepAbstract::critical_finalize()
{
	inherited::critical_finalize	();
	inherited::object->wake_up					();
}

