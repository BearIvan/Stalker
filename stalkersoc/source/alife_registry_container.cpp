////////////////////////////////////////////////////////////////////////////
//	Module 		: alife_registry_container.cpp
//	Created 	: 01.07.2004
//  Modified 	: 01.07.2004
//	Author		: Dmitriy Iassenev
//	Description : ALife registry container class
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "alife_registry_container.h"
#include "engine/object_interfaces.h"
#include "alife_space.h"
#include "engine/object_type_traits.h"
/*
template <typename T1, typename _T2, typename Head>
struct CRegistryHelperLoad {
	typedef typename object_type_traits::remove_reference<_T2>::type T2;

	template <bool loadable>
	IC	static void do_load(T1 *self, T2 &p1)
	{
	}

	template <>
	IC	static void do_load<true>(T1 *self, T2 &p1)
	{
		self->Head::load(p1);
	}

	IC	static void process(T1 *self, T2 &p1)
	{
		do_load<object_type_traits::is_base_and_derived<ISerializable,Head>::value>(self,p1);
	}
};

template <typename T1, typename _T2, typename Head>
struct CRegistryHelperSave {
	typedef typename object_type_traits::remove_reference<_T2>::type T2;

	template <bool loadable>
	IC	static void do_save(T1 *self, T2 &p1)
	{
	}

	template <>
	IC	static void do_save<true>(T1 *self, T2 &p1)
	{
		self->Head::save(p1);
	}

	IC	static void process(T1 *self, T2 &p1)
	{
		do_save<object_type_traits::is_base_and_derived<ISerializable,Head>::value>(self,p1);
	}
};

template <template <typename _1, typename _2, typename _3> class helper, typename T1, typename T2, typename TList>
class CRegistryHelperProcess
{
private:
	ASSERT_TYPELIST(TList);

	typedef typename TList::Head Head;
	typedef typename TList::Tail Tail;

public:
	template <typename _1>
	IC	static void go_process(T1 *self, T2 p1)
	{
		CRegistryHelperProcess<helper,T1,T2,Tail>::process(self,p1);
	}

	template <>
	IC	static void go_process<Loki::NullType>(T1 *self, T2 p1)
	{
	}

	IC	static void process(T1 *self, T2 p1)
	{
		go_process<Tail>(self,p1);
		helper<T1,T2,Head>::process(self,p1);
	}
};

void CALifeRegistryContainer::load(IReader &file_stream)
{
	R_ASSERT2					(file_stream.find_chunk(REGISTRY_CHUNK_DATA),"Can't find chunk REGISTRY_CHUNK_DATA!");
	CRegistryHelperProcess<
		CRegistryHelperLoad,
		CALifeRegistryContainer,
		IReader&,
		TYPE_LIST
	>::process					(this,file_stream);
}

void CALifeRegistryContainer::save(IWriter &memory_stream)
{
	memory_stream.open_chunk	(REGISTRY_CHUNK_DATA);
	CRegistryHelperProcess<
		CRegistryHelperSave,
		CALifeRegistryContainer,
		IWriter&,
		TYPE_LIST
	>::process					(this,memory_stream);
	memory_stream.close_chunk	();
}
*/
template <typename TContainer, typename TList>
struct RegistryHelper;

template <typename TContainer>
struct RegistryHelper<TContainer, Loki::NullType>
{
	static void Save(TContainer*, IWriter&) {};
	static void Load(TContainer*, IReader&) {};
};

template <typename TContainer, typename Head, typename Tail>
struct RegistryHelper<TContainer, Loki::Typelist<Head, Tail>>
{
	static constexpr bool isSerializable =
		object_type_traits::is_base_and_derived<ISerializable, Head>::value;

	static void Save(TContainer* self, IWriter& writer)
	{
		RegistryHelper<TContainer, Tail>::Save(self, writer);
		if constexpr (isSerializable)
			self->Head::save(writer);
	};

	static void Load(TContainer* self, IReader& reader)
	{
		RegistryHelper<TContainer, Tail>::Load(self, reader);
		if constexpr (isSerializable)
			self->Head::load(reader);
	}
};

void CALifeRegistryContainer::load(IReader& file_stream)
{
	R_ASSERT2(file_stream.find_chunk(REGISTRY_CHUNK_DATA), "Can't find chunk REGISTRY_CHUNK_DATA!");
	RegistryHelper<CALifeRegistryContainer, TYPE_LIST>::Load(this, file_stream);
}

void CALifeRegistryContainer::save(IWriter& memory_stream)
{
	memory_stream.open_chunk(REGISTRY_CHUNK_DATA);
	RegistryHelper<CALifeRegistryContainer, TYPE_LIST>::Save(this, memory_stream);
	memory_stream.close_chunk();
}
