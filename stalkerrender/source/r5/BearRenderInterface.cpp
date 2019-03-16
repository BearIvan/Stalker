#include "pch.h"

BearRenderInterface::BearRenderInterface()
{
}

IRender_interface::GenerationLevel BearRenderInterface::get_generation()
{
	return GenerationLevel();
}

bool BearRenderInterface::is_sun_static()
{
	return false;
}

DWORD BearRenderInterface::get_dx_level()
{
	return 0;
}

void BearRenderInterface::create()
{
}

void BearRenderInterface::destroy()
{
}

void BearRenderInterface::reset_begin()
{
}

void BearRenderInterface::reset_end()
{
}

void BearRenderInterface::level_Load(IReader *)
{
}

void BearRenderInterface::level_Unload()
{
}

HRESULT BearRenderInterface::shader_compile(LPCSTR name, DWORD const * pSrcData, UINT SrcDataLen, LPCSTR pFunctionName, LPCSTR pTarget, DWORD Flags, void *& result)
{
	return E_NOTIMPL;
}

LPCSTR BearRenderInterface::getShaderPath()
{
	return LPCSTR();
}

IRender_Sector * BearRenderInterface::getSector(int id)
{
	return nullptr;
}

IRenderVisual * BearRenderInterface::getVisual(int id)
{
	return nullptr;
}

IRender_Sector * BearRenderInterface::detectSector(const Fvector & P)
{
	return nullptr;
}

IRender_Target * BearRenderInterface::getTarget()
{
	return nullptr;
}

void BearRenderInterface::set_Transform(Fmatrix * M)
{
}

void BearRenderInterface::set_HUD(BOOL V)
{
}

BOOL BearRenderInterface::get_HUD()
{
	return 0;
}

void BearRenderInterface::set_Invisible(BOOL V)
{
}

void BearRenderInterface::flush()
{
}

void BearRenderInterface::set_Object(IRenderable * O)
{
}

void BearRenderInterface::add_Occluder(Fbox2 & bb_screenspace)
{
}

void BearRenderInterface::add_Visual(IRenderVisual * V)
{
}

void BearRenderInterface::add_Geometry(IRenderVisual * V)
{
}

void BearRenderInterface::add_StaticWallmark(const wm_shader & S, const Fvector & P, float s, CDB::TRI * T, Fvector * V)
{
}

void BearRenderInterface::add_StaticWallmark(IWallMarkArray * pArray, const Fvector & P, float s, CDB::TRI * T, Fvector * V)
{
}

void BearRenderInterface::clear_static_wallmarks()
{
}

void BearRenderInterface::add_SkeletonWallmark(const Fmatrix * xf, IKinematics * obj, IWallMarkArray * pArray, const Fvector & start, const Fvector & dir, float size)
{
}

IRender_ObjectSpecific * BearRenderInterface::ros_create(IRenderable * parent)
{
	return nullptr;
}

void BearRenderInterface::ros_destroy(IRender_ObjectSpecific *&)
{
}

IRender_Light * BearRenderInterface::light_create()
{
	return nullptr;
}

IRender_Glow * BearRenderInterface::glow_create()
{
	return nullptr;
}

IRenderVisual * BearRenderInterface::model_CreateParticles(LPCSTR name)
{
	return nullptr;
}

IRenderVisual * BearRenderInterface::model_Create(LPCSTR name, IReader * data)
{
	return nullptr;
}

IRenderVisual * BearRenderInterface::model_CreateChild(LPCSTR name, IReader * data)
{
	return nullptr;
}

IRenderVisual * BearRenderInterface::model_Duplicate(IRenderVisual * V)
{
	return nullptr;
}

void BearRenderInterface::model_Delete(IRenderVisual *& V, BOOL bDiscard)
{
}

void BearRenderInterface::model_Logging(BOOL bEnable)
{
}

void BearRenderInterface::models_Prefetch()
{
}

void BearRenderInterface::models_Clear(BOOL b_complete)
{
}

BOOL BearRenderInterface::occ_visible(vis_data & V)
{
	return 0;
}

BOOL BearRenderInterface::occ_visible(Fbox & B)
{
	return 0;
}

BOOL BearRenderInterface::occ_visible(sPoly & P)
{
	return 0;
}

void BearRenderInterface::Calculate()
{
}

void BearRenderInterface::Render()
{
}

void BearRenderInterface::Screenshot(ScreenshotMode mode, LPCSTR name)
{
}

void BearRenderInterface::Screenshot(ScreenshotMode mode, CMemoryWriter & memory_writer)
{
}

void BearRenderInterface::ScreenshotAsyncBegin()
{
}

void BearRenderInterface::ScreenshotAsyncEnd(CMemoryWriter & memory_writer)
{
}

void BearRenderInterface::rmNear()
{
}

void BearRenderInterface::rmFar()
{
}

void BearRenderInterface::rmNormal()
{
}

u32 BearRenderInterface::memory_usage()
{
	return u32();
}

void BearRenderInterface::ScreenshotImpl(ScreenshotMode mode, LPCSTR name, CMemoryWriter * memory_writer)
{
}
