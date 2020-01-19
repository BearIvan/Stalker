////////////////////////////////////////////////////////////////////////////
//	Module 		: path_manager_level_inline.h
//	Created 	: 21.03.2002
//  Modified 	: 03.03.2004
//	Author		: Dmitriy Iassenev
//	Description : Level path manager inline functions
////////////////////////////////////////////////////////////////////////////

#pragma once

#define TEMPLATE_SPECIALIZATION \
	template <\
		typename _DataStorage,\
		typename _dist_type,\
		typename _index_type,\
		typename _iteration_type\
	>

#define CLevelPathManager CPathManager<\
	CLevelGraph,\
	_DataStorage,\
	SBaseParameters<\
		_dist_type,\
		_index_type,\
		_iteration_type\
	>,\
	_dist_type,\
	_index_type,\
	_iteration_type\
>

TEMPLATE_SPECIALIZATION
CLevelPathManager::~CPathManager			()
{
}

TEMPLATE_SPECIALIZATION
IC	void CLevelPathManager::setup			(
		const _Graph			*_graph,
		_DataStorage			*_data_storage,
		xr_vector<_index_type>	*_path,
		const _index_type		&_start_node_index,
		const _index_type		&_goal_node_index,
		const _Parameters		&parameters
	)
{
	inherited::setup(
		_graph,
		_data_storage,
		_path,
		_start_node_index,
		_goal_node_index,
		parameters
	);
	m_distance_xz			= inherited::graph->header().cell_size();
	m_sqr_distance_xz		= XrMath::sqr(inherited::graph->header().cell_size());
//		square_size_y			= XrMath::sqr((float)(inherited::graph->header().factor_y()/32767.0));
//		size_y					= (float)(inherited::graph->header().factor_y()/32767.0);
}

TEMPLATE_SPECIALIZATION
IC	void CLevelPathManager::init			()
{
	const _Graph::CVertex	&tNode1	= *inherited::graph->vertex(inherited::start_node_index);
	inherited::graph->unpack_xz		(tNode1,x2,z2);
//		y2						= (float)(tNode1.position().y());
	
	const _Graph::CVertex	&tNode2	= *inherited::graph->vertex(inherited::goal_node_index);
	inherited::graph->unpack_xz		(tNode2,x3,z3);
//		y3						= (float)(tNode2.position().y());
	x1						= x2;
//		y1						= y2;
	z1						= z2;
}

TEMPLATE_SPECIALIZATION
IC	_dist_type CLevelPathManager::evaluate	(const _index_type &node_index1, const _index_type &node_index2, const _Graph::const_iterator &/**i/**/)
{
	VERIFY					(inherited::graph);
	
//		const _Graph::CVertex	&tNode1 = *inherited::graph->vertex(node_index2);

//		y2						= (float)(tNode1.position().y());

//		return					(XrMath::sqrt(square_size_y*(float)XrMath::sqr(y2 - y1) + m_sqr_distance_xz));
	return					(m_distance_xz);// + (y2 - y1)*size_y);
}

TEMPLATE_SPECIALIZATION
IC	_dist_type CLevelPathManager::estimate	(const _index_type &node_index) const
{
	VERIFY					(inherited::graph);
//		return					(XrMath::sqrt((float)(m_sqr_distance_xz*float(XrMath::sqr(x3 - x1) + XrMath::sqr(z3 - z1)) + square_size_y*(float)XrMath::sqr(y3 - y1))));
	return					(2*m_distance_xz*_dist_type(XrMath::abs(x3 - x1) + XrMath::abs(z3 - z1)));// + XrMath::abs(y3 - y1)*size_y);
//		int						x = XrMath::abs(x3 - x1);
//		int						z = XrMath::abs(z3 - z1);
//		return					(m_distance_xz*_dist_type(_min(x,z)*3 + 2*XrMath::abs(x - z)));
}

TEMPLATE_SPECIALIZATION
IC	bool CLevelPathManager::is_goal_reached	(const _index_type &node_index)
{
	if (node_index == inherited::goal_node_index)
		return				(true);
	
	best_node				= inherited::graph->vertex(node_index);
	inherited::graph->unpack_xz		(best_node,x1,z1);
//		y1						= (float)(best_node->position().y());

	return					(false);
}

TEMPLATE_SPECIALIZATION
IC	bool CLevelPathManager::is_limit_reached(const _iteration_type	iteration_count) const
{
	VERIFY					(inherited::data_storage);
	return					(inherited::is_limit_reached(iteration_count));
}

TEMPLATE_SPECIALIZATION
IC	bool CLevelPathManager::is_accessible	(const _index_type &vertex_id) const
{
	VERIFY					(inherited::graph);
//	return					(inherited::graph->valid_vertex_id(vertex_id));
	return					(inherited::graph->is_accessible(vertex_id));
}

TEMPLATE_SPECIALIZATION
IC	void CLevelPathManager::begin			(const _index_type &vertex_id, const_iterator &begin, const_iterator &end)
{
	inherited::graph->begin			(best_node,begin,end);
}

TEMPLATE_SPECIALIZATION
IC	const _index_type CLevelPathManager::get_value		(const_iterator &i) const
{
	return					(inherited::graph->value(best_node,i));
}

#undef TEMPLATE_SPECIALIZATION
#undef CLevelPathManager