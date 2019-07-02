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

#ifdef LOG_ACTION
LPCSTR CObjectHandlerPlanner::action2string(const _action_id_type &id)
{
	LPSTR S = m_temp_string;
	if (action_object_id(id) != 0xffff)
		if (Level().Objects.net_Find(action_object_id(id)))
			xr_strcpy	(m_temp_string,*Level().Objects.net_Find(action_object_id(id))->cName());
		else
			xr_strcpy	(m_temp_string,"no_items");
	else
		xr_strcpy	(m_temp_string,"no_items");
	BearCore::BearString::Contact		(m_temp_string,":");
	switch (action_state_id(id)) {
		case ObjectHandlerSpace::eWorldOperatorShow			: {BearCore::BearString::Contact(m_temp_string,"Show");					break;}
		case ObjectHandlerSpace::eWorldOperatorDoShow		: {BearCore::BearString::Contact(m_temp_string,"DoShow");					break;}
		case ObjectHandlerSpace::eWorldOperatorHide			: {BearCore::BearString::Contact(m_temp_string,"Hide");					break;}
		case ObjectHandlerSpace::eWorldOperatorDrop			: {BearCore::BearString::Contact(m_temp_string,"Drop");					break;}
		case ObjectHandlerSpace::eWorldOperatorStrapping	: {BearCore::BearString::Contact(m_temp_string,"Strapping");				break;}
		case ObjectHandlerSpace::eWorldOperatorStrapping2Idle: {BearCore::BearString::Contact(m_temp_string,"Strapping to idle");		break;}
		case ObjectHandlerSpace::eWorldOperatorUnstrapping	: {BearCore::BearString::Contact(m_temp_string,"Unstrapping");				break;}
		case ObjectHandlerSpace::eWorldOperatorUnstrapping2Idle: {BearCore::BearString::Contact(m_temp_string,"Unstrapping to idle");	break;}
		case ObjectHandlerSpace::eWorldOperatorStrapped		: {BearCore::BearString::Contact(m_temp_string,"StrappedIdle");			break;}
		case ObjectHandlerSpace::eWorldOperatorIdle			: {BearCore::BearString::Contact(m_temp_string,"Idle");					break;}
		case ObjectHandlerSpace::eWorldOperatorAim1			: {BearCore::BearString::Contact(m_temp_string,"Aim1");					break;}
		case ObjectHandlerSpace::eWorldOperatorAim2			: {BearCore::BearString::Contact(m_temp_string,"Aim2");					break;}
		case ObjectHandlerSpace::eWorldOperatorReload1		: {BearCore::BearString::Contact(m_temp_string,"Reload1");					break;}
		case ObjectHandlerSpace::eWorldOperatorReload2		: {BearCore::BearString::Contact(m_temp_string,"Reload2");					break;}
		case ObjectHandlerSpace::eWorldOperatorForceReload1	: {BearCore::BearString::Contact(m_temp_string,"Force Reload1");			break;}
		case ObjectHandlerSpace::eWorldOperatorForceReload2	: {BearCore::BearString::Contact(m_temp_string,"Force Reload2");			break;}
		case ObjectHandlerSpace::eWorldOperatorFire1		: {BearCore::BearString::Contact(m_temp_string,"Fire1");					break;}
		case ObjectHandlerSpace::eWorldOperatorFire2		: {BearCore::BearString::Contact(m_temp_string,"Fire2");					break;}
		case ObjectHandlerSpace::eWorldOperatorFireNoReload	: {BearCore::BearString::Contact(m_temp_string,"Fire (no reload)");		break;}
		case ObjectHandlerSpace::eWorldOperatorAimingReady1	: {BearCore::BearString::Contact(m_temp_string,"AimingReady1");			break;}
		case ObjectHandlerSpace::eWorldOperatorAimingReady2	: {BearCore::BearString::Contact(m_temp_string,"AimingReady2");			break;}
		case ObjectHandlerSpace::eWorldOperatorSwitch1		: {BearCore::BearString::Contact(m_temp_string,"Switch1");					break;}
		case ObjectHandlerSpace::eWorldOperatorSwitch2		: {BearCore::BearString::Contact(m_temp_string,"Switch2");					break;}
		case ObjectHandlerSpace::eWorldOperatorQueueWait1	: {BearCore::BearString::Contact(m_temp_string,"QueueWait1");				break;}
		case ObjectHandlerSpace::eWorldOperatorQueueWait2	: {BearCore::BearString::Contact(m_temp_string,"QueueWait1");				break;}
		case ObjectHandlerSpace::eWorldOperatorThrowStart	: {BearCore::BearString::Contact(m_temp_string,"ThrowStart");				break;}
		case ObjectHandlerSpace::eWorldOperatorThrowIdle	: {BearCore::BearString::Contact(m_temp_string,"ThrowIdle");				break;}
		case ObjectHandlerSpace::eWorldOperatorThrow		: {BearCore::BearString::Contact(m_temp_string,"Throwing");				break;}
		case ObjectHandlerSpace::eWorldOperatorThreaten		: {BearCore::BearString::Contact(m_temp_string,"Threaten");				break;}
		case ObjectHandlerSpace::eWorldOperatorPrepare		: {BearCore::BearString::Contact(m_temp_string,"Preparing");				break;}
		case ObjectHandlerSpace::eWorldOperatorUse			: {BearCore::BearString::Contact(m_temp_string,"Using");					break;}
		case ObjectHandlerSpace::eWorldOperatorGetAmmo1		: {BearCore::BearString::Contact(m_temp_string,"GetAmmo1");				break;}
		case ObjectHandlerSpace::eWorldOperatorGetAmmo2		: {BearCore::BearString::Contact(m_temp_string,"GetAmmo2");				break;}
		case ObjectHandlerSpace::eWorldOperatorAimForceFull1: {BearCore::BearString::Contact(m_temp_string,"AimForceFull1");			break;}
		case ObjectHandlerSpace::eWorldOperatorAimForceFull2: {BearCore::BearString::Contact(m_temp_string,"AimForceFull2");			break;}
		default												: NODEFAULT;
	}
	return		(S);
}

