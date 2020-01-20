#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterAttackCampStealOutAbstract CStateMonsterAttackCampStealOut<_Object>

TEMPLATE_SPECIALIZATION
CStateMonsterAttackCampStealOutAbstract::CStateMonsterAttackCampStealOut(_Object *obj) : inherited(obj) 
{
}

TEMPLATE_SPECIALIZATION
void CStateMonsterAttackCampStealOutAbstract::execute()
{
	if (inherited::object->EnemyMan.get_my_vertex_enemy_last_seen() == u32(-1)) return;
	
	inherited::object->path().set_target_point			(inherited::object->EnemyMan.get_my_vertex_enemy_last_seen());	
	inherited::object->path().set_rebuild_time			(0);
	inherited::object->path().set_distance_to_end		(0.f);
	inherited::object->path().set_use_covers			(false);
	
	inherited::object->set_action						(ACT_STEAL);
	inherited::object->anim().accel_deactivate			();
	inherited::object->anim().accel_set_braking		(false);
	inherited::object->set_state_sound					(MonsterSound::eMonsterSoundSteal);
}

#define STATE_EXECUTE_TIME 8000


TEMPLATE_SPECIALIZATION
bool CStateMonsterAttackCampStealOutAbstract::check_completion()
{
	if (inherited::object->EnemyMan.get_my_vertex_enemy_last_seen() == u32(-1)) return true;
	if (inherited::object->EnemyMan.see_enemy_now()) return true;
	if (inherited::object->HitMemory.get_last_hit_time() > inherited::time_state_started) return true;
	if (inherited::time_state_started + STATE_EXECUTE_TIME < time()) return true;
	
	Fvector pos = ai().level_graph().vertex_position(inherited::object->EnemyMan.get_my_vertex_enemy_last_seen());
	if ((inherited::object->Position().distance_to(pos) < 2.f) && inherited::object->control().path_builder().is_path_end(0.f)) return true;

	return false;
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterAttackCampStealOutAbstract::check_start_conditions()
{
	if (inherited::object->EnemyMan.get_my_vertex_enemy_last_seen() == u32(-1)) return false;
	if (inherited::object->EnemyMan.see_enemy_now()) return false;
	return true;	
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterAttackCampStealOutAbstract

