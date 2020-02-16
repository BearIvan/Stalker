#pragma once
#include "xrRender/RenderVisual.h"
class XRayRenderVisual :public IRenderVisual
{
public:
	virtual ~XRayRenderVisual();
	virtual void				Load(const char* N, IReader* data, u32 dwFlags);
	virtual void				Copy(XRayRenderVisual* from);
	virtual void				Depart() {};
	virtual void				Spawn() {};
	virtual void				Release() {}
	virtual void Render(float LOD) {};
	virtual u32					getType();
	virtual vis_data& 	getVisData();
#ifdef DEBUG
	virtual shared_str		getDebugName() ;
#endif
public:
	XRayBlender Shader;
	vis_data					Vis;
	u32 Type;
#ifdef DEBUG
	shared_str DebugName;
#endif
};