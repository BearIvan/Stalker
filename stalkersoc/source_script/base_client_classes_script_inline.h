////////////////////////////////////////////////////////////////////////////
//	Module 		: base_client_classes_script.cpp
//	Created 	: 20.12.2004
//  Modified 	: 20.12.2004
//	Author		: Dmitriy Iassenev
//	Description : XRay base client classes script export
////////////////////////////////////////////////////////////////////////////

#include "base_client_classes.h"
#include "base_client_classes_wrappers.h"
#include "engine/feel_sound.h"
#include "XrRender/fbasicvisual.h"
#include "XrRender/skeletonanimated.h"
#include "ai/stalker/ai_stalker.h"
using namespace luabind;

#pragma optimize("s",on)
template<>
inline void DLL_PureScript::script_register	(lua_State *L)
{
	module(L)
	[
		class_<DLL_Pure, no_bases, default_holder,CDLL_PureWrapper>("DLL_Pure")
			.def(constructor<>())
			.def("_construct",&DLL_Pure::_construct,&CDLL_PureWrapper::_construct_static)
	];
}
template<>
inline void ISheduledScript::script_register	(lua_State *L)
{
	module(L)
	[
		class_<ISheduled, no_bases, default_holder, CISheduledWrapper>("ISheduled")
//			.def(constructor<>())
//			.def("shedule_Scale",		&ISheduled::shedule_Scale,		&CISheduledWrapper::shedule_Scale_static)
//			.def("shedule_Update",		&ISheduled::shedule_Update,		&CISheduledWrapper::shedule_Update_static)
	];
}
template<>
inline void IRenderableScript::script_register	(lua_State *L)
{
	module(L)
	[
		class_<IRenderable, no_bases, default_holder, CIRenderableWrapper>("IRenderable")
//			.def(constructor<>())
//			.def("renderable_Render",&IRenderable::renderable_Render,&CIRenderableWrapper::renderable_Render_static)
//			.def("renderable_ShadowGenerate",&IRenderable::renderable_ShadowGenerate,&CIRenderableWrapper::renderable_ShadowGenerate_static)
//			.def("renderable_ShadowReceive",&IRenderable::renderable_ShadowReceive,&CIRenderableWrapper::renderable_ShadowReceive_static)
	];
}
template<>
inline void ICollidableScript::script_register	(lua_State *L)
{
	module(L)
	[
		class_<ICollidable>("ICollidable")
			.def(constructor<>())
	];
}
template<>
inline void CObjectScript::script_register		(lua_State *L)
{
	module(L)
	[
//		class_<CObject,bases<DLL_Pure,ISheduled,ICollidable,IRenderable>,CObjectWrapper>("CObject")
//			.def(constructor<>())
//			.def("_construct",			&CObject::_construct,&CObjectWrapper::_construct_static)
		
//			.def("shedule_Scale",		&CObject::shedule_Scale,		&CObjectWrapper::shedule_Scale_static)
//			.def("shedule_Update",		&CObject::shedule_Update,		&CObjectWrapper::shedule_Update_static)

//			.def("renderable_Render"		,&CObject::renderable_Render,&CObjectWrapper::renderable_Render_static)
//			.def("renderable_ShadowGenerate",&CObject::renderable_ShadowGenerate,&CObjectWrapper::renderable_ShadowGenerate_static)
//			.def("renderable_ShadowReceive",&CObject::renderable_ShadowReceive,&CObjectWrapper::renderable_ShadowReceive_static)
//			.def("Visual",					&CObject::Visual)

		class_<CGameObject,bases<DLL_Pure,ISheduled,ICollidable,IRenderable>,default_holder,CGameObjectWrapper>("CGameObject")
			.def(constructor<>())
			.def("_construct",			&CGameObject::_construct,&CGameObjectWrapper::_construct_static)
			.def("Visual",				&CGameObject::Visual)

//			.def("shedule_Scale",		&CGameObject::shedule_Scale,		&CGameObjectWrapper::shedule_Scale_static)
//			.def("shedule_Update",		&CGameObject::shedule_Update,		&CGameObjectWrapper::shedule_Update_static)

//			.def("renderable_Render"		,&CGameObject::renderable_Render,&CGameObjectWrapper::renderable_Render_static)
//			.def("renderable_ShadowGenerate",&CGameObject::renderable_ShadowGenerate,&CGameObjectWrapper::renderable_ShadowGenerate_static)
//			.def("renderable_ShadowReceive",&CGameObject::renderable_ShadowReceive,&CGameObjectWrapper::renderable_ShadowReceive_static)

			.def("net_Export",			&CGameObject::net_Export,		&CGameObjectWrapper::net_Export_static)
			.def("net_Import",			&CGameObject::net_Import,		&CGameObjectWrapper::net_Import_static)
			.def("net_Spawn",			&CGameObject::net_Spawn,	&CGameObjectWrapper::net_Spawn_static)

			.def("use",					&CGameObject::use,	&CGameObjectWrapper::use_static)

//			.def("setVisible",			&CGameObject::setVisible)
			.def("getVisible",			&CGameObject::getVisible)
			.def("getEnabled",			&CGameObject::getEnabled)
//			.def("setEnabled",			&CGameObject::setEnabled)

//		,class_<CPhysicsShellHolder,CGameObject>("CPhysicsShellHolder")
//			.def(constructor<>())

//		,class_<CEntity,CPhysicsShellHolder,CEntityWrapper>("CEntity")
//			.def(constructor<>())
//			.def("HitSignal",&CEntity::HitSignal,&CEntityWrapper::HitSignal_static)
//			.def("HitImpulse",&CEntity::HitImpulse,&CEntityWrapper::HitImpulse_static)

//		,class_<CEntityAlive,CEntity>("CEntityAlive")
//			.def(constructor<>())

//		,class_<CCustomMonster,CEntityAlive>("CCustomMonster")
//			.def(constructor<>())

//		,class_<CAI_Stalker,CCustomMonster>("CAI_Stalker")
	];
}
template<>
inline void IRenderVisualScript::script_register		(lua_State *L)
{
	module(L)
	[
		class_<IRenderVisual>("IRenderVisual")
			//.def(constructor<>())
			.def("dcast_PKinematicsAnimated",&IRenderVisual::dcast_PKinematicsAnimated)
	];
}
inline void IKinematicsAnimated_PlayCycle(IKinematicsAnimated* sa, LPCSTR anim)
{
	sa->PlayCycle(anim);
}
template<>
inline void IKinematicsAnimatedScript::script_register		(lua_State *L)
{
	module(L)
	[
		class_<IKinematicsAnimated>("IKinematicsAnimated")
			.def("PlayCycle",		&IKinematicsAnimated_PlayCycle)
	];
}
template<>
inline void CBlendScript::script_register		(lua_State *L)
{
	module(L)
		[
			class_<CBlend>("CBlend")
			//			.def(constructor<>())
		];
}

/*
void IKinematicsScript::script_register		(lua_State *L)
{
	module(L)
		[
			class_<IKinematics, FHierrarhyVisual>("IKinematics")
			//			.def(constructor<>())
		];
}

void FHierrarhyVisualScript::script_register		(lua_State *L)
{
	module(L)
		[
			class_<FHierrarhyVisual, IRenderVisual>("FHierrarhyVisual")
			//			.def(constructor<>())
		];
}
*/