#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>
#define CStateControlMoveOutAbstract CStateControlMoveOut<_Object>

#define MAX_STATE_TIME						10000
#define DEFAULT_LOOK_POINT_CHANGE_DELAY		2000
#define LOOK_COVER_PROBABILITY				30

TEMPLATE_SPECIALIZATION
void CStateControlMoveOutAbstract::initialize()
{
	inherited::initialize			();

	inherited::object->path().prepare_builder	();
	m_last_time_look_point_updated	= 0;

	m_state							= eMoveToNodeEnemyLastSeen;

	m_current_delay					= DEFAULT_LOOK_POINT_CHANGE_DELAY;

	// cheating here
	m_enemy_vertex					= inherited::object->EnemyMan.get_enemy()->ai_location().level_vertex_id();
}

TEMPLATE_SPECIALIZATION
void CStateControlMoveOutAbstract::execute()
{
	
	update_target_point						();

	inherited::object->path().set_target_point			(m_target_position, m_target_node);	
	inherited::object->path().set_rebuild_time			(20000);
	inherited::object->path().set_distance_to_end		(0.f);
	inherited::object->path().set_use_covers			(false);


	inherited::object->anim().m_tAction				= ACT_STEAL;
	inherited::object->anim().accel_deactivate			();
	inherited::object->anim().accel_set_braking		(false);
	inherited::object->set_state_sound					(MonsterSound::eMonsterSoundAggressive);

	// look into very open point where we go
	update_look_point						();
	inherited::object->custom_dir().head_look_point	(m_look_point);
	inherited::object->custom_anim().set_body_state	(CControllerAnimation::eTorsoSteal,CControllerAnimation::eLegsTypeStealMotion);
}

TEMPLATE_SPECIALIZATION
bool CStateControlMoveOutAbstract::check_start_conditions()
{
	if (inherited::object->EnemyMan.see_enemy_now())		return false;

	return true;
}

TEMPLATE_SPECIALIZATION
bool CStateControlMoveOutAbstract::check_completion()
{
	if (inherited::object->EnemyMan.see_enemy_now())		return true;
	if (inherited::time_state_started + 10000 < time())	return true;
	if (inherited::object->HitMemory.is_hit() && 
		inherited::object->HitMemory.get_last_hit_time() > inherited::time_state_started) return true;

	if (inherited::object->EnemyMan.get_enemy_position().distance_to(inherited::object->Position()) < 1.f) return true;

	return false;
}

TEMPLATE_SPECIALIZATION
void CStateControlMoveOutAbstract::update_target_point()
{
	if (m_state == eMoveToNodeEnemyLastSeen) {
		if (inherited::object->Position().similar(m_target_position,0.05f)) m_state = eMoveToLastEnemyLocation;
	}
	
	if (m_state == eMoveToNodeEnemyLastSeen) {
		if (inherited::object->EnemyMan.get_my_vertex_enemy_last_seen() != u32(-1))
			m_target_node		= inherited::object->EnemyMan.get_my_vertex_enemy_last_seen();
		else 
			m_target_node		= m_enemy_vertex;
		m_target_position	= ai().level_graph().vertex_position				(m_target_node);
	} else {
		m_target_node		= m_enemy_vertex;
		m_target_position	= ai().level_graph().vertex_position				(m_target_node);
	}
}


TEMPLATE_SPECIALIZATION
void CStateControlMoveOutAbstract::update_look_point()
{
	if (inherited::object->HitMemory.get_last_hit_time() > inherited::object->EnemyMan.get_enemy_time_last_seen()) {
		m_look_point.mad				(inherited::object->Position(),inherited::object->HitMemory.get_last_hit_dir(),5.f);
		m_look_point.y					+= 1.5f;
		m_last_time_look_point_updated	= time();
		return;
	} 
	
	if (m_last_time_look_point_updated + m_current_delay > time()) return;

	if ((Random.randI(100) < LOOK_COVER_PROBABILITY) && (m_last_time_look_point_updated != 0)) {
		float angle			= ai().level_graph().vertex_cover_angle(inherited::object->ai_location().level_vertex_id(),deg(10), std::greater<float>());
		m_look_point.mad	(inherited::object->Position(), Fvector().setHP(angle,0.f), 3.f);
		m_current_delay		= DEFAULT_LOOK_POINT_CHANGE_DELAY;
	} else {
		m_look_point		= inherited::object->EnemyMan.get_enemy_position();
		m_current_delay		= DEFAULT_LOOK_POINT_CHANGE_DELAY * 2;
	}
	
	m_look_point.y					+= 1.5f;
	m_last_time_look_point_updated	= time();
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateControlMoveOutAbstract
