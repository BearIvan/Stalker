#pragma once

#include "../../../level.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterLookActorAbstract CStateMonsterLookActor<_Object>

TEMPLATE_SPECIALIZATION
void CStateMonsterLookActorAbstract::execute()
{
	inherited::object->set_action			(ACT_STAND_IDLE);
	inherited::object->dir().face_target	(Level().CurrentEntity()->Position(), 1200);
}



#define CStateMonsterTurnAwayFromActorAbstract CStateMonsterTurnAwayFromActor<_Object>

TEMPLATE_SPECIALIZATION
void CStateMonsterTurnAwayFromActorAbstract::execute()
{
	Fvector point;
	Fvector dir;
	dir.sub			(inherited::object->Position(), Level().CurrentEntity()->Position());
	dir.normalize	();
	point.mad		(inherited::object->Position(), dir, 2.f);
	
	inherited::object->set_action			(ACT_STAND_IDLE);
	inherited::object->dir().face_target	(point, 1200);
}



#define CStateMonstertTestIdleAbstract CStateMonstertTestIdle<_Object>

TEMPLATE_SPECIALIZATION
void CStateMonstertTestIdleAbstract::execute()
{
	inherited::object->set_action			(ACT_STAND_IDLE);
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterLookActorAbstract
#undef CStateMonsterTurnAwayFromActorAbstract
#undef CStateMonstertTestIdleAbstract

