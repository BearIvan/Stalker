#pragma once
class XRayFHierrarhyVisual:public XRayRenderVisual
{
public:
	BearVector<XRayRenderVisual*>		children;
public:
	virtual void				Load(const char* N, IReader* data, u32 dwFlags);
	virtual void				Copy(XRayRenderVisual* from);
	virtual void	Release();
	virtual void Render(float LOD);
public:
	XRayFHierrarhyVisual();
	virtual ~XRayFHierrarhyVisual();

private:
	BOOL							m_DontDelete;

};