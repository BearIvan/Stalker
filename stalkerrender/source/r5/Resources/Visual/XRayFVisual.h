#pragma once
#include "xrRender/RenderVisual.h"
#include "xrRender/Kinematics.h"
class XRayFVisual :public XRayRenderVisual
{
public:
	XRayFVisual();
	virtual ~XRayFVisual();
	virtual void Load(const char* N, IReader* data, u32 dwFlags);
	virtual void				Copy(XRayRenderVisual* from);
	virtual void Render(float LOD);
	bsize CountIndex;
	bsize OffsetIndex;
	bsize CountVertex;
	bsize OffsetVertex;
	u32 FVF;
	BearFactoryPointer<BearRHI::BearRHIIndexBuffer> IndexBuffer;
	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> VertexBuffer;
	


};