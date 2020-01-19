#pragma once

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0501
#endif // _WIN32_WINNT

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>

#pragma warning(disable:4995)
	#include <intrin.h>
#pragma warning(default:4995)


#define ECORE_API
#define XRCPU_PIPE
#ifdef _EDITOR
#	include "skeletonX.h" 
#	include "skeletoncustom.h"
#else // _EDITOR
	#include "tools/xrCore.h"
	#include "xrRender/SkeletonXVertRender.h"
#define ENGINE_API
	#include "engine/bone.h"
	#define RENDER 1
	#include "engine/Render.h"
#undef ENGINE_API
	#include "engine/Device.h"
	#include "XrRender/light.h"
#endif // _EDITOR

#include "xrCPU_Pipe.h"