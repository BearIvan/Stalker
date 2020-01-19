////////////////////////////////////////////////////////////////////////////
//	Module 		: CEdge.h
//	Created 	: 14.01.2004
//  Modified 	: 19.02.2005
//	Author		: Dmitriy Iassenev
//	Description : Graph edge class template
////////////////////////////////////////////////////////////////////////////

#pragma once

template <
	typename __edge_weight_type,
	typename __vertex_type
>
class CEdge {
public:
	using _edge_weight_type						= __edge_weight_type;
	using  _vertex_type							=__vertex_type;
	typedef typename _vertex_type::_vertex_id_type	_vertex_id_type;

private:
	_edge_weight_type				m_weight;
	_vertex_type					*m_vertex;

public:
	IC								CEdge		(const _edge_weight_type &weight, _vertex_type *vertex);
	IC		const _edge_weight_type	&weight		() const;
	IC		_vertex_type			*vertex		() const;
	IC		const _vertex_id_type	&vertex_id	() const;
	IC		bool					operator==	(const _vertex_id_type &vertex_id) const;
	IC		bool					operator==	(const CEdge &obj) const;
};

#include "graph_edge_inline.h"