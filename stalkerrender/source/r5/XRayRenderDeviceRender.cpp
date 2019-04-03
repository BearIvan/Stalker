#include "pch.h"
extern const bchar* GGraphicsAPI[];
extern bsize GCurrentApi;

XRayRenderDeviceRender::XRayRenderDeviceRender()
{
	BearGraphics::BearRenderInterface::Initialize(GGraphicsAPI[GCurrentApi]);
	GXRayRenderResource = bear_new<XRayRenderResource>();
}

XRayRenderDeviceRender::~XRayRenderDeviceRender()
{

}

void XRayRenderDeviceRender::Copy(IRenderDeviceRender & _in)
{
	BEAR_ASSERT(0);
}

void XRayRenderDeviceRender::setGamma(float fGamma)
{
}

void XRayRenderDeviceRender::setBrightness(float fGamma)
{
}

void XRayRenderDeviceRender::setContrast(float fGamma)
{
}

void XRayRenderDeviceRender::updateGamma()
{
}

void XRayRenderDeviceRender::OnDeviceDestroy(BOOL bKeepTextures)
{
}

void XRayRenderDeviceRender::ValidateHW()
{
}

void XRayRenderDeviceRender::DestroyHW()
{
	bear_delete(GXRayRenderResource);
	BearGraphics::BearRenderInterface::Destroy();
	
}

void XRayRenderDeviceRender::Reset(BearUI::BearViewport&viewport)
{
	BearGraphics::BearRenderInterface::SetViewport(0, 0, viewport.GetSizeFloat().x, viewport.GetSizeFloat().y);
	GXRayRenderResource->Resize(viewport.GetSizeFloat().x, viewport.GetSizeFloat().y);

}

void XRayRenderDeviceRender::SetupStates()
{
}

void XRayRenderDeviceRender::OnDeviceCreate(LPCSTR shName)
{
	GXRayRenderResource->LoadShaders(shName);
}

void XRayRenderDeviceRender::Create(BearUI::BearViewport&viewport, bool)
{
	m_viewport = &viewport;
	BearGraphics::BearRenderInterface::AttachRenderTargetView(viewport);
	BearGraphics::BearRenderInterface::SetViewport(0, 0, viewport.GetSizeFloat().x, viewport.GetSizeFloat().y);
	GXRayRenderResource->Resize(viewport.GetSizeFloat().x, viewport.GetSizeFloat().y);
}

void XRayRenderDeviceRender::SetupGPU(BOOL bForceGPU_SW, BOOL bForceGPU_NonPure, BOOL bForceGPU_REF)
{
}

void XRayRenderDeviceRender::overdrawBegin()
{
}

void XRayRenderDeviceRender::overdrawEnd()
{
}

void XRayRenderDeviceRender::DeferredLoad(BOOL E)
{
}

void XRayRenderDeviceRender::ResourcesDeferredUpload()
{
}

void XRayRenderDeviceRender::ResourcesGetMemoryUsage(u32 & m_base, u32 & c_base, u32 & m_lmaps, u32 & c_lmaps)
{
}

void XRayRenderDeviceRender::ResourcesDestroyNecessaryTextures()
{
}

void XRayRenderDeviceRender::ResourcesStoreNecessaryTextures()
{
}

void XRayRenderDeviceRender::ResourcesDumpMemoryUsage()
{
}

bool XRayRenderDeviceRender::HWSupportsShaderYUV2RGB()
{
	return true;
}

IRenderDeviceRender::DeviceState XRayRenderDeviceRender::GetDeviceState()
{
	return DeviceState();
}

BOOL XRayRenderDeviceRender::GetForceGPU_REF()
{
	return 0;
}

u32 XRayRenderDeviceRender::GetCacheStatPolys()
{
	return u32();
}

void XRayRenderDeviceRender::Begin()
{
}

void XRayRenderDeviceRender::Clear()
{

	m_viewport->ClearColor(BearColor::Black);
}

void XRayRenderDeviceRender::End()
{
	//test.Push();

	//plane->Draw();
	/*uiRender.StartPrimitive(6, uiRender.ptTriList, uiRender.pttTL);
	uiRender.PushPoint(100.0f, -100.0f, 0, 0xFFFFFFFF, 0, 0);
	uiRender.PushPoint(100.0f, 100.0f, 0, 0xFFFFFFFF, 0, 1);
	uiRender.PushPoint(-100.0f, 100.0f, 0, 0xFFFFFFFF, 1, 0);
	uiRender.FlushPrimitive();*/
	m_viewport->Swap();
}

void XRayRenderDeviceRender::ClearTarget()
{
}

void XRayRenderDeviceRender::SetCacheXform(Fmatrix & mView, Fmatrix & mProject)
{
	GXRayRenderResource->SetView(mView);
	GXRayRenderResource->SetProject(mProject);
}

void XRayRenderDeviceRender::OnAssetsChanged()
{
}
