#pragma once

#include "XrRender/skeletoncustom.h"
#include "../../../actor.h"
#include "engine/CameraBase.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateBloodsuckerVampireExecuteAbstract CStateBloodsuckerVampireExecute<_Object>

#define VAMPIRE_TIME_HOLD		4000
#define VAMPIRE_HIT_IMPULSE		400.f
#define VAMPIRE_MIN_DIST		0.5f
#define VAMPIRE_MAX_DIST		1.5f

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireExecuteAbstract::initialize()
{
	inherited::initialize					();

	inherited::object->CControlledActor::install		();

	look_head				();

	m_action				= eActionPrepare;
	time_vampire_started	= 0;

	inherited::object->stop_invisible_predator	();

	m_effector_activated			= false;
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireExecuteAbstract::execute()
{
	if (!inherited::object->CControlledActor::is_turning() && !m_effector_activated) {
		inherited::object->ActivateVampireEffector	();
		m_effector_activated			= true;
	}
	
	look_head							();

	switch (m_action) {
		case eActionPrepare:
			execute_vampire_prepare();
			m_action = eActionContinue;
			break;

		case eActionContinue:
			execute_vampire_continue();
			break;

		case eActionFire:
			execute_vampire_hit();
			m_action = eActionWaitTripleEnd;
			break;

		case eActionWaitTripleEnd:
			if (!inherited::object->com_man().ta_is_active()) {
				m_action = eActionCompleted; 
			}

		case eActionCompleted:
			break;
	}

	inherited::object->set_action			(ACT_STAND_IDLE);
	inherited::object->dir().face_target	(inherited::object->EnemyMan.get_enemy());
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireExecuteAbstract::finalize()
{
	inherited::finalize();

	inherited::object->start_invisible_predator	();

	if (inherited::object->CControlledActor::is_controlling())
		inherited::object->CControlledActor::release		();
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireExecuteAbstract::critical_finalize()
{
	inherited::critical_finalize();

	if (inherited::object->CControlledActor::is_controlling())
		inherited::object->CControlledActor::release		();
	
	inherited::object->start_invisible_predator	();
}

TEMPLATE_SPECIALIZATION
bool CStateBloodsuckerVampireExecuteAbstract::check_start_conditions()
{
	const CEntityAlive	*enemy = inherited::object->EnemyMan.get_enemy();
	
	// проверить дистанцию
	float dist		= inherited::object->MeleeChecker.distance_to_enemy	(enemy);
	if ((dist > VAMPIRE_MAX_DIST) || (dist < VAMPIRE_MIN_DIST))	return false;

	// проверить направление на врага
	if (!inherited::object->control().direction().is_face_target(enemy, XrMath::PI_DIV_6)) return false;

	return true;
}

TEMPLATE_SPECIALIZATION
bool CStateBloodsuckerVampireExecuteAbstract::check_completion()
{
	return (m_action == eActionCompleted);
}

//////////////////////////////////////////////////////////////////////////

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireExecuteAbstract::execute_vampire_prepare()
{
	inherited::object->com_man().ta_activate		(inherited::object->anim_triple_vampire);
	time_vampire_started				= Device.dwTimeGlobal;
	
	inherited::object->sound().play(CAI_Bloodsucker::eVampireGrasp);
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireExecuteAbstract::execute_vampire_continue()
{
	if (inherited::object->Position().distance_to(Actor()->Position()) > 2.f) {
		inherited::object->com_man().ta_deactivate();
		m_action = eActionCompleted;
		return;
	}
	
	inherited::object->sound().play(CAI_Bloodsucker::eVampireSucking);

	// проверить на грави удар
	if (time_vampire_started + VAMPIRE_TIME_HOLD < Device.dwTimeGlobal) {
		m_action = eActionFire;
	}
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireExecuteAbstract::execute_vampire_hit()
{
	inherited::object->com_man().ta_pointbreak				();
	inherited::object->sound().play						(CAI_Bloodsucker::eVampireHit);
	inherited::object->SatisfyVampire						();
}

//////////////////////////////////////////////////////////////////////////

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireExecuteAbstract::look_head()
{
	IKinematics *pK = smart_cast<IKinematics*>(inherited::object->Visual());
	Fmatrix bone_transform;
	bone_transform = pK->LL_GetTransform(pK->LL_BoneID("bip01_head"));	

	Fmatrix global_transform;
	global_transform.mul_43(inherited::object->XFORM(),bone_transform);

	inherited::object->CControlledActor::look_point	(global_transform.c);
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateBloodsuckerVampireExecuteAbstract

