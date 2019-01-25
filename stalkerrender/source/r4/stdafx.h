// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#pragma warning(disable:4995)
#include "engine/stdafx.h"
#pragma warning(disable:4995)
#include "directx/d3dx9.h"
#pragma warning(default:4995)
#pragma warning(disable:4714)
#pragma warning( 4 : 4018 )
#pragma warning( 4 : 4244 )
#pragma warning(disable:4237)

#include "directx/D3D11.h"
#include "directx/D3Dx11core.h"
#include "directx/D3DCompiler.h"

#include "xrRender/xrD3DDefs.h"

#include "xrRender/Debug/dxPixEventWrapper.h"

#define		R_R1	1
#define		R_R2	2
#define		R_R3	3
#define		R_R4	4
#define		RENDER	R_R4

#include "particles/psystem.h"

#include "xrRender/HW.h"
#include "xrRender/Shader.h"
#include "xrRender/R_Backend.h"
#include "xrRender/R_Backend_Runtime.h"

#include "xrRender/resourcemanager.h"

#include "engine/vis_common.h"
#include "engine/render.h"
#include "engine/_d3d_extensions.h"
#include "engine/igame_level.h"
#include "xrRender/blenders\blender.h"
#include "xrRender/blenders\blender_clsid.h"
#include "xrRender/xrRender_console.h"
#include "r4.h"
#include "api/XrGameVersionController.h"
#include "engine/EnvironmentSOC.h"

inline CEnvironment& GetEnv()
{
	BEAR_ASSERT(gameVersionController->getGame() != GameVersionController::SOC);
	return ENV;
}
IC	void	jitter(CBlender_Compile& C)
{
//	C.r_Sampler	("jitter0",	JITTER(0), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
//	C.r_Sampler	("jitter1",	JITTER(1), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
//	C.r_Sampler	("jitter2",	JITTER(2), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
//	C.r_Sampler	("jitter3",	JITTER(3), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
	C.r_dx10Texture	("jitter0",	JITTER(0));
	C.r_dx10Texture	("jitter1",	JITTER(1));
	C.r_dx10Texture	("jitter2",	JITTER(2));
	C.r_dx10Texture	("jitter3",	JITTER(3));
	C.r_dx10Texture	("jitter4",	JITTER(4));
	C.r_dx10Texture	("jitterMipped",	r2_jitter_mipped);
	C.r_dx10Sampler	("smp_jitter");
}
