#include "pch.h"

BearRenderDeviceRender::BearRenderDeviceRender()
{
}

void BearRenderDeviceRender::Copy(IRenderDeviceRender & _in)
{
}

void BearRenderDeviceRender::setGamma(float fGamma)
{
}

void BearRenderDeviceRender::setBrightness(float fGamma)
{
}

void BearRenderDeviceRender::setContrast(float fGamma)
{
}

void BearRenderDeviceRender::updateGamma()
{
}

void BearRenderDeviceRender::OnDeviceDestroy(BOOL bKeepTextures)
{
}

void BearRenderDeviceRender::ValidateHW()
{
}

void BearRenderDeviceRender::DestroyHW()
{
}

void BearRenderDeviceRender::Reset(BearUI::BearViewport&viewport)
{
}

void BearRenderDeviceRender::SetupStates()
{
}

void BearRenderDeviceRender::OnDeviceCreate(LPCSTR shName)
{
}

void BearRenderDeviceRender::Create(BearUI::BearViewport&viewport, bool)
{
}

void BearRenderDeviceRender::SetupGPU(BOOL bForceGPU_SW, BOOL bForceGPU_NonPure, BOOL bForceGPU_REF)
{
}

void BearRenderDeviceRender::overdrawBegin()
{
}

void BearRenderDeviceRender::overdrawEnd()
{
}

void BearRenderDeviceRender::DeferredLoad(BOOL E)
{
}

void BearRenderDeviceRender::ResourcesDeferredUpload()
{
}

void BearRenderDeviceRender::ResourcesGetMemoryUsage(u32 & m_base, u32 & c_base, u32 & m_lmaps, u32 & c_lmaps)
{
}

void BearRenderDeviceRender::ResourcesDestroyNecessaryTextures()
{
}

void BearRenderDeviceRender::ResourcesStoreNecessaryTextures()
{
}

void BearRenderDeviceRender::ResourcesDumpMemoryUsage()
{
}

bool BearRenderDeviceRender::HWSupportsShaderYUV2RGB()
{
	return false;
}

IRenderDeviceRender::DeviceState BearRenderDeviceRender::GetDeviceState()
{
	return DeviceState();
}

BOOL BearRenderDeviceRender::GetForceGPU_REF()
{
	return 0;
}

u32 BearRenderDeviceRender::GetCacheStatPolys()
{
	return u32();
}

void BearRenderDeviceRender::Begin()
{
}

void BearRenderDeviceRender::Clear()
{
}

void BearRenderDeviceRender::End()
{
}

void BearRenderDeviceRender::ClearTarget()
{
}

void BearRenderDeviceRender::SetCacheXform(Fmatrix & mView, Fmatrix & mProject)
{
}

void BearRenderDeviceRender::OnAssetsChanged()
{
}
