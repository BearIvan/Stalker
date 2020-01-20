#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>
#define CStateMonsterHittedMoveOutAbstract CStateMonsterHittedMoveOut<_Object>

#define DIST_TO_PATH_END	1.5f
#define DIST_TO_HIT_POINT	3.f

TEMPLATE_SPECIALIZATION
void CStateMonsterHittedMoveOutAbstract::initialize()
{
	inherited::initialize();
	select_target();
	inherited::object->path().prepare_builder	();	
}

TEMPLATE_SPECIALIZATION
void CStateMonsterHittedMoveOutAbstract::execute()
{
	// проверить на завершение пути
	if (inherited::object->control().path_builder().detail().time_path_built() > inherited::time_state_started) {
		if (inherited::object->control().path_builder().is_path_end(DIST_TO_PATH_END)) 
			select_target		();
	}
	
	if (target.node != u32(-1))
		inherited::object->path().set_target_point	(target.position, target.node);
	else
		inherited::object->path().set_target_point	(inherited::object->HitMemory.get_last_hit_position());

	float dist = inherited::object->HitMemory.get_last_hit_position().distance_to(inherited::object->Position());

	if (dist > 10.f) inherited::object->set_action	(ACT_WALK_FWD);
	else inherited::object->set_action				(ACT_STEAL);
	
	inherited::object->anim().accel_deactivate	();
	inherited::object->anim().accel_set_braking (false);
	inherited::object->set_state_sound				(MonsterSound::eMonsterSoundIdle);
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterHittedMoveOutAbstract::check_completion()
{
	if (inherited::object->HitMemory.get_last_hit_time() > inherited::time_state_started) return true;

	float dist = inherited::object->HitMemory.get_last_hit_position().distance_to(inherited::object->Position());
	if (dist < DIST_TO_HIT_POINT) return true;

	return false;
}

TEMPLATE_SPECIALIZATION
void CStateMonsterHittedMoveOutAbstract::select_target()
{
	if (!inherited::object->GetCoverCloseToPoint(inherited::object->HitMemory.get_last_hit_position(), 10.f, 20.f, 0.f, 15.f, target.position, target.node)){
		target.node = u32(-1);
	}
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterHittedMoveOutAbstract
