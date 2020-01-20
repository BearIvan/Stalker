#pragma once

#include "../../../PhysicsShell.h"
#include "../../../PHInterpolation.h"
#include "../../../PHElement.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterRestFunAbstract CStateMonsterRestFun<_Object>

#define IMPULSE_TO_CORPSE	15.f
#define MIN_DELAY			100
#define TIME_IN_STATE		8000

TEMPLATE_SPECIALIZATION
CStateMonsterRestFunAbstract::CStateMonsterRestFun(_Object *obj) : inherited(obj)
{
}

TEMPLATE_SPECIALIZATION
void CStateMonsterRestFunAbstract::initialize()
{
	inherited::initialize	();

	time_last_hit			= 0;
}


TEMPLATE_SPECIALIZATION
void CStateMonsterRestFunAbstract::execute()
{
	Fvector point;
	float	dist;
	
	Fvector dir;
	dir.sub			(inherited::object->CorpseMan.get_corpse_position(), inherited::object->Position());
	dist			= dir.magnitude();
	dir.normalize	();
	point.mad		(inherited::object->CorpseMan.get_corpse_position(), dir, 2.0f);

	inherited::object->set_action									(ACT_RUN);
	inherited::object->path().set_target_point			(point);
	inherited::object->path().set_rebuild_time			(100 + u32(50.f * dist));
	inherited::object->path().set_use_covers			(false);
	inherited::object->path().set_distance_to_end		(0.5f);
	inherited::object->anim().accel_activate					(eAT_Calm);
	inherited::object->anim().accel_set_braking					(false);
	
	inherited::object->set_state_sound								(MonsterSound::eMonsterSoundIdle);
	
	if ((dist < inherited::object->db().m_fDistToCorpse + 0.5f) && (time_last_hit + MIN_DELAY < Device.dwTimeGlobal)) {
		CEntityAlive		*corpse = const_cast<CEntityAlive *>		(inherited::object->CorpseMan.get_corpse());
		CPhysicsShellHolder	*target = smart_cast<CPhysicsShellHolder *>	(corpse);

		if  (target && target->m_pPhysicsShell) {
			Fvector			dir1;
			dir1.add			(Fvector().sub(target->Position(), inherited::object->Position()), inherited::object->Direction());
			
			float			h,p;
			dir1.getHP		(h,p);
			dir1.setHP		(h, p + 5 * XrMath::M_PI / 180);
			dir1.normalize	();
			
			// выполнить бросок
			for (u32 i=0; i<target->m_pPhysicsShell->Elements().size();i++) {
				target->m_pPhysicsShell->Elements()[i]->applyImpulse(dir1, IMPULSE_TO_CORPSE * target->m_pPhysicsShell->getMass() / target->m_pPhysicsShell->Elements().size());
			}

			time_last_hit	= Device.dwTimeGlobal;
		}
	}
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterRestFunAbstract::check_start_conditions()
{
	return ((inherited::object->CorpseMan.get_corpse() != 0) && inherited::object->Home->at_home(inherited::object->CorpseMan.get_corpse()->Position()));
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterRestFunAbstract::check_completion()
{
	if (!inherited::object->CorpseMan.get_corpse()) return true;
	if (inherited::time_state_started + TIME_IN_STATE < Device.dwTimeGlobal) return true;
	return false;
}
