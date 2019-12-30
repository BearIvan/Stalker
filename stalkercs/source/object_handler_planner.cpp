////////////////////////////////////////////////////////////////////////////
//	Module 		: object_handler_planner.cpp
//	Created 	: 11.03.2004
//  Modified 	: 01.12.2004
//	Author		: Dmitriy Iassenev
//	Description : Object handler action planner
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "object_handler_planner.h"
#include "object_property_evaluators.h"
#include "object_actions.h"
#include "ai_monster_space.h"
#include "object_handler_space.h"
#include "ai/stalker/ai_stalker.h"
#include "inventory.h"
#include "object_handler_planner_impl.h"
#include "weaponmagazined.h"
#include "missile.h"
#include "ai_monster_space.h"

using namespace ObjectHandlerSpace;

IC	ObjectHandlerSpace::EWorldProperties CObjectHandlerPlanner::object_property(MonsterSpace::EObjectAction object_action) const
{
	switch (object_action) {
		case MonsterSpace::eObjectActionSwitch1			: return(ObjectHandlerSpace::eWorldPropertySwitch1);
		case MonsterSpace::eObjectActionSwitch2			: return(ObjectHandlerSpace::eWorldPropertySwitch2);
		case MonsterSpace::eObjectActionAim1			: return(ObjectHandlerSpace::eWorldPropertyAimingReady1);
		case MonsterSpace::eObjectActionAim2			: return(ObjectHandlerSpace::eWorldPropertyAiming2);
		case MonsterSpace::eObjectActionFire1			: return(ObjectHandlerSpace::eWorldPropertyFiring1);
		case MonsterSpace::eObjectActionFireNoReload	: return(ObjectHandlerSpace::eWorldPropertyFiringNoReload1);
		case MonsterSpace::eObjectActionFire2			: return(ObjectHandlerSpace::eWorldPropertyFiring2);
		case MonsterSpace::eObjectActionIdle			: return(ObjectHandlerSpace::eWorldPropertyIdle);
		case MonsterSpace::eObjectActionStrapped		: return(ObjectHandlerSpace::eWorldPropertyIdleStrap);
		case MonsterSpace::eObjectActionDrop			: return(ObjectHandlerSpace::eWorldPropertyDropped);
		case MonsterSpace::eObjectActionActivate		: return(ObjectHandlerSpace::eWorldPropertyIdle);
		case MonsterSpace::eObjectActionDeactivate		: return(ObjectHandlerSpace::eWorldPropertyNoItemsIdle);
		case MonsterSpace::eObjectActionAimReady1		: return(ObjectHandlerSpace::eWorldPropertyAimingReady1);
		case MonsterSpace::eObjectActionAimReady2		: return(ObjectHandlerSpace::eWorldPropertyAimingReady2);
		case MonsterSpace::eObjectActionAimForceFull1	: return(ObjectHandlerSpace::eWorldPropertyAimForceFull1);
		case MonsterSpace::eObjectActionAimForceFull2	: return(ObjectHandlerSpace::eWorldPropertyAimForceFull2);
		case MonsterSpace::eObjectActionUse				: return(ObjectHandlerSpace::eWorldPropertyUsed);
		default							: NODEFAULT;
	}
#ifdef DEBUG
	return	(ObjectHandlerSpace::eWorldPropertyDummy);
#endif
}

