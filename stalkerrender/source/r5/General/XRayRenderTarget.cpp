#include "pch.h"

XRayRenderTarget::XRayRenderTarget()
{
	{
		RTVBasicColor = GResourcesManager->GetTexture(RT_BASIC);
		RTVBasicColor->Texture = BearRenderInterface::CreateTexture2D(RDEVICE.dwWidth, RDEVICE.dwHeight, HW->Viewport->GetFormat());

		DSVBasic = GResourcesManager->GetTexture(DS_BASIC);
		DSVBasic->Texture = BearRenderInterface::CreateTexture2D(RDEVICE.dwWidth, RDEVICE.dwHeight, DSF_DEPTH32F);
		{
			BearRenderPassDescription RenderPassDescription;
			RenderPassDescription.RenderTargets[0].Clear = true;
			RenderPassDescription.RenderTargets[0].Color = BearColor::Black;
			RenderPassDescription.RenderTargets[0].Format = HW->Viewport->GetFormat();
			RenderPassDescription.DepthStencil.Clear = true;
			RenderPassDescription.DepthStencil.Depth = 1.f;
			RenderPassDescription.DepthStencil.Format = DSF_DEPTH32F;
			RenderPass_Base = BearRenderInterface::CreateRenderPass(RenderPassDescription);
		}
		{
			BearFrameBufferDescription FrameBufferDescription;
			FrameBufferDescription.RenderPass = RenderPass_Base;
			FrameBufferDescription.DepthStencil = DSVBasic->Texture;
			FrameBufferDescription.RenderTargets[0] = RTVBasicColor->Texture;
			FrameBuffer_Base = BearRenderInterface::CreateFrameBuffer(FrameBufferDescription);
		}
	}
	
}

XRayRenderTarget::~XRayRenderTarget()
{
	GResourcesManager->Free(RTVBasicColor);
	GResourcesManager->Free(DSVBasic);
}

void XRayRenderTarget::set_blur(float f)
{
}

void XRayRenderTarget::set_gray(float f)
{
}

void XRayRenderTarget::set_duality_h(float f)
{
}

void XRayRenderTarget::set_duality_v(float f)
{
}

void XRayRenderTarget::set_noise(float f)
{
}

void XRayRenderTarget::set_noise_scale(float f)
{
}

void XRayRenderTarget::set_noise_fps(float f)
{
}

void XRayRenderTarget::set_color_base(u32 f)
{
}

void XRayRenderTarget::set_color_gray(u32 f)
{
}

void XRayRenderTarget::set_color_add(const Fvector & f)
{
}

bsize XRayRenderTarget::get_width()
{
	return RDEVICE.dwWidth;
}

bsize XRayRenderTarget::get_height()
{
	return RDEVICE.dwHeight;
}

void XRayRenderTarget::set_cm_imfluence(float f)
{
}

void XRayRenderTarget::set_cm_interpolate(float f)
{
}

void XRayRenderTarget::set_cm_textures(const shared_str & tex0, const shared_str & tex1)
{
}
XRayRenderTarget* GRenderTarget = 0;