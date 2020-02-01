#include "pch.h"
#include "engine/IGame_Persistent.h"
const bchar* GGraphicsAPI[] =
{ 
		TEXT("bear_opengl"),
	TEXT("bear_directx11"),
	TEXT("bear_directx10"),

};
static const bchar* ShaderPath[] =
{ 
	TEXT("opengl"),
	TEXT("directx11"),
	TEXT("directx10"),
};
bsize GCurrentApi=0;
XRayRenderInterface::XRayRenderInterface()
{

}

IRender_interface::GenerationLevel XRayRenderInterface::get_generation()
{
	return GenerationLevel();
}

bool XRayRenderInterface::is_sun_static()
{
	return true;
}

DWORD XRayRenderInterface::get_dx_level()
{
	return 0;
}

void XRayRenderInterface::create()
{

}

void XRayRenderInterface::destroy()
{
}

void XRayRenderInterface::reset_begin()
{
}

void XRayRenderInterface::reset_end()
{
}

void XRayRenderInterface::level_Load(IReader *)
{
}

void XRayRenderInterface::level_Unload()
{
}

HRESULT XRayRenderInterface::shader_compile(LPCSTR name, DWORD const * pSrcData, UINT SrcDataLen, LPCSTR pFunctionName, LPCSTR pTarget, DWORD Flags, void *& result)
{
	return E_NOTIMPL;
}

LPCSTR XRayRenderInterface::getShaderPath()
{
	static BearStringPath path;
	BearString::Copy(path, TEXT("r5"));
	BearString::Contact(path, BEAR_PATH);
	BearString::Contact(path, ShaderPath[GCurrentApi]);
	return path;
}

IRender_Sector * XRayRenderInterface::getSector(int id)
{
	return nullptr;
}

IRenderVisual * XRayRenderInterface::getVisual(int id)
{
	return nullptr;
}

IRender_Sector * XRayRenderInterface::detectSector(const Fvector & P)
{
	return nullptr;
}

IRender_Target * XRayRenderInterface::getTarget()
{
	return &m_RenderTarget;
}

void XRayRenderInterface::set_Transform(Fmatrix * M)
{
}

void XRayRenderInterface::set_HUD(BOOL V)
{
}

BOOL XRayRenderInterface::get_HUD()
{
	return 0;
}

void XRayRenderInterface::set_Invisible(BOOL V)
{
}

void XRayRenderInterface::flush()
{
}

void XRayRenderInterface::set_Object(IRenderable * O)
{
}

void XRayRenderInterface::add_Occluder(Fbox2 & bb_screenspace)
{
}

void XRayRenderInterface::add_Visual(IRenderVisual * V)
{
}

void XRayRenderInterface::add_Geometry(IRenderVisual * V)
{
}

void XRayRenderInterface::add_StaticWallmark(const wm_shader & S, const Fvector & P, float s, CDB::TRI * T, Fvector * V)
{
}

void XRayRenderInterface::add_StaticWallmark(IWallMarkArray * pArray, const Fvector & P, float s, CDB::TRI * T, Fvector * V)
{
}

void XRayRenderInterface::clear_static_wallmarks()
{
}

void XRayRenderInterface::add_SkeletonWallmark(const Fmatrix * xf, IKinematics * obj, IWallMarkArray * pArray, const Fvector & start, const Fvector & dir, float size)
{
}

IRender_ObjectSpecific * XRayRenderInterface::ros_create(IRenderable * parent)
{
	return nullptr;
}

void XRayRenderInterface::ros_destroy(IRender_ObjectSpecific *&)
{
}

IRender_Light * XRayRenderInterface::light_create()
{
	return nullptr;
}

IRender_Glow * XRayRenderInterface::glow_create()
{
	return nullptr;
}

IRenderVisual * XRayRenderInterface::model_CreateParticles(LPCSTR name)
{
	return nullptr;
}

IRenderVisual * XRayRenderInterface::model_Create(LPCSTR name, IReader * data)
{
	return nullptr;
}

IRenderVisual * XRayRenderInterface::model_CreateChild(LPCSTR name, IReader * data)
{
	return nullptr;
}

IRenderVisual * XRayRenderInterface::model_Duplicate(IRenderVisual * V)
{
	return nullptr;
}

void XRayRenderInterface::model_Delete(IRenderVisual *& V, BOOL bDiscard)
{
}

void XRayRenderInterface::model_Logging(BOOL bEnable)
{
}

void XRayRenderInterface::models_Prefetch()
{
}

void XRayRenderInterface::models_Clear(BOOL b_complete)
{
}

BOOL XRayRenderInterface::occ_visible(vis_data & V)
{
	return 0;
}

BOOL XRayRenderInterface::occ_visible(Fbox & B)
{
	return 0;
}

BOOL XRayRenderInterface::occ_visible(sPoly & P)
{
	return 0;
}

void XRayRenderInterface::Calculate()
{
}

void XRayRenderInterface::Render()
{
	if (g_pGamePersistent)	g_pGamePersistent->OnRenderPPUI_main();
}

void XRayRenderInterface::Screenshot(ScreenshotMode mode, LPCSTR name)
{
}

void XRayRenderInterface::Screenshot(ScreenshotMode mode, CMemoryWriter & memory_writer)
{
}

void XRayRenderInterface::ScreenshotAsyncBegin()
{
}

void XRayRenderInterface::ScreenshotAsyncEnd(CMemoryWriter & memory_writer)
{
}

void XRayRenderInterface::rmNear()
{
}

void XRayRenderInterface::rmFar()
{
}

void XRayRenderInterface::rmNormal()
{
}

u32 XRayRenderInterface::memory_usage()
{
	return u32();
}

void XRayRenderInterface::ScreenshotImpl(ScreenshotMode mode, LPCSTR name, CMemoryWriter * memory_writer)
{
}


