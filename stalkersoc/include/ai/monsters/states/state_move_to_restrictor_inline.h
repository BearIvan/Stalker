#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterMoveToRestrictorAbstract CStateMonsterMoveToRestrictor<_Object>

TEMPLATE_SPECIALIZATION
void CStateMonsterMoveToRestrictorAbstract::initialize()
{
	inherited::initialize();
	inherited::object->path().prepare_builder();

	Fvector position;
	u32		node = inherited::object->control().path_builder().restrictions().accessible_nearest(inherited::object->Position(), position);
	inherited::object->path().set_target_point	(ai().level_graph().vertex_position(node), node);
}

TEMPLATE_SPECIALIZATION
void CStateMonsterMoveToRestrictorAbstract::execute()
{
	inherited::object->set_action					(ACT_RUN);
	
	inherited::object->anim().accel_activate		(EAccelType(eAT_Aggressive));
	inherited::object->anim().accel_set_braking	(true);
	inherited::object->set_state_sound				(MonsterSound::eMonsterSoundIdle);
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterMoveToRestrictorAbstract::check_start_conditions()
{
	return (!inherited::object->control().path_builder().accessible(inherited::object->Position()));
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterMoveToRestrictorAbstract::check_completion()
{
	return (inherited::object->control().path_builder().accessible(inherited::object->Position()));
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterMoveToRestrictorAbstract