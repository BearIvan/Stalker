#include "pch.h"

XRayUIRender::XRayUIRender():m_IndexCount(0),m_LIT_iMaxVerts(0),m_TL_iMaxVerts(0), m_shader(0)
{
	
}

XRayUIRender::~XRayUIRender()
{
}

void XRayUIRender::CreateUIGeom()
{
}

void XRayUIRender::DestroyUIGeom()
{
	m_IndexBuffer.Clear();
	m_TL_buffer.Clear();
	m_LIT_buffer.Clear();
}

void XRayUIRender::SetShader(IUIShader & shader)
{
	m_shader = &dynamic_cast<XRayUIShader*>(&shader)->shader;
}

void XRayUIRender::SetAlphaRef(int aref)
{
}

void XRayUIRender::SetScissor(Irect * rect)
{
	if (rect)
	{
		BearGraphics::BearRenderInterface::SetScissor(static_cast<float>(rect->x1), static_cast<float>(rect->y1), static_cast<float>(rect->x2), static_cast<float>(rect->y2));
	}
	else
	{
		BearGraphics::BearRenderInterface::DisableScissor();
	}
	
}

void XRayUIRender::GetActiveTextureResolution(Fvector2 & res)
{
	auto size = m_shader->GetSizeTextureFromPixel(0);
	res.set(size.x, size.y);
}

void XRayUIRender::PushPoint(float x, float y, float z, u32 C, float u, float v)
{
	switch (m_PointType)
	{
	case pttLIT:
		LIT_pv->set(x, y, z, C, u, v);
		++LIT_pv;
		break;
	case pttTL:
		TL_pv->set(x, y, C, u, v);
		++TL_pv;
		break;
	}
}

void XRayUIRender::StartPrimitive(u32 iMaxVerts, ePrimitiveType primType, ePointType pointType)
{
	//iMaxVerts = iMaxVerts * 3;
	if (iMaxVerts > m_IndexCount)
	{
		m_IndexBuffer.Create(iMaxVerts * sizeof(uint32));
		auto list = (uint32*)m_IndexBuffer.Lock();
		for (bsize i = 0; i < iMaxVerts; i++)
		{
			list[i] = i;// 2 - (i % 3) + ((i / 3) * 3);
		}
		m_IndexCount = iMaxVerts;
		m_IndexBuffer.Unlock();
	}
	switch (pointType)
	{
	case IUIRender::pttTL:
		if (iMaxVerts > m_TL_iMaxVerts) {
			m_TL_buffer.Create(iMaxVerts * sizeof(FVF::TL));
			m_TL_iMaxVerts = iMaxVerts;
			
			
		}
		TL_pv = (FVF::TL*)m_TL_buffer.Lock();
		TL_start_pv = TL_pv;
		break;
	case IUIRender::pttLIT:
		if (iMaxVerts > m_LIT_iMaxVerts) {
			m_LIT_buffer.Create(iMaxVerts * sizeof(FVF::LIT));
			m_LIT_iMaxVerts = iMaxVerts;
			
		
		}
		LIT_pv = (FVF::LIT*)m_LIT_buffer.Lock();
		LIT_start_pv = LIT_pv;
		break;
	default:
		BEAR_ASSERT(0);
	}
	m_PrimitiveType = primType;
	m_PointType = pointType;
}

void XRayUIRender::FlushPrimitive()
{
	bsize primCount = 0;
	bsize p_cnt = 0;

	GXRayRenderResource->SetShader(*m_shader);
	switch (m_PointType)
	{
	case pttLIT:
		p_cnt = LIT_pv - LIT_start_pv;
		BEAR_ASSERT(p_cnt <= m_LIT_iMaxVerts);
		m_LIT_buffer.Unlock();
		BearGraphics::BearRenderInterface::SetVertexBuffer(m_LIT_buffer);
		break;
	case pttTL:
		p_cnt = TL_pv - TL_start_pv;
		BEAR_ASSERT(p_cnt <= m_TL_iMaxVerts);
		m_TL_buffer.Unlock();
		BearGraphics::BearRenderInterface::SetVertexBuffer(m_TL_buffer);
		break;
	default:
		NODEFAULT;
	}
	BearGraphics::BearRenderInterface::SetIndexBuffer(m_IndexBuffer);
	BearGraphics::BearDrawType DrawType = BearGraphics::DT_TRIANGLE;
	//	Update data for primitive type

	switch (m_PrimitiveType)
	{
	case ptTriStrip:
	case ptTriList:
		primCount = p_cnt;
		DrawType = BearGraphics::DT_TRIANGLE;
		break;
	case ptLineStrip:
	case ptLineList:
		primCount = p_cnt;
		DrawType = BearGraphics::DT_POINT;
		break;
	default:
		NODEFAULT;
	}

	if (primCount > 0)
	{
		BearGraphics::BearRenderInterface::Draw(primCount, 0, DrawType);
	}
	m_PrimitiveType = ptNone;
	m_PointType = pttNone;
}

LPCSTR XRayUIRender::UpdateShaderName(LPCSTR tex_name, LPCSTR sh_name)
{
	return LPCSTR();
}

void XRayUIRender::CacheSetXformWorld(const Fmatrix & M)
{
	GXRayRenderResource->SetWorld(M);
}

void XRayUIRender::CacheSetCullMode(CullMode)
{
}