void CObjectHandlerPlanner::set_goal	(MonsterSpace::EObjectAction object_action, CGameObject *game_object, u32 min_queue_size, u32 max_queue_size, u32 min_queue_interval, u32 max_queue_interval)
{
	EWorldProperties		goal = object_property(object_action);
	u32						condition_id = goal;

	if (game_object && (eWorldPropertyNoItemsIdle != goal)) {
		CWeapon				*weapon = smart_cast<CWeapon*>(game_object);
		if (weapon && (goal == eWorldPropertyIdleStrap) && !weapon->can_be_strapped())
			goal			= eWorldPropertyIdle;
		condition_id		= uid(game_object->ID(), goal);
	}
	else
		condition_id		= u32(eWorldPropertyNoItemsIdle);

#ifdef DEBUG
	if (m_use_log) {
		Msg					("%6d : Active item %s",Device.dwTimeGlobal,object().inventory().ActiveItem() ? *object().inventory().ActiveItem()->object().cName() : "no active items");
		Msg					("%6d : Goal %s",Device.dwTimeGlobal,property2string(condition_id));
	}
#endif
	CState					condition;
	condition.add_condition	(CWorldProperty(condition_id,true));
	set_target_state		(condition);

	if (!game_object || (min_queue_size < 0))
		return;

	CWeaponMagazined		*weapon = smart_cast<CWeaponMagazined*>(game_object);
	if (!weapon)
		return;

	if	(
			(m_min_queue_size != min_queue_size) ||
			(m_max_queue_size != max_queue_size) ||
			(m_min_queue_interval != min_queue_interval) ||
			(m_max_queue_interval != max_queue_interval) ||
			(m_next_time_change <= Device.dwTimeGlobal)
		)
	{
		m_min_queue_size		= min_queue_size;
		m_max_queue_size		= max_queue_size;
		m_min_queue_interval	= min_queue_interval;
		m_max_queue_interval	= max_queue_interval;

		if (m_max_queue_size == m_min_queue_size)
			m_queue_size		= XrMath::max(u32(1),m_min_queue_size);
		else
			m_queue_size		= XrMath::max(1,::Random.randI(m_min_queue_size,m_max_queue_size));

		if (m_max_queue_interval == m_min_queue_interval)
			m_queue_interval	= m_min_queue_interval;
		else
			m_queue_interval	= ::Random.randI(m_min_queue_interval,m_max_queue_interval);

		m_next_time_change		= Device.dwTimeGlobal + m_queue_interval;

		weapon->SetQueueSize	(m_queue_size);
		this->action(uid(weapon->ID(),eWorldOperatorQueueWait1)).set_inertia_time(m_queue_interval ? m_queue_interval : 300);
		this->action(uid(weapon->ID(),eWorldOperatorQueueWait2)).set_inertia_time(m_queue_interval ? m_queue_interval : 300);
	}
}
IN const bchar*xr_strcnt(bchar*a, const bchar*b)
{
	BearString::Contact(a, 64, b);
	return a;
}
#ifdef LOG_ACTION
LPCSTR CObjectHandlerPlanner::action2string(const _action_id_type &id)
{
	LPSTR S = m_temp_string;
	if (action_object_id(id) != 0xffff)
		if (Level().Objects.net_Find(action_object_id(id)))
			BearString::Copy	(S,sizeof(m_temp_string),*Level().Objects.net_Find(action_object_id(id))->cName());
		else
			BearString::Copy	(S,sizeof(m_temp_string),"no_items");
	else
		BearString::Copy	(S,sizeof(m_temp_string),"no_items");
	xr_strcnt		(S,":");
	switch (action_state_id(id)) {
		case ObjectHandlerSpace::eWorldOperatorShow			: {xr_strcnt(S,"Show");					break;}
		case ObjectHandlerSpace::eWorldOperatorDoShow		: {xr_strcnt(S,"DoShow");					break;}
		case ObjectHandlerSpace::eWorldOperatorHide			: {xr_strcnt(S,"Hide");					break;}
		case ObjectHandlerSpace::eWorldOperatorDrop			: {xr_strcnt(S,"Drop");					break;}
		case ObjectHandlerSpace::eWorldOperatorStrapping	: {xr_strcnt(S,"Strapping");				break;}
		case ObjectHandlerSpace::eWorldOperatorStrapping2Idle: {xr_strcnt(S,"Strapping to idle");		break;}
		case ObjectHandlerSpace::eWorldOperatorUnstrapping	: {xr_strcnt(S,"Unstrapping");				break;}
		case ObjectHandlerSpace::eWorldOperatorUnstrapping2Idle: {xr_strcnt(S,"Unstrapping to idle");	break;}
		case ObjectHandlerSpace::eWorldOperatorStrapped		: {xr_strcnt(S,"StrappedIdle");			break;}
		case ObjectHandlerSpace::eWorldOperatorIdle			: {xr_strcnt(S,"Idle");					break;}
		case ObjectHandlerSpace::eWorldOperatorAim1			: {xr_strcnt(S,"Aim1");					break;}
		case ObjectHandlerSpace::eWorldOperatorAim2			: {xr_strcnt(S,"Aim2");					break;}
		case ObjectHandlerSpace::eWorldOperatorReload1		: {xr_strcnt(S,"Reload1");					break;}
		case ObjectHandlerSpace::eWorldOperatorReload2		: {xr_strcnt(S,"Reload2");					break;}
		case ObjectHandlerSpace::eWorldOperatorForceReload1	: {xr_strcnt(S,"Force Reload1");			break;}
		case ObjectHandlerSpace::eWorldOperatorForceReload2	: {xr_strcnt(S,"Force Reload2");			break;}
		case ObjectHandlerSpace::eWorldOperatorFire1		: {xr_strcnt(S,"Fire1");					break;}
		case ObjectHandlerSpace::eWorldOperatorFire2		: {xr_strcnt(S,"Fire2");					break;}
		case ObjectHandlerSpace::eWorldOperatorFireNoReload	: {xr_strcnt(S,"Fire (no reload)");		break;}
		case ObjectHandlerSpace::eWorldOperatorAimingReady1	: {xr_strcnt(S,"AimingReady1");			break;}
		case ObjectHandlerSpace::eWorldOperatorAimingReady2	: {xr_strcnt(S,"AimingReady2");			break;}
		case ObjectHandlerSpace::eWorldOperatorSwitch1		: {xr_strcnt(S,"Switch1");					break;}
		case ObjectHandlerSpace::eWorldOperatorSwitch2		: {xr_strcnt(S,"Switch2");					break;}
		case ObjectHandlerSpace::eWorldOperatorQueueWait1	: {xr_strcnt(S,"QueueWait1");				break;}
		case ObjectHandlerSpace::eWorldOperatorQueueWait2	: {xr_strcnt(S,"QueueWait1");				break;}
		case ObjectHandlerSpace::eWorldOperatorThrowStart	: {xr_strcnt(S,"ThrowStart");				break;}
		case ObjectHandlerSpace::eWorldOperatorThrowIdle	: {xr_strcnt(S,"ThrowIdle");				break;}
		case ObjectHandlerSpace::eWorldOperatorThrow		: {xr_strcnt(S,"Throwing");				break;}
		case ObjectHandlerSpace::eWorldOperatorThreaten		: {xr_strcnt(S,"Threaten");				break;}
		case ObjectHandlerSpace::eWorldOperatorPrepare		: {xr_strcnt(S,"Preparing");				break;}
		case ObjectHandlerSpace::eWorldOperatorUse			: {xr_strcnt(S,"Using");					break;}
		case ObjectHandlerSpace::eWorldOperatorGetAmmo1		: {xr_strcnt(S,"GetAmmo1");				break;}
		case ObjectHandlerSpace::eWorldOperatorGetAmmo2		: {xr_strcnt(S,"GetAmmo2");				break;}
		case ObjectHandlerSpace::eWorldOperatorAimForceFull1: {xr_strcnt(S,"AimForceFull1");			break;}
		case ObjectHandlerSpace::eWorldOperatorAimForceFull2: {xr_strcnt(S,"AimForceFull2");			break;}
		default												: NODEFAULT;
	}
	return		(S);
}

