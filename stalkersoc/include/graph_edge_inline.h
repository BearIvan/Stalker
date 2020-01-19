////////////////////////////////////////////////////////////////////////////
//	Module 		: graph_edge_inline.h
//	Created 	: 14.01.2004
//  Modified 	: 19.02.2005
//	Author		: Dmitriy Iassenev
//	Description : Graph edge class template inline functions
////////////////////////////////////////////////////////////////////////////

#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename __edge_weight_type,\
	typename __vertex_type\
>

#define CSGraphEdge  CEdge<__edge_weight_type,__vertex_type>

TEMPLATE_SPECIALIZATION
IC	CSGraphEdge::CEdge			(const __edge_weight_type &weight, __vertex_type *vertex)
{
	m_weight		= weight;
	VERIFY			(vertex);
	m_vertex		= vertex;
}

TEMPLATE_SPECIALIZATION
IC	 const typename CSGraphEdge::_edge_weight_type &CSGraphEdge::weight	() const
{
	return			(m_weight);
}

TEMPLATE_SPECIALIZATION
IC	 typename CSGraphEdge::_vertex_type *CSGraphEdge::vertex				() const
{
	return			(m_vertex);
}

TEMPLATE_SPECIALIZATION
IC	const  typename CSGraphEdge::_vertex_id_type &CSGraphEdge::vertex_id	() const
{
	return			(vertex()->vertex_id());
}

TEMPLATE_SPECIALIZATION
IC	bool CSGraphEdge::operator==	(const _vertex_id_type &vertex_id) const
{
	return			(vertex()->vertex_id() == vertex_id);
}

TEMPLATE_SPECIALIZATION
IC	bool CSGraphEdge::operator==	(const CEdge &obj) const
{
	if (weight() != obj.weight())
		return		(false);

	return			(vertex()->vertex_id() == obj.vertex()->vertex_id());
}

#undef TEMPLATE_SPECIALIZATION
#undef CSGraphEdge