LPCSTR CObjectHandlerPlanner::property2string(const _condition_type &id)
{
	if (action_object_id(id) != 0xffff)
		if (Level().Objects.net_Find(action_object_id(id)))
			xr_strcpy	(m_temp_string,*Level().Objects.net_Find(action_object_id(id))->cName());
		else
			xr_strcpy	(m_temp_string,"no_items");
	else
		xr_strcpy	(m_temp_string,"no_items");
	BearCore::BearString::Contact		(m_temp_string,":");
	switch (action_state_id(id)) {
		case ObjectHandlerSpace::eWorldPropertyHidden			: {BearCore::BearString::Contact(m_temp_string,"Hidden");				break;}
		case ObjectHandlerSpace::eWorldPropertyShown			: {BearCore::BearString::Contact(m_temp_string,"Shown");				break;}
		case ObjectHandlerSpace::eWorldPropertyStrapped			: {BearCore::BearString::Contact(m_temp_string,"Strapped");			break;}
		case ObjectHandlerSpace::eWorldPropertyStrapped2Idle	: {BearCore::BearString::Contact(m_temp_string,"Strapped to idle");	break;}
		case ObjectHandlerSpace::eWorldPropertySwitch1			: {BearCore::BearString::Contact(m_temp_string,"Switch1");				break;}
		case ObjectHandlerSpace::eWorldPropertySwitch2			: {BearCore::BearString::Contact(m_temp_string,"Switch2");				break;}
		case ObjectHandlerSpace::eWorldPropertyAimed1			: {BearCore::BearString::Contact(m_temp_string,"Aimed1");				break;}
		case ObjectHandlerSpace::eWorldPropertyAimed2			: {BearCore::BearString::Contact(m_temp_string,"Aimed2");				break;}
		case ObjectHandlerSpace::eWorldPropertyAimForceFull1	: {BearCore::BearString::Contact(m_temp_string,"AimedForceFull1");		break;}
		case ObjectHandlerSpace::eWorldPropertyAimForceFull2	: {BearCore::BearString::Contact(m_temp_string,"AimedForceFull2");		break;}
		case ObjectHandlerSpace::eWorldPropertyAiming1			: {BearCore::BearString::Contact(m_temp_string,"Aiming1");				break;}
		case ObjectHandlerSpace::eWorldPropertyAiming2			: {BearCore::BearString::Contact(m_temp_string,"Aiming2");				break;}
		case ObjectHandlerSpace::eWorldPropertyEmpty1			: {BearCore::BearString::Contact(m_temp_string,"Empty1");				break;}
		case ObjectHandlerSpace::eWorldPropertyEmpty2			: {BearCore::BearString::Contact(m_temp_string,"Empty2");				break;}
		case ObjectHandlerSpace::eWorldPropertyFull1			: {BearCore::BearString::Contact(m_temp_string,"Full1");				break;}
		case ObjectHandlerSpace::eWorldPropertyFull2			: {BearCore::BearString::Contact(m_temp_string,"Full2");				break;}
		case ObjectHandlerSpace::eWorldPropertyReady1			: {BearCore::BearString::Contact(m_temp_string,"Ready1");				break;}
		case ObjectHandlerSpace::eWorldPropertyReady2			: {BearCore::BearString::Contact(m_temp_string,"Ready2");				break;}
		case ObjectHandlerSpace::eWorldPropertyFiring1			: {BearCore::BearString::Contact(m_temp_string,"Firing1");				break;}
		case ObjectHandlerSpace::eWorldPropertyFiringNoReload1	: {BearCore::BearString::Contact(m_temp_string,"FiringNoReload1");		break;}
		case ObjectHandlerSpace::eWorldPropertyFiring2			: {BearCore::BearString::Contact(m_temp_string,"Firing2");				break;}
		case ObjectHandlerSpace::eWorldPropertyAimingReady1		: {BearCore::BearString::Contact(m_temp_string,"AimingReady1");		break;}
		case ObjectHandlerSpace::eWorldPropertyAimingReady2		: {BearCore::BearString::Contact(m_temp_string,"AimingReady2");		break;}
		case ObjectHandlerSpace::eWorldPropertyAmmo1			: {BearCore::BearString::Contact(m_temp_string,"Ammo1");				break;}
		case ObjectHandlerSpace::eWorldPropertyAmmo2			: {BearCore::BearString::Contact(m_temp_string,"Ammo2");				break;}
		case ObjectHandlerSpace::eWorldPropertyIdle				: {BearCore::BearString::Contact(m_temp_string,"Idle");				break;}
		case ObjectHandlerSpace::eWorldPropertyIdleStrap		: {BearCore::BearString::Contact(m_temp_string,"IdleStrap");			break;}
		case ObjectHandlerSpace::eWorldPropertyDropped			: {BearCore::BearString::Contact(m_temp_string,"Dropped");				break;}
		case ObjectHandlerSpace::eWorldPropertyQueueWait1		: {BearCore::BearString::Contact(m_temp_string,"QueueWait1");			break;}
		case ObjectHandlerSpace::eWorldPropertyQueueWait2		: {BearCore::BearString::Contact(m_temp_string,"QueueWait2");			break;}
		case ObjectHandlerSpace::eWorldPropertyThrowStarted		: {BearCore::BearString::Contact(m_temp_string,"ThrowStarted");		break;}
		case ObjectHandlerSpace::eWorldPropertyThrowIdle		: {BearCore::BearString::Contact(m_temp_string,"ThrowIdle");			break;}
		case ObjectHandlerSpace::eWorldPropertyThrow			: {BearCore::BearString::Contact(m_temp_string,"Throwing");			break;}
		case ObjectHandlerSpace::eWorldPropertyThreaten			: {BearCore::BearString::Contact(m_temp_string,"Threaten");			break;}
		case ObjectHandlerSpace::eWorldPropertyPrepared			: {BearCore::BearString::Contact(m_temp_string,"Prepared");			break;}
		case ObjectHandlerSpace::eWorldPropertyUsed				: {BearCore::BearString::Contact(m_temp_string,"Used");				break;}
		case ObjectHandlerSpace::eWorldPropertyUseEnough		: {BearCore::BearString::Contact(m_temp_string,"UseEnough");			break;}
		case ObjectHandlerSpace::eWorldPropertyItemID			: {m_temp_string[xr_strlen(m_temp_string) - 1] = 0;			break;}
		default													: NODEFAULT;
	}
	return		(m_temp_string);
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
