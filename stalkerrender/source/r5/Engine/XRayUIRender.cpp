#include "pch.h"

XRayUIRender::XRayUIRender()
{
	m_vertex_bufferTL_current = m_vertex_buffersTL.end();
	m_vertex_bufferLIT_current = m_vertex_buffersLIT.end();
	Blender = 0;
	TL_pv = 0;
}

XRayUIRender::~XRayUIRender()
{
}

void XRayUIRender::CreateUIGeom()
{
}

void XRayUIRender::DestroyUIGeom()
{
	m_vertex_buffersTL.clear();
	m_vertex_buffersLIT.clear();
	m_vertex_bufferTL_current = m_vertex_buffersTL.end();
	m_vertex_bufferLIT_current = m_vertex_buffersLIT.end();
	Blender = 0;

}

void XRayUIRender::SetShader(IUIShader& shader)
{
	Blender =& static_cast<XRayUIShader&>(shader).Blender;
}

void XRayUIRender::SetAlphaRef(int aref)
{
}

void XRayUIRender::SetScissor(Irect* rect)
{
}

void XRayUIRender::GetActiveTextureResolution(Fvector2& res)
{
	BEAR_ASSERT(Blender);
	if (Blender->E[0].Textures[0])
	{
		auto size = Blender->E[0].Textures[0]->GetSize();
		res.set(size.x,size.y) ;
	}
}

void XRayUIRender::PushPoint(float x, float y, float z, u32 C, float u, float v)
{
	BEAR_ASSERT(TL_pv);
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

void XRayUIRender::StartPrimitive(bsize iMaxVerts, ePrimitiveType primType, ePointType pointType)
{

	m_PrimitiveType = primType;
	m_PointType = pointType;
	switch (pointType)
	{
	case IUIRender::pttTL:
		TL_iMaxVerts = iMaxVerts;
		if (m_vertex_bufferTL_current == m_vertex_buffersTL.end())
		{
			BearFactoryPointer<BearRHI::BearRHIVertexBuffer> vertex_buffer = BearRenderInterface::CreateVertexBuffer();
			vertex_buffer->Create(GResourcesManager->GetStride(FVF::F_TL), iMaxVerts, true);
			m_vertex_buffersTL.push_back(vertex_buffer);
			m_vertex_bufferTL_current = --m_vertex_buffersTL.end();

		}
		else
		{
			if ((*m_vertex_bufferTL_current)->GetCount() < iMaxVerts)
			{
				(*m_vertex_bufferTL_current)->Create(GResourcesManager->GetStride(FVF::F_TL), iMaxVerts, true);
			}
		}

		TL_start_pv = (FVF::TL*)(*m_vertex_bufferTL_current)->Lock();
		BEAR_ASSERT(TL_start_pv);
		TL_pv = TL_start_pv;
		break;
	case IUIRender::pttLIT:
		LIT_iMaxVerts = iMaxVerts;
		if (m_vertex_bufferLIT_current == m_vertex_buffersLIT.end())
		{
			BearFactoryPointer<BearRHI::BearRHIVertexBuffer> vertex_buffer = BearRenderInterface::CreateVertexBuffer();
			vertex_buffer->Create(GResourcesManager->GetStride(FVF::F_TL), iMaxVerts, true);
			m_vertex_buffersLIT.push_back(vertex_buffer);
			m_vertex_bufferLIT_current = --m_vertex_buffersLIT.end();

		}
		else
		{
			if ((*m_vertex_bufferLIT_current)->GetCount() < iMaxVerts)
			{
				(*m_vertex_bufferLIT_current)->Create(GResourcesManager->GetStride(FVF::F_TL), iMaxVerts, true);
			}
		}
		LIT_start_pv =(FVF::LIT*) (*m_vertex_bufferLIT_current)->Lock();
		LIT_pv = LIT_start_pv;
		break;
	default:
		break;
	}

	
}

void XRayUIRender::FlushPrimitive()
{
	BEAR_ASSERT(Blender);
	bsize p_cnt = 0;
	switch (m_PointType)
	{
	case IUIRender::pttTL:
		p_cnt = TL_pv - TL_start_pv;
		BEAR_ASSERT(p_cnt <= TL_iMaxVerts);
		(*m_vertex_bufferTL_current)->Unlock();
		HW->Context->SetVertexBuffer((*m_vertex_bufferTL_current));
		m_vertex_bufferTL_current++;
		break;
	case IUIRender::pttLIT:
		p_cnt = LIT_pv - LIT_start_pv;
		BEAR_ASSERT(p_cnt <= LIT_iMaxVerts);
		(*m_vertex_bufferLIT_current)->Unlock();
		HW->Context->SetVertexBuffer((*m_vertex_bufferLIT_current));

		m_vertex_bufferLIT_current++;
		break;
	default:
		break;
	}
	GRenderInterface.UpdateDescriptorHeap(Blender->E[0]);
	if (!Blender->E[0].Set(HW->Context,FVF::F_TL)) {  return; }


	HW->Context->Draw(p_cnt);
	
}

void XRayUIRender::Flush()
{
	m_vertex_bufferTL_current = m_vertex_buffersTL.begin();
	m_vertex_bufferLIT_current = m_vertex_buffersLIT.begin();
}

LPCSTR XRayUIRender::UpdateShaderName(LPCSTR tex_name, LPCSTR sh_name)
{
	return LPCSTR();
}

void XRayUIRender::CacheSetXformWorld(const Fmatrix& M)
{
}

void XRayUIRender::CacheSetCullMode(CullMode)
{
}
XRayUIRender GUIRender;