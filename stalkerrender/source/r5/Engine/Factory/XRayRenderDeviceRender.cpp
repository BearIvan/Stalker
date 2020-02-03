#include "pch.h"

XRayRenderDeviceRender::XRayRenderDeviceRender()
{

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
	GRenderInterface.destroy();
	bear_delete(GResourcesManager);
}

void XRayRenderDeviceRender::ValidateHW()
{
}

void XRayRenderDeviceRender::DestroyHW()
{
	/*bear_delete(GXRayRenderResource);*/
	bear_delete(HW);
	BearRenderInterface::Destroy();
	
}

void XRayRenderDeviceRender::Reset(BearWindow&viewport)
{
	HW->Update(viewport);
	/*BearGraphics::BearRenderInterface::SetViewport(0, 0, viewport.GetSizeFloat().x, viewport.GetSizeFloat().y);
	GXRayRenderResource->Resize(viewport.GetSizeFloat().x, viewport.GetSizeFloat().y);*/

}

void XRayRenderDeviceRender::SetupStates()
{
}

void XRayRenderDeviceRender::OnDeviceCreate(LPCSTR shName)
{
	GResourcesManager = xr_new<XRayResourcesManager>();
	GResourcesManager->LoadShaders(shName);
	GRenderInterface.create();
	/*GXRayRenderResource->LoadShaders(shName);*/
}

void XRayRenderDeviceRender::Create(BearWindow&viewport, bool)
{

	
	HW = bear_new<XRayHardware>();
	HW->Update(viewport);
	/*m_viewport = &viewport;
	BearGraphics::BearRenderInterface::AttachRenderTargetView(viewport);
	BearGraphics::BearRenderInterface::SetViewport(0, 0, viewport.GetSizeFloat().x, viewport.GetSizeFloat().y);
	GXRayRenderResource->Resize(viewport.GetSizeFloat().x, viewport.GetSizeFloat().y);*/
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

bsize XRayRenderDeviceRender::GetCacheStatPolys()
{
	return 0;
}

void XRayRenderDeviceRender::Begin()
{
	HW->Context->AttachFrameBuffer(HW->FrameBuffer);
	HW->Context->SetViewport(0, 0, static_cast<float>(RDEVICE.dwWidth), static_cast<float>(RDEVICE.dwHeight));
	HW->Context->ClearFrameBuffer();
}

void XRayRenderDeviceRender::Clear()
{
	//m_viewport->ClearColor(BearColor::Black);
}

void XRayRenderDeviceRender::End()
{
	HW->Context->Flush(true);
	HW->Viewport->Copy(HW->TBasicColor);
	GUIRender.Flush();
	for (auto b = HW->FontRenders.begin(), e = HW->FontRenders.end(); b != e; b++)
	{
		(*b)->Flush();
	}

	//test.Push();

	//plane->Draw();
	/*uiRender.StartPrimitive(6, uiRender.ptTriList, uiRender.pttTL);
	uiRender.PushPoint(100.0f, -100.0f, 0, 0xFFFFFFFF, 0, 0);
	uiRender.PushPoint(100.0f, 100.0f, 0, 0xFFFFFFFF, 0, 1);
	uiRender.PushPoint(-100.0f, 100.0f, 0, 0xFFFFFFFF, 1, 0);
	uiRender.FlushPrimitive();*/
	//m_viewport->Swap();
}

void XRayRenderDeviceRender::ClearTarget()
{
}

void XRayRenderDeviceRender::SetCacheXform(Fmatrix & mView, Fmatrix & mProject)
{
	GRenderInterface.SetView(mView);
	GRenderInterface.SetProject(mProject);
	/*GXRayRenderResource->SetView(mView);
	GXRayRenderResource->SetProject(mProject);*/
}

void XRayRenderDeviceRender::OnAssetsChanged()
{
}
