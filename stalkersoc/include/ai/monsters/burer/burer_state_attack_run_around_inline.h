#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateBurerAttackRunAroundAbstract CStateBurerAttackRunAround<_Object>


#define DIST_QUANT			10.f
#define TIME_RUN_AWAY		3500

TEMPLATE_SPECIALIZATION
CStateBurerAttackRunAroundAbstract::CStateBurerAttackRunAround(_Object *obj) : inherited(obj)
{
}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackRunAroundAbstract::initialize()
{
	inherited::initialize		();

	time_started				= Device.dwTimeGlobal;
	dest_direction.set			(0.f,0.f,0.f);

	// select point
	Fvector						dir_to_enemy, dir_from_enemy;
	dir_to_enemy.sub			(inherited::object->EnemyMan.get_enemy()->Position(),inherited::object->Position());
	dir_to_enemy.normalize		();

	dir_from_enemy.sub			(inherited::object->Position(),inherited::object->EnemyMan.get_enemy()->Position());
	dir_from_enemy.normalize	();

	float dist = inherited::object->Position().distance_to(inherited::object->EnemyMan.get_enemy()->Position());

	if (dist > 30.f) {							// бежать к врагу
		selected_point.mad(inherited::object->Position(),dir_to_enemy,DIST_QUANT);
	} else if ((dist < 20.f) && (dist > 4.f)) {	// убегать от врага
		selected_point.mad(inherited::object->Position(),dir_from_enemy,DIST_QUANT);
		dest_direction.sub			(inherited::object->EnemyMan.get_enemy()->Position(),selected_point);
		dest_direction.normalize	();
	} else {											// выбрать случайную позицию
		selected_point = random_position(inherited::object->Position(), DIST_QUANT);
		dest_direction.sub			(inherited::object->EnemyMan.get_enemy()->Position(),selected_point);
		dest_direction.normalize	();
	}
	
	inherited::object->path().prepare_builder();
}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackRunAroundAbstract::execute()
{
	if (!XrMath::fis_zero(dest_direction.square_magnitude())) {
		inherited::object->path().set_use_dest_orient		(true);
		inherited::object->path().set_dest_direction		(dest_direction);
	} else inherited::object->path().set_use_dest_orient	(false);


	inherited::object->set_action							(ACT_RUN);
	inherited::object->path().set_target_point			(selected_point);
	inherited::object->path().set_generic_parameters	();
	inherited::object->path().set_use_covers			(false);

	inherited::object->set_state_sound						(MonsterSound::eMonsterSoundAggressive);
}


TEMPLATE_SPECIALIZATION
bool CStateBurerAttackRunAroundAbstract::check_start_conditions()
{
	return true;
}

TEMPLATE_SPECIALIZATION
bool CStateBurerAttackRunAroundAbstract::check_completion()
{
	if ((time_started + TIME_RUN_AWAY < Device.dwTimeGlobal) || 
		(inherited::object->control().path_builder().is_moving_on_path() && inherited::object->control().path_builder().is_path_end(2.f))) {

		inherited::object->dir().face_target(inherited::object->EnemyMan.get_enemy());
		return true;
	}

	return false;
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateBurerAttackRunAroundAbstract
