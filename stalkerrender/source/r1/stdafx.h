// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifdef MSVC
#pragma warning(disable:4995)
#endif
#include "engine/stdafx.h"
#ifdef MSVC
#pragma warning(disable:4995)
#include "directx/d3dx9.h"
#include "directx/d3d9.h"
#pragma warning(default:4995)
#else
#include <d3d9.h>
#include <d3dx9.h>
#endif


#include "xrRender/xrD3DDefs.h"

#include "xrRender/HW.h"
#include "xrRender/Shader.h"
#include "xrRender/R_Backend.h"
#include "xrRender/R_Backend_Runtime.h"

#define		R_R1	1
#define		R_R2	2
#define		R_R3	3
#define		R_R4	4
#define		RENDER	R_R1

#include "xrRender/resourcemanager.h"

#include "engine/vis_common.h"
#include "engine/render.h"
#include "engine/_d3d_extensions.h"

#ifndef _EDITOR
#include "engine/igame_level.h"

#include "xrRender/blenders\blender.h"
#include "xrRender/blenders\blender_clsid.h"
#include "particles/psystem.h"
#include "xrRender/xrRender_console.h"
#include "FStaticRender.h"
#endif

#define		TEX_POINT_ATT	"internal\\internal_light_attpoint"
#define		TEX_SPOT_ATT	"internal\\internal_light_attclip"

#define LUABIND_NO_EXCEPTIONS