LPCSTR CObjectHandlerPlanner::property2string(const _condition_type &id)
{
	LPSTR S = m_temp_string;
	if (action_object_id(id) != 0xffff)
		if (Level().Objects.net_Find(action_object_id(id)))
			BearString::Copy	(S,sizeof(m_temp_string),*Level().Objects.net_Find(action_object_id(id))->cName());
		else
			BearString::Copy	(S,sizeof(m_temp_string),"no_items");
	else
		BearString::Copy	(S,sizeof(m_temp_string),"no_items");
	xr_strcnt		(S,":");
	switch (action_state_id(id)) {
		case ObjectHandlerSpace::eWorldPropertyHidden			: {xr_strcnt(S,"Hidden");				break;}
		case ObjectHandlerSpace::eWorldPropertyShown			: {xr_strcnt(S,"Shown");				break;}
		case ObjectHandlerSpace::eWorldPropertyStrapped			: {xr_strcnt(S,"Strapped");			break;}
		case ObjectHandlerSpace::eWorldPropertyStrapped2Idle	: {xr_strcnt(S,"Strapped to idle");	break;}
		case ObjectHandlerSpace::eWorldPropertySwitch1			: {xr_strcnt(S,"Switch1");				break;}
		case ObjectHandlerSpace::eWorldPropertySwitch2			: {xr_strcnt(S,"Switch2");				break;}
		case ObjectHandlerSpace::eWorldPropertyAimed1			: {xr_strcnt(S,"Aimed1");				break;}
		case ObjectHandlerSpace::eWorldPropertyAimed2			: {xr_strcnt(S,"Aimed2");				break;}
		case ObjectHandlerSpace::eWorldPropertyAimForceFull1	: {xr_strcnt(S,"AimedForceFull1");		break;}
		case ObjectHandlerSpace::eWorldPropertyAimForceFull2	: {xr_strcnt(S,"AimedForceFull2");		break;}
		case ObjectHandlerSpace::eWorldPropertyAiming1			: {xr_strcnt(S,"Aiming1");				break;}
		case ObjectHandlerSpace::eWorldPropertyAiming2			: {xr_strcnt(S,"Aiming2");				break;}
		case ObjectHandlerSpace::eWorldPropertyEmpty1			: {xr_strcnt(S,"Empty1");				break;}
		case ObjectHandlerSpace::eWorldPropertyEmpty2			: {xr_strcnt(S,"Empty2");				break;}
		case ObjectHandlerSpace::eWorldPropertyFull1			: {xr_strcnt(S,"Full1");				break;}
		case ObjectHandlerSpace::eWorldPropertyFull2			: {xr_strcnt(S,"Full2");				break;}
		case ObjectHandlerSpace::eWorldPropertyReady1			: {xr_strcnt(S,"Ready1");				break;}
		case ObjectHandlerSpace::eWorldPropertyReady2			: {xr_strcnt(S,"Ready2");				break;}
		case ObjectHandlerSpace::eWorldPropertyFiring1			: {xr_strcnt(S,"Firing1");				break;}
		case ObjectHandlerSpace::eWorldPropertyFiringNoReload1	: {xr_strcnt(S,"FiringNoReload1");		break;}
		case ObjectHandlerSpace::eWorldPropertyFiring2			: {xr_strcnt(S,"Firing2");				break;}
		case ObjectHandlerSpace::eWorldPropertyAimingReady1		: {xr_strcnt(S,"AimingReady1");		break;}
		case ObjectHandlerSpace::eWorldPropertyAimingReady2		: {xr_strcnt(S,"AimingReady2");		break;}
		case ObjectHandlerSpace::eWorldPropertyAmmo1			: {xr_strcnt(S,"Ammo1");				break;}
		case ObjectHandlerSpace::eWorldPropertyAmmo2			: {xr_strcnt(S,"Ammo2");				break;}
		case ObjectHandlerSpace::eWorldPropertyIdle				: {xr_strcnt(S,"Idle");				break;}
		case ObjectHandlerSpace::eWorldPropertyIdleStrap		: {xr_strcnt(S,"IdleStrap");			break;}
		case ObjectHandlerSpace::eWorldPropertyDropped			: {xr_strcnt(S,"Dropped");				break;}
		case ObjectHandlerSpace::eWorldPropertyQueueWait1		: {xr_strcnt(S,"QueueWait1");			break;}
		case ObjectHandlerSpace::eWorldPropertyQueueWait2		: {xr_strcnt(S,"QueueWait2");			break;}
		case ObjectHandlerSpace::eWorldPropertyThrowStarted		: {xr_strcnt(S,"ThrowStarted");		break;}
		case ObjectHandlerSpace::eWorldPropertyThrowIdle		: {xr_strcnt(S,"ThrowIdle");			break;}
		case ObjectHandlerSpace::eWorldPropertyThrow			: {xr_strcnt(S,"Throwing");			break;}
		case ObjectHandlerSpace::eWorldPropertyThreaten			: {xr_strcnt(S,"Threaten");			break;}
		case ObjectHandlerSpace::eWorldPropertyPrepared			: {xr_strcnt(S,"Prepared");			break;}
		case ObjectHandlerSpace::eWorldPropertyUsed				: {xr_strcnt(S,"Used");				break;}
		case ObjectHandlerSpace::eWorldPropertyUseEnough		: {xr_strcnt(S,"UseEnough");			break;}
		case ObjectHandlerSpace::eWorldPropertyItemID			: {S[xr_strlen(S) - 1] = 0;			break;}
		default													: NODEFAULT;
	}
	return		(S);
}
#endif

