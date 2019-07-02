#ifndef SPACE_UTILS_H
#define SPACE_UTILS_H

#include "ode/src/collision_kernel.h"

IC void spatialParsFromDGeom(dGeomID d_space,Fvector& center,Fvector& AABB,float& radius)
{
	
	d_space->computeAABB();
	dReal* dAABB=d_space->aabb;
	center.set(
		(dAABB[0]+dAABB[1])/2.f,
		(dAABB[2]+dAABB[3])/2.f,
		(dAABB[4]+dAABB[5])/2.f
		);
	AABB.x=dAABB[1]-center.x,AABB.y=dAABB[3]-center.y,AABB.z=dAABB[5]-center.z;
	radius=XrMath::max(AABB.x,XrMath::max(AABB.y,AABB.z));
}
#endif