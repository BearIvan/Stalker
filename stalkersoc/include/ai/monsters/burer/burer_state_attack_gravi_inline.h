#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateBurerAttackGraviAbstract CStateBurerAttackGravi<_Object>

#define GOOD_DISTANCE_FOR_GRAVI 6.f

TEMPLATE_SPECIALIZATION
CStateBurerAttackGraviAbstract::CStateBurerAttackGravi(_Object *obj) : inherited(obj)
{
}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackGraviAbstract::initialize()
{
	inherited::initialize			();

	m_action						= ACTION_GRAVI_STARTED;

	time_gravi_started				= 0;

	inherited::object->set_script_capture		(false);
}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackGraviAbstract::execute()
{
	switch (m_action) {
		/************************/
		case ACTION_GRAVI_STARTED:
			/************************/

			ExecuteGraviStart();
			m_action = ACTION_GRAVI_CONTINUE;

			break;
			/************************/
		case ACTION_GRAVI_CONTINUE:
			/************************/

			ExecuteGraviContinue();

			break;

			/************************/
		case ACTION_GRAVI_FIRE:
			/************************/

			ExecuteGraviFire();
			m_action = ACTION_WAIT_TRIPLE_END;

			break;
			/***************************/
		case ACTION_WAIT_TRIPLE_END:
			/***************************/
			if (!inherited::object->com_man().ta_is_active()) {
				m_action = ACTION_COMPLETED; 
			}

			/*********************/
		case ACTION_COMPLETED:
			/*********************/

			break;
	}

	inherited::object->anim().m_tAction	= ACT_STAND_IDLE;	
	inherited::object->dir().face_target	(inherited::object->EnemyMan.get_enemy(), 500);
}
TEMPLATE_SPECIALIZATION
void CStateBurerAttackGraviAbstract::finalize()
{
	inherited::finalize();

	inherited::object->com_man().ta_pointbreak	();
	inherited::object->DeactivateShield		();
	inherited::object->set_script_capture		(true);
}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackGraviAbstract::critical_finalize()
{
	inherited::critical_finalize();
	
	inherited::object->com_man().ta_pointbreak	();
	inherited::object->DeactivateShield		();
	inherited::object->StopGraviPrepare		();
	inherited::object->set_script_capture		(false);
}

TEMPLATE_SPECIALIZATION
bool CStateBurerAttackGraviAbstract::check_start_conditions()
{
	// обработать объекты
	float dist = inherited::object->Position().distance_to(inherited::object->EnemyMan.get_enemy()->Position());
	if (dist < GOOD_DISTANCE_FOR_GRAVI) return false;
	if (!inherited::object->EnemyMan.see_enemy_now()) return false; 
	if (!inherited::object->control().direction().is_face_target(inherited::object->EnemyMan.get_enemy(), deg(45))) return false;
	if (inherited::object->com_man().ta_is_active()) return false;

	// всё ок, можно начать грави атаку
	return true;
}

TEMPLATE_SPECIALIZATION
bool CStateBurerAttackGraviAbstract::check_completion()
{
	return (m_action == ACTION_COMPLETED);
}

//////////////////////////////////////////////////////////////////////////

TEMPLATE_SPECIALIZATION
void CStateBurerAttackGraviAbstract::ExecuteGraviStart()
{
	inherited::object->com_man().ta_activate(inherited::object->anim_triple_gravi);

	time_gravi_started			= Device.dwTimeGlobal;

	inherited::object->StartGraviPrepare();
	inherited::object->ActivateShield();
}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackGraviAbstract::ExecuteGraviContinue()
{
	// проверить на грави удар

	float dist = inherited::object->Position().distance_to(inherited::object->EnemyMan.get_enemy()->Position());
	float time_to_hold = (abs(dist - GOOD_DISTANCE_FOR_GRAVI)/GOOD_DISTANCE_FOR_GRAVI);
	XrMath::clamp(time_to_hold, 0.f, 1.f);
	time_to_hold *= float(inherited::object->m_gravi_time_to_hold);

	if (time_gravi_started + u32(time_to_hold) < Device.dwTimeGlobal) {
		m_action = ACTION_GRAVI_FIRE;
	}
}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackGraviAbstract::ExecuteGraviFire()
{
	inherited::object->com_man().ta_pointbreak();

	Fvector from_pos;
	Fvector target_pos;
	from_pos	= inherited::object->Position();	from_pos.y		+= 0.5f;
	target_pos	= inherited::object->EnemyMan.get_enemy()->Position();	target_pos.y	+= 0.5f;

	inherited::object->m_gravi_object.activate(inherited::object->EnemyMan.get_enemy(), from_pos, target_pos);

	inherited::object->StopGraviPrepare	();
	inherited::object->sound().play		(CBurer::eMonsterSoundGraviAttack);
	inherited::object->DeactivateShield	();
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateBurerAttackGraviAbstract