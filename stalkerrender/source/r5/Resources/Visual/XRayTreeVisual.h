#pragma once
class	XRayTreeVisual : public XRayRenderVisual
{
private:
	struct	_5color
	{
		Fvector					rgb;		// - all static lighting
		float					hemi;		// - hemisphere
		float					sun;		// - sun
	};
protected:
	_5color						c_scale;
	_5color						c_bias;
	Fmatrix						xform;
public:
	bsize CountIndex;
	bsize OffsetIndex;
	bsize CountVertex;
	bsize OffsetVertex;
	u32 FVF;
	BearFactoryPointer<BearRHI::BearRHIIndexBuffer> IndexBuffer;
	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> VertexBuffer;
public:
	virtual void Render(float LOD);									// LOD - Level Of Detail  [0.0f - min, 1.0f - max], Ignored
	virtual void Load(LPCSTR N, IReader* data, u32 dwFlags);
	virtual void Copy(XRayRenderVisual* pFrom);

	XRayTreeVisual(void);
	virtual ~XRayTreeVisual(void);
};

class XRayTreeVisual_ST : public XRayTreeVisual
{
	typedef XRayTreeVisual inherited;
public:
	XRayTreeVisual_ST(void);
	virtual			~XRayTreeVisual_ST(void);

	virtual void	Render(float LOD);									// LOD - Level Of Detail  [0.0f - min, 1.0f - max], Ignored
	virtual void	Load(LPCSTR N, IReader* data, u32 dwFlags);
	virtual void	Copy(XRayRenderVisual* pFrom);
};

class XRayTreeVisual_PM : public XRayTreeVisual
{
	typedef XRayTreeVisual inherited;
private:
	FSlideWindowItem* pSWI;
	u32					last_lod;
public:
	XRayTreeVisual_PM(void);
	virtual			~XRayTreeVisual_PM(void);

	virtual void	Render(float LOD);									// LOD - Level Of Detail  [0.0f - min, 1.0f - max], Ignored
	virtual void	Load(LPCSTR N, IReader* data, u32 dwFlags);
	virtual void	Copy(XRayRenderVisual* pFrom);
};
