#include "pch.h"

XRayDebugRender::XRayDebugRender()
{
}

void XRayDebugRender::Render()
{
}

void XRayDebugRender::add_lines(Fvector const* vertices, bsize const& vertex_count, u16 const* pairs, bsize const& pair_count, u32 const& color)
{
}

void XRayDebugRender::NextSceneMode()
{
}

void XRayDebugRender::ZEnable(bool bEnable)
{
}

void XRayDebugRender::OnFrameEnd()
{
}

void XRayDebugRender::SetShader(const debug_shader& shader)
{
}

void XRayDebugRender::CacheSetXformWorld(const Fmatrix& M)
{
}

void XRayDebugRender::CacheSetCullMode(CullMode mode)
{
}

void XRayDebugRender::SetAmbient(u32 colour)
{
}

void XRayDebugRender::SetDebugShader(dbgShaderHandle shdHandle)
{
}

void XRayDebugRender::DestroyDebugShader(dbgShaderHandle shdHandle)
{
}

void XRayDebugRender::dbg_DrawTRI(Fmatrix& T, Fvector& p1, Fvector& p2, Fvector& p3, u32 C)
{
}
