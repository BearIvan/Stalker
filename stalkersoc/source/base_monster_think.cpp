#include "StdAfx.h"
#include "ai/monsters/basemonster/base_monster.h"
#include "ai/monsters/ai_monster_squad.h"
#include "ai/monsters/ai_monster_squad_manager.h"
#include "profiler.h"
#include "ai/monsters/state_manager.h"
#include "PhysicsShell.h"
#include "detail_path_manager.h"
#include "ai/monsters/monster_velocity_space.h"
#include "level.h"
#include "ai/monsters/control_animation_base.h"

void CBaseMonster::Think()
{
	START_PROFILE("Base Monster/Think");

	if (!g_Alive() || getDestroy())			return;

	// ����������������
	InitThink								();
	anim().ScheduledInit					();

	// �������� ������
	START_PROFILE("Base Monster/Think/Update Memory");
	UpdateMemory							();
	STOP_PROFILE;

	// �������� �����
	START_PROFILE("Base Monster/Think/Update Squad");
	monster_squad().update					(this);
	STOP_PROFILE;

	// ��������� FSM
	START_PROFILE("Base Monster/Think/FSM");
	update_fsm								();
	STOP_PROFILE;	
	
	STOP_PROFILE;
}

void CBaseMonster::update_fsm()
{
	StateMan->update				();
	
	// ��������� ��������� ������������� � FSM ����������
	post_fsm_update					();
	
	TranslateActionToPathParams		();

	// ������������� squad � ����� �����
	squad_notify					();

#ifdef DEBUG
	debug_fsm						();
#endif
}

void CBaseMonster::post_fsm_update()
{
	if (!EnemyMan.get_enemy()) return;
	
	EMonsterState state = StateMan->get_state_type();


	// Look at enemy while running
	m_bRunTurnLeft = m_bRunTurnRight = false;
	

	if (is_state(state, eStateAttack) && control().path_builder().is_moving_on_path()) {

		float	dir_yaw = control().path_builder().detail().direction().getH();
		float	yaw_target = Fvector().sub(EnemyMan.get_enemy()->Position(), Position()).getH();

		float angle_diff	= angle_difference(yaw_target, dir_yaw);

		if ((angle_diff > PI_DIV_3) && (angle_diff < 5 * PI_DIV_6)) {
			if (from_right(dir_yaw, yaw_target))	m_bRunTurnRight = true;
			else									m_bRunTurnLeft	= true;
		}
	}
}

void CBaseMonster::squad_notify()
{
	CMonsterSquad	*squad = monster_squad().get_squad(this);
	SMemberGoal		goal;

	EMonsterState state = StateMan->get_state_type();

	if (is_state(state, eStateAttack)) {
		
		goal.type	= MG_AttackEnemy;
		goal.entity	= const_cast<CEntityAlive*>(EnemyMan.get_enemy());

	} else if (is_state(state, eStateRest)) {
		goal.entity	= squad->GetLeader();

		if (state == eStateRest_Idle)							goal.type	= MG_Rest;
		else if (state == eStateRest_WalkGraphPoint) 			goal.type	= MG_WalkGraph;
		else if (state == eStateRest_MoveToHomePoint) 			goal.type	= MG_WalkGraph;
		else if (state == eStateCustomMoveToRestrictor)			goal.type	= MG_WalkGraph;
		else if (state == eStateRest_WalkToCover)				goal.type	= MG_WalkGraph;
		else if (state == eStateRest_LookOpenPlace)				goal.type	= MG_Rest;
		else													goal.entity	= 0;

	} else if (is_state(state, eStateSquad)) {
		goal.type	= MG_Rest;
		goal.entity	= squad->GetLeader();
	}
	
	squad->UpdateGoal(this, goal);
}