void CObjectHandlerPlanner::remove_evaluators	(CObject *object)
{
#pragma todo("Dima to Dima : safe, but not optimal!")
	for (;;) {
		EVALUATORS::iterator	I = m_evaluators.lower_bound(uid(object->ID(),0));
		if (!object_action((*I).first,object))
			break;
		remove_evaluator		((*I).first);
	}
}

void CObjectHandlerPlanner::remove_operators	(CObject *object)
{
#pragma todo("Dima to Dima : safe, but not optimal!")
	for (;;) {
		OPERATOR_VECTOR::iterator	I = std::lower_bound(m_operators.begin(),m_operators.end(),uid(object->ID(),0));
		if (!object_action((*I).m_operator_id,object))
			break;
		remove_operator	((*I).m_operator_id);
	}
}

void CObjectHandlerPlanner::init_storage	()
{
	m_storage.set_property		(eWorldPropertyAimed1,false);
	m_storage.set_property		(eWorldPropertyAimed2,false);
	m_storage.set_property		(eWorldPropertyUseEnough,false);
	m_storage.set_property		(eWorldPropertyStrapped,false);
	m_storage.set_property		(eWorldPropertyStrapped2Idle,false);
}

void CObjectHandlerPlanner::setup	(CAI_Stalker *object)
{
	inherited::setup			(object);
	CActionBase<CAI_Stalker>	*action;

	m_min_queue_size			= 0;
	m_max_queue_size			= 0;
	m_min_queue_interval		= 0;
	m_max_queue_interval		= 0;
	m_next_time_change			= 0;

	clear						();

	init_storage				();

	add_evaluator				(u32(eWorldPropertyNoItems),			xr_new<CObjectPropertyEvaluatorNoItems>(m_object));
	add_evaluator				(u32(eWorldPropertyNoItemsIdle),		xr_new<CObjectPropertyEvaluatorConst>(false));
	action						= xr_new<CSObjectActionBase>(m_object,m_object,&m_storage,"no items idle");
	add_condition				(action,0xffff,eWorldPropertyItemID,true);
	add_effect					(action,0xffff,eWorldPropertyIdle,	true);
	add_operator				(u32(eWorldOperatorNoItemsIdle),action);

	set_goal					(MonsterSpace::eObjectActionIdle,0,0,0,0,0);

#ifdef LOG_ACTION
	set_use_log					(!!psAI_Flags.test(aiGOAPObject));
#endif
}

void CObjectHandlerPlanner::add_item			(CInventoryItem *inventory_item)
{
	CWeapon						*weapon		= smart_cast<CWeapon*>		(inventory_item);
	if (weapon) {
		add_evaluators			(weapon);
		add_operators			(weapon);
		return;
	}

	CMissile					*missile	= smart_cast<CMissile*>		(inventory_item);
	if (missile) {
		add_evaluators			(missile);
		add_operators			(missile);
		return;
	}
}

void CObjectHandlerPlanner::remove_item		(CInventoryItem *inventory_item)
{
	VERIFY					(target_state().conditions().size() == 1);
	if (action_object_id(target_state().conditions().back().condition()) == inventory_item->object().ID()) {
		init_storage		();
		set_goal			(MonsterSpace::eObjectActionIdle,0,0,0,0,0);
	}

	remove_evaluators		(&inventory_item->object());
	remove_operators		(&inventory_item->object());
}

void CObjectHandlerPlanner::update			()
{
#ifdef LOG_ACTION
	if ((psAI_Flags.test(aiGOAPObject) && !m_use_log) || (!psAI_Flags.test(aiGOAPObject) && m_use_log))
		set_use_log			(!!psAI_Flags.test(aiGOAPObject));
#endif
	inherited::update		();
}
