#ifndef TRAFFIC_OPTIMIZATION_INCLUDED
#define TRAFFIC_OPTIMIZATION_INCLUDED

enum enum_traffic_optimization
{
	eto_none				=	0,
	eto_last_change			=	1 << 2,
};//enum enum_traffic_optimization

extern u32	g_sv_traffic_optimization_level;

#endif//#ifndef TRAFFIC_OPTIMIZATION_INCLUDED