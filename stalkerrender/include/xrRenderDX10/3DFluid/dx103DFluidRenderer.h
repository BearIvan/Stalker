#ifndef	dx103DFluidRenderer_included
#define	dx103DFluidRenderer_included
#pragma once

#ifdef MSVC
#pragma warning(push)
#pragma warning(disable:4005)
#include "directx\DXGI.h"
#pragma warning(pop)
#include "directx\d3dx10math.h"
#include "directx\d3dx10core.h"
#include "directx\d3dx10tex.h"
#include "directx\d3dx10mesh.h"
#include "directx\d3dx10async.h"
#include "directx\d3dx11.h"
#include "directx\d3d11.h"
#else
#include <DXGI.h>
#include <d3dx10math.h>
#include <d3dx10core.h>
#include <d3dx10tex.h>
#include <d3dx10mesh.h>
#include <d3dx10async.h>
#include <d3dx11.h>
#include <d3d11.h>
#endif

class dx103DFluidRenderer
{
public:
	enum Renderer_RT
	{
		RRT_RayDataTex = 0,
		RRT_RayDataTexSmall,
		RRT_RayCastTex,
		RRT_EdgeTex,
		RRT_NumRT
	};

public:
	dx103DFluidRenderer();
	~dx103DFluidRenderer();

	void	Initialize(bsize gridWidth, bsize gridHeight, int gridDepth);
	void	Destroy();

	void	SetScreenSize(bsize width, bsize height);

	void	Draw(const dx103DFluidData &FluidData);

	static LPCSTR*	GetRTNames() { return m_pRTNames; }
	static LPCSTR*	GetResourceRTNames() { return m_pResourceRTNames; }

private:
	enum RendererShader
	{
		RS_CompRayData_Back = 0,
		RS_CompRayData_Front,
		RS_QuadDownSampleRayDataTexture,
		
		RS_QuadEdgeDetect,
		RS_QuadRaycastFog,
		RS_QuadRaycastCopyFog,
		RS_QuadRaycastFire,
		RS_QuadRaycastCopyFire,

		RS_NumShaders
	};

	struct FogLighting 
	{
		Fvector3	m_vLightIntencity;

		void Reset() { ZeroMemory(this, sizeof(*this));}
	};

private:
	void	InitShaders();
	void	DestroyShaders();

	void	CreateGridBox();
	void	CreateScreenQuad();
	void	CreateJitterTexture();
	void	CreateHHGGTexture();

	void	CalculateRenderTextureSize(int screenWidth, int screenHeight);
	void	CreateRayDataResources( int width, int height );

	void	ComputeRayData();
	void	ComputeEdgeTexture();

	void	DrawScreenQuad();
	void	DrawBox();

	void	CalculateLighting(const dx103DFluidData &FluidData, FogLighting  &LightData);

private:
	bool		m_bInited;

	Fvector3	m_vGridDim;
	float		m_fMaxDim;

	int			m_iRenderTextureWidth;
	int			m_iRenderTextureHeight;

	D3DXMATRIX	m_gridMatrix;
	//Fmatrix		m_gridMatrix;

	D3DFORMAT				RTFormats[ RRT_NumRT ];
	ref_rt					RT[ RRT_NumRT ];
	static LPCSTR			m_pRTNames[ RRT_NumRT ];
	static LPCSTR			m_pResourceRTNames[ RRT_NumRT ];

	ref_selement	m_RendererTechnique[ RS_NumShaders ];

	ref_texture		m_JitterTexture;
	ref_texture		m_HHGGTexture;

	ref_geom					m_GeomGridBox;
	ID3DVertexBuffer			*m_pGridBoxVertexBuffer;
	ID3DIndexBuffer				*m_pGridBoxIndexBuffer;
	int							m_iGridBoxVertNum;
	int							m_iGridBoxFaceNum;

	ref_geom					m_GeomQuadVertex;
	ID3DVertexBuffer			*m_pQuadVertexBuffer;

	//	Cache vectors to avoid memory reallocations
	//	TODO: DX10: Reserve memory on object creation
	xr_vector<ISpatial*>		m_lstRenderables;
};

#endif	//	dx103DFluidRenderer_included