#include "pch.h"

XRayShader::XRayShader()
{
	m_vertex_state = 0;
	m_transform_type = TT_Matrix;
	bear_fill(m_pixel_textures);
	bear_fill(m_vertex_textures);
}

XRayShader::~XRayShader()
{
	for (bsize i = 0; i < 16 && m_vertex_textures[i]; i++)
	{
		GXRayRenderResource->Unload(m_vertex_textures[i]);
	}
	for (bsize i = 0; i < 16 && m_pixel_textures[i]; i++)
	{
		GXRayRenderResource->Unload(m_pixel_textures[i]);
	}
}

void XRayShader::SetShader(const bchar * ps, const bchar * vs)
{
	m_vertex = GXRayRenderResource->GetVertexShader(vs);
	BearString::Copy(m_name_vertex, vs);
	m_pixel = GXRayRenderResource->GetPixelShader(ps);

}

void XRayShader::SetVertexState(uint32 state)
{
	m_vertex_state = state;
}

void XRayShader::SetTextureToPixel(bsize id, const bchar * texture)
{
	m_pixel_textures[id] = GXRayRenderResource->GetTexture(texture);
}

void XRayShader::SetTextureToVertex(bsize id, const bchar * texture)
{
	m_vertex_textures[id] = GXRayRenderResource->GetTexture(texture);
}

BearVector2<float> XRayShader::GetSizeTextureFromPixel(bsize id)
{
	if (m_pixel_textures[id])
	{
		return m_pixel_textures[id]->GetSize();
	}
	return BearVector2<float>();
}

BearVector2<float> XRayShader::GetSizeTextureFromVertex(bsize id )
{
	if (m_vertex_textures[id])
	{
		return m_vertex_textures[id]->GetSize();
	}
	return BearVector2<float>();
}

void XRayShader::SetTransformType(TransformType type)
{
	m_transform_type = type;
}
