#pragma once
class XRayFProgressive:public XRayFVisual
{
protected:
	FSlideWindowItem	nSWI;
	FSlideWindowItem* xSWI;
	u32					last_lod;
public:
	XRayFProgressive();
	virtual ~XRayFProgressive();
	virtual void				Load(const char* N, IReader* data, u32 dwFlags);
	virtual void				Copy(XRayRenderVisual* from);
	virtual void 		Release();
};