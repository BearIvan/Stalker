#pragma once
class XRayUIRender:public IUIRender
{
public:
	XRayUIRender();
	~XRayUIRender();
	virtual void CreateUIGeom();
	virtual void DestroyUIGeom();

	virtual void SetShader(IUIShader &shader);
	virtual void SetAlphaRef(int aref);
	//.	virtual void StartTriList(u32 iMaxVerts);
	//.	virtual void FlushTriList();
	//.	virtual void StartTriFan(u32 iMaxVerts);
	//.	virtual void FlushTriFan();

		//virtual void StartTriStrip(u32 iMaxVerts);
		//virtual void FlushTriStrip();
	//.	virtual void StartLineStrip(u32 iMaxVerts);
	//.	virtual void FlushLineStrip();
	//.	virtual void StartLineList(u32 iMaxVerts);
	//.	virtual void FlushLineList();
	virtual void SetScissor(Irect* rect = NULL);
	virtual void GetActiveTextureResolution(Fvector2 &res);

	//.	virtual void PushPoint(float x, float y, u32 c, float u, float v);
	//.	virtual void PushPoint(int x, int y, u32 c, float u, float v);
	virtual void PushPoint(float x, float y, float z, u32 C, float u, float v);

	virtual void StartPrimitive(bsize iMaxVerts, ePrimitiveType primType, ePointType pointType);
	virtual void FlushPrimitive();
	virtual void Flush();
	virtual LPCSTR	UpdateShaderName(LPCSTR tex_name, LPCSTR sh_name);

	virtual void	CacheSetXformWorld(const Fmatrix& M);
	virtual void	CacheSetCullMode(CullMode);
private:
	BearVector < BearFactoryPointer<BearRHI::BearRHIVertexBuffer>> m_vertex_buffersLIT;
	BearVector < BearFactoryPointer<BearRHI::BearRHIVertexBuffer>>::iterator m_vertex_bufferLIT_current;
	BearVector < BearFactoryPointer<BearRHI::BearRHIVertexBuffer>> m_vertex_buffersTL;
	BearVector < BearFactoryPointer<BearRHI::BearRHIVertexBuffer>>::iterator m_vertex_bufferTL_current;
	ePrimitiveType	m_PrimitiveType;
	ePointType		m_PointType;

	XRayBlender Blender;

	bsize TL_iMaxVerts;
	FVF::TL* TL_start_pv;
	FVF::TL* TL_pv;

	bsize LIT_iMaxVerts;
	FVF::LIT* LIT_start_pv;
	FVF::LIT* LIT_pv;
};
extern XRayUIRender GUIRender;