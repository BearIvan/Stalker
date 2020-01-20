#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateBloodsuckerVampireApproachAbstract CStateBloodsuckerVampireApproach<_Object>

TEMPLATE_SPECIALIZATION
CStateBloodsuckerVampireApproachAbstract::CStateBloodsuckerVampireApproach(_Object *obj) : inherited(obj)
{
}

TEMPLATE_SPECIALIZATION
CStateBloodsuckerVampireApproachAbstract::~CStateBloodsuckerVampireApproach()
{
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireApproachAbstract::initialize()
{
	inherited::initialize();
	inherited::object->path().prepare_builder	();	
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireApproachAbstract::execute()
{
	// установка параметров функциональных блоков
	inherited::object->set_action								(ACT_RUN);
	inherited::object->anim().accel_activate					(eAT_Aggressive);
	inherited::object->anim().accel_set_braking				(false);
	inherited::object->path().set_target_point					(inherited::object->EnemyMan.get_enemy()->Position(), inherited::object->EnemyMan.get_enemy()->ai_location().level_vertex_id());
	inherited::object->path().set_rebuild_time					(inherited::object->get_attack_rebuild_time());
	inherited::object->path().set_use_covers					(false);
	inherited::object->path().set_distance_to_end				(0.1f);
	inherited::object->set_state_sound							(MonsterSound::eMonsterSoundAggressive);
}

