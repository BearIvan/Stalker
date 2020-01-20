#pragma once

#include "../../../sound_player.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterLookToUnprotectedAreaAbstract CStateMonsterLookToUnprotectedArea<_Object>

TEMPLATE_SPECIALIZATION
CStateMonsterLookToUnprotectedAreaAbstract::CStateMonsterLookToUnprotectedArea(_Object *obj) : inherited(obj, &data)
{
}

TEMPLATE_SPECIALIZATION
CStateMonsterLookToUnprotectedAreaAbstract::~CStateMonsterLookToUnprotectedArea()
{
}

TEMPLATE_SPECIALIZATION
void CStateMonsterLookToUnprotectedAreaAbstract::initialize()
{
	inherited::initialize();
	
	Fvector position;
	position = inherited::object->Position();
	position.y += 0.3f;

	float angle = ai().level_graph().vertex_cover_angle(inherited::object->ai_location().level_vertex_id(),XrMath::PI_DIV_6,std::less<float>());

	Fvector dir;
	dir.set(1.f,0.f,0.f);
	dir.setHP(angle+XrMath::M_PI, 0.f);
	dir.normalize();

	target_point.mad(inherited::object->Position(),dir, 1.f);
}

TEMPLATE_SPECIALIZATION
void CStateMonsterLookToUnprotectedAreaAbstract::execute()
{
	inherited::object->anim().m_tAction				= data.action;
	inherited::object->anim().SetSpecParams			(data.spec_params);
	inherited::object->dir().face_target				(target_point);

	if (data.sound_type != u32(-1)) {
		if (data.sound_delay != u32(-1))
			inherited::object->sound().play(data.sound_type, 0,0,data.sound_delay);
		else 
			inherited::object->sound().play(data.sound_type);
	}

}

TEMPLATE_SPECIALIZATION
bool CStateMonsterLookToUnprotectedAreaAbstract::check_completion()
{	
	if (data.time_out !=0) {
		if (inherited::time_state_started + data.time_out < Device.dwTimeGlobal) return true;
	} else 	if (!inherited::object->control().direction().is_turning()) return true;

	return false;
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterLookToUnprotectedAreaAbstract
