#include "pch.h"

BearDebugRender::BearDebugRender()
{
}

void BearDebugRender::Render()
{
}

void BearDebugRender::add_lines(Fvector const * vertices, u32 const & vertex_count, u16 const * pairs, u32 const & pair_count, u32 const & color)
{
}

void BearDebugRender::NextSceneMode()
{
}

void BearDebugRender::ZEnable(bool bEnable)
{
}

void BearDebugRender::OnFrameEnd()
{
}

void BearDebugRender::SetShader(const debug_shader & shader)
{
}

void BearDebugRender::CacheSetXformWorld(const Fmatrix & M)
{
}

void BearDebugRender::CacheSetCullMode(CullMode mode)
{
}

void BearDebugRender::SetAmbient(u32 colour)
{
}

void BearDebugRender::SetDebugShader(dbgShaderHandle shdHandle)
{
}

void BearDebugRender::DestroyDebugShader(dbgShaderHandle shdHandle)
{
}

void BearDebugRender::dbg_DrawTRI(Fmatrix & T, Fvector & p1, Fvector & p2, Fvector & p3, u32 C)
{
}
