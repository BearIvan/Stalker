////////////////////////////////////////////////////////////////////////////
//	Module 		: script_engine_export.cpp
//	Created 	: 01.04.2004
//  Modified 	: 22.06.2004
//	Author		: Dmitriy Iassenev
//	Description : XRay Script Engine export
////////////////////////////////////////////////////////////////////////////
#include "pch_script.h"

#define SCRIPT_REGISTRATOR
#include "script_export_space.h"
#include "script_engine_export.h"

#include "game_cl_single_script_inline.h"
#include "action_base_script_inline.h"
#include "action_planner_script_inline.h"
#include "action_planner_action_script_export_inline.h"
#include "script_render_device_script_inline.h"
#include "base_client_classes_script_inline.h"
#include "script_fcolor_script_inline.h"
#include "script_flags_script_inline.h"
#include "script_fmatrix_script_inline.h"
#include "script_fvector_script_inline.h"
#include "script_net_packet_script_inline.h"
#include "script_reader_script_inline.h"
#include "property_evaluator_script_inline.h"
#pragma optimize("s",on)
template <typename TList> struct Register
{
	ASSERT_TYPELIST(TList);

	inline static void _Register(lua_State *L)
	{
		Register<typename TList::Tail>::_Register(L);
#ifdef XRGAME_EXPORTS
#	ifdef _DEBUG
		Msg("Export:%s", typeid(typename TList::Head).name());
#	endif
#endif
		TList::Head::script_register(L);
	}
};

template <> struct Register<Loki::NullType>
{
	static void _Register(lua_State *L)
	{
	}
};
/*
template <typename T1, typename T2>
struct TypePair {
	typedef T1	first;
	typedef T2	second;
};

template<bool C, class T = void>
using enable_if_t = typename std::enable_if<C, T>::type;
template <typename TFullList> struct DynamicCast
{
	ASSERT_TYPELIST(TFullList);

	template <typename TList, typename T> 
	struct Helper2
	{
		typedef typename TList::Head Head;

		static void Register(lua_State *L)
		{
			Helper2<TList::Tail,T>::Register(L);
			declare<Loki::SuperSubclassStrict<Head,T>::value>();
		}

		template <bool b>
		static void declare(enable_if_t<!b>)
		{
		}


		template <bool b>
		static void declare(enable_if_t<b>)
		{
			Msg		("Exporting function to cast from \"%s\" to \"%s\"",typeid(T).name(),typeid(Head).name());
		}
	};

	template <typename T> struct Helper2<Loki::NullType,T>
	{
		static void Register(lua_State *L)
		{
		}
	};

	template <typename TList> struct Helper
	{
		static void Register(lua_State *L)
		{
			Helper<TList::Tail>::Register(L);
			Helper2<TFullList,TList::Head>::Register(L);
		}
	};

	template <> struct Helper<Loki::NullType>
	{
		static void Register(lua_State *L)
		{
		}
	};

	static void Register(lua_State *L)
	{
		Helper<TFullList>::Register(L);
	}
};*/
void export_classes	(lua_State *L)
{
	Register<script_type_list>::_Register(L);
//	DynamicCast<script_type_list>::Register(L);
//	Register<Loki::TL::DerivedToFrontAll<script_type_list>::Result>::_Register(L);
}
