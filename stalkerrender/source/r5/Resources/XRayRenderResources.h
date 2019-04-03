#pragma once
#define REGISTER(name,byte,size) name=byte,
enum D3DVertexState
{
#include "D3D9VertexState.h"
};
#undef REGISTER
#include "xrRender/FVF.h"
class XRayShader;
class XRayRenderResource
{
public:
	XRayRenderResource();
	~XRayRenderResource();

	void LoadShaders(const bchar*name);
	bsize GetStride(uint32 VertexState);

	XRayConstant&GetConstant(const bchar*name);
	XRayMatrix&GetConstantMatrix(const bchar*name);

	void CreateShader(const bchar*name,XRayShader&shader);

	BearGraphics::BearVertexShaderRef GetVertexShader(const bchar*name);
	BearGraphics::BearPixelShaderRef GetPixelShader(const bchar*name);

	XRayTexture* GetTexture(const bchar*name);
	void Unload(XRayTexture* texture);

	void SetVertexState(uint32 VertexState, const bchar*vs);

	void SetView(const Fmatrix&mat);
	void SetProject(const Fmatrix&mat);
	void SetWorld(const Fmatrix&mat);
	void SetShader(XRayShader&sahder);
	void Resize(float wigth, float hieght);

private:
	void UpdateMatrix();
	Fmatrix m_MView, m_MProject, m_MWorld, m_MVPW,m_MWV;
	BearGraphics::BearShaderConstantsRef m_MatrixConstant;
private:
	BearGraphics::BearShaderConstantsRef m_ScreenResolution;
private:

	BearGraphics::BearVertexStateRef CreateVertexState(uint32 VertexState, const bchar*vs);
	XRayBlender*CreateBlender(CLASS_ID ID);
	void LoadShaders();
	void LoadShader(const bchar*name);
private:
	XRayBlender*GetBlender(const bchar*name);
	BearMap<uint32, BearGraphics::BearVertexStateRef> m_MapOfVertexState;
	uint32 m_CurrentVertexState;

	void CreateBlend(XRayShader&shader);
	BearCore::BearMap<uint16, BearGraphics::BearBlendStateRef> m_blend_map;
	uint16 m_blend_current;
	BearGraphics::BearBlendStateRef m_blend_default;


	BearMap<BearStringConteniar, BearGraphics::BearVertexShaderRef> m_MapOfVertexShader;
	BearMap<BearStringConteniar, XRayConstant> m_MapOfConstant;
	BearMap<BearStringConteniar, XRayMatrix> m_MapOfConstantMatrix;
	BearMap<BearStringConteniar, BearGraphics::BearPixelShaderRef > m_MapOfPixelShader;
	BearMap < BearStringConteniar, XRayBlender*>m_MapOfBlenders;
	BearMap < BearStringConteniar, XRayTexture* >m_MapOfTextures;
};
extern XRayRenderResource *GXRayRenderResource;