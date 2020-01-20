#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>
#define CStateChimeraThreatenRoarAbstract CStateChimeraThreatenRoar<_Object>

TEMPLATE_SPECIALIZATION
void CStateChimeraThreatenRoarAbstract::initialize()
{
	inherited::initialize	();

}

TEMPLATE_SPECIALIZATION
void CStateChimeraThreatenRoarAbstract::execute()
{

	inherited::object->set_action				(ACT_STAND_IDLE);
	inherited::object->anim().SetSpecParams	(ASP_THREATEN);
	inherited::object->set_state_sound			(MonsterSound::eMonsterSoundThreaten);
	inherited::object->dir().face_target		(inherited::object->EnemyMan.get_enemy(), 1200);
}

#define STATE_TIME_OUT	4000

TEMPLATE_SPECIALIZATION
bool CStateChimeraThreatenRoarAbstract::check_completion()
{	
	if (inherited::time_state_started + STATE_TIME_OUT < Device.dwTimeGlobal) return true;
	return false;
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateChimeraThreatenRoarAbstract

