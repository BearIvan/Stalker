#include "pch.h"

XRayRenderTarget::XRayRenderTarget()
{
}

XRayRenderTarget::~XRayRenderTarget()
{
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
