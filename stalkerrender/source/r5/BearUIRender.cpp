#include "pch.h"

BearUIRender::BearUIRender()
{
}

void BearUIRender::CreateUIGeom()
{
}

void BearUIRender::DestroyUIGeom()
{
}

void BearUIRender::SetShader(IUIShader & shader)
{
}

void BearUIRender::SetAlphaRef(int aref)
{
}

void BearUIRender::SetScissor(Irect * rect)
{
}

void BearUIRender::GetActiveTextureResolution(Fvector2 & res)
{
}

void BearUIRender::PushPoint(float x, float y, float z, u32 C, float u, float v)
{
}

void BearUIRender::StartPrimitive(u32 iMaxVerts, ePrimitiveType primType, ePointType pointType)
{
}

void BearUIRender::FlushPrimitive()
{
}

LPCSTR BearUIRender::UpdateShaderName(LPCSTR tex_name, LPCSTR sh_name)
{
	return LPCSTR();
}

void BearUIRender::CacheSetXformWorld(const Fmatrix & M)
{
}

void BearUIRender::CacheSetCullMode(CullMode)
{
}
