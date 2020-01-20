////////////////////////////////////////////////////////////////////////////
//	Module 		: level_location_selector_inline.h
//	Created 	: 02.10.2001
//  Modified 	: 18.11.2003
//	Author		: Dmitriy Iassenev
//	Description : Level location selector inline functions
////////////////////////////////////////////////////////////////////////////

#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _VertexEvaluator,\
	typename _vertex_id_type\
>

#define CLevelLocationSelector CBaseLocationSelector<CLevelGraph,_VertexEvaluator,_vertex_id_type>

TEMPLATE_SPECIALIZATION
IC	CLevelLocationSelector::CBaseLocationSelector	(CRestrictedObject *object) :
	inherited(object, inherited::selector_manager)
{
}

TEMPLATE_SPECIALIZATION
IC	void CLevelLocationSelector::before_search	(_vertex_id_type &vertex_id)
{
	if (inherited::m_restricted_object) {
		if (!inherited::m_restricted_object->accessible(vertex_id)) {
			Fvector dest_pos;
			vertex_id	= inherited::m_restricted_object->accessible_nearest(inherited::m_graph->vertex_position(vertex_id),dest_pos);
		}
		inherited::m_restricted_object->add_border(vertex_id, inherited::m_evaluator->m_fRadius);
	}
}

TEMPLATE_SPECIALIZATION
IC	void CLevelLocationSelector::after_search	()
{
	if (inherited::m_restricted_object)
		inherited::m_restricted_object->remove_border();
}

#undef TEMPLATE_SPECIALIZATION
#undef CLevelLocationSelector
