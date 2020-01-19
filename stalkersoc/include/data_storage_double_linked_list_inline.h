////////////////////////////////////////////////////////////////////////////
//	Module 		: data_storage_double_linked_list_inline.h
//	Created 	: 21.03.2002
//  Modified 	: 26.02.2004
//	Author		: Dmitriy Iassenev
//	Description : Double linked list data storage inline functions
////////////////////////////////////////////////////////////////////////////

#pragma once

#define TEMPLATE_SPECIALIZATION \
	template <bool sorted>\
	template <\
		typename _data_storage,\
		template <typename _T> class _vertex\
	>
#ifdef MSVC
#define CDoubleLinkedList	CDataStorageDoubleLinkedList<sorted>::CDataStorage<_data_storage,_vertex>
#else
#define CDoubleLinkedList	CDataStorageDoubleLinkedList<sorted>::template CDataStorage<_data_storage,_vertex>
#endif
TEMPLATE_SPECIALIZATION
//IC	CDoubleLinkedList::CDataStorage(const u32 vertex_count, const _dist_type _max_distance = _dist_type(u32(-1))) :
IC	CDoubleLinkedList::CDataStorage			(const u32 vertex_count, const _dist_type _max_distance ) :
	inherited				(vertex_count)
{
	m_switch_factor			= _dist_type(1);
}

TEMPLATE_SPECIALIZATION
CDoubleLinkedList::~CDataStorage			()
{
}

TEMPLATE_SPECIALIZATION
IC	void CDoubleLinkedList::init			()
{
	inherited::init			();
	inherited::m_list_tail->prev()		= inherited::m_list_head;
}

TEMPLATE_SPECIALIZATION
IC	void CDoubleLinkedList::add_opened		(CGraphVertex &vertex)
{
	inherited_base::add_opened	(vertex);
	if (!sorted) {
		inherited::m_list_head->next()->prev()	= &vertex;
		vertex.next()				= inherited::m_list_head->next();
		inherited::m_list_head->next()			= &vertex;
		vertex.prev()				= inherited::m_list_head;
	}
	else {
		for (CGraphVertex *i = inherited::m_list_head->next(); ; i = i->next())
			if (i->f() >= vertex.f()) {
				vertex.next()		= i;
				vertex.prev()		= i->prev();
				i->prev()->next()	= &vertex;
				i->prev()			= &vertex;
				break;
			}
	}
}

TEMPLATE_SPECIALIZATION
IC	void CDoubleLinkedList::decrease_opened	(CGraphVertex &vertex, const _dist_type value)
{
	VERIFY					(!inherited::is_opened_empty());
	
	if (!sorted)
		return;

	if (vertex.prev()->f() <= vertex.f())
		return;

	vertex.prev()->next()			= vertex.next();
	vertex.next()->prev()			= vertex.prev();

	if (vertex.f() <= inherited::m_list_head->next()->f()) {
		vertex.prev()			= inherited::m_list_head;
		vertex.next()			= inherited::m_list_head->next();
		inherited::m_list_head->next()->prev() = &vertex;
		inherited::m_list_head->next()		= &vertex;
	}
	else {
		if (vertex.prev()->f() - vertex.f() < m_switch_factor*(vertex.f() - inherited::m_list_head->next()->f()))
			for (CGraphVertex *i = vertex.prev()->prev(); ; i = i->prev()) {
				if (i->f() <= vertex.f()) {
					vertex.next() = i->next();
					vertex.prev() = i;
					i->next()->prev() = &vertex;
					i->next() = &vertex;
					break;
				}
			}
		else
			for (CGraphVertex *i = inherited::m_list_head->next(); ; i = i->next())
				if (i->f() >= vertex.f()) {
					vertex.next() = i;
					vertex.prev() = i->prev();
					i->prev()->next() = &vertex;
					i->prev() = &vertex;
					break;
				}
	}
}

TEMPLATE_SPECIALIZATION
IC	void CDoubleLinkedList::remove_best_opened	()
{
	VERIFY					(!inherited::is_opened_empty());
	inherited::m_list_head->next()->next()->prev()	= inherited::m_list_head;
	inherited::remove_best_opened();
}

TEMPLATE_SPECIALIZATION
IC	typename CDoubleLinkedList::CGraphVertex &CDoubleLinkedList::get_best	() const
{
	VERIFY					(!inherited::is_opened_empty());
	if (sorted)
		return				(*inherited::m_list_head->next());

	_dist_type				fmin = inherited::m_max_distance;
	CGraphVertex* best = 0;
	for (CGraphVertex *i = inherited::m_list_head->next(); i; i = i->next())
		if (i->f() < fmin) {
			fmin			= i->f();
			best			= i;
		}
	VERIFY					(best);
	if (best->prev() != inherited::m_list_head) {
		best->prev()->next()		= best->next();
		best->next()->prev()		= best->prev();
		best->next()				= inherited::m_list_head->next();
		best->prev()				= inherited::m_list_head;
		inherited::m_list_head->next()->prev() = best;
		inherited::m_list_head->next()			= best;
	}
	return					(*inherited::m_list_head->next());
}

TEMPLATE_SPECIALIZATION
IC	void CDoubleLinkedList::set_switch_factor	(const _dist_type _switch_factor)
{
	if (!sorted)
		NODEFAULT;
	m_switch_factor			= _switch_factor;
}


#undef TEMPLATE_SPECIALIZATION
#undef CDoubleLinkedList