#include "pch.h"

XRayUIRender::XRayUIRender()
{
	m_vertex_bufferTL0uv_current = m_vertex_buffersTL0uv.end();
	m_vertex_bufferTL_current = m_vertex_buffersTL.end();
	m_vertex_bufferLIT_current = m_vertex_buffersLIT.end();
	Blender = 0;
	TL_pv = 0;
	m_Scissor = 0;
}

XRayUIRender::~XRayUIRender()
{
}

void XRayUIRender::CreateUIGeom()
{
}

void XRayUIRender::DestroyUIGeom()
{
	m_vertex_buffersTL0uv.clear();
	m_vertex_buffersTL.clear();
	m_vertex_buffersLIT.clear();
	m_vertex_bufferTL0uv_current = m_vertex_buffersTL0uv.end();
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

	if (rect)HW->Context->SetScissor(true, rect->x1, rect->y1, rect->x2, rect->y2);
	else HW->Context->SetScissor(false, 0, 0, 0, 0);
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
		switch (m_PrimitiveType)
		{
		case IUIRender::ptTriList:
		case IUIRender::ptTriStrip:

			TL_pv->set(x, y, C, u, v);
			++TL_pv;
			break;
		case IUIRender::ptLineStrip:
		case IUIRender::ptLineList:

			TL0uv_pv->set(x, y, C);
			++TL0uv_pv;
			break;
		}
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

		switch (m_PrimitiveType)
		{
		case IUIRender::ptTriList:
		case IUIRender::ptTriStrip:



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
		case IUIRender::ptLineStrip:
		case IUIRender::ptLineList:



			if (m_vertex_bufferTL0uv_current == m_vertex_buffersTL0uv.end())
			{
				BearFactoryPointer<BearRHI::BearRHIVertexBuffer> vertex_buffer = BearRenderInterface::CreateVertexBuffer();
				vertex_buffer->Create(GResourcesManager->GetStride(FVF::F_TL0uv), iMaxVerts, true);
				m_vertex_buffersTL0uv.push_back(vertex_buffer);
				m_vertex_bufferTL0uv_current = --m_vertex_buffersTL0uv.end();

			}
			else
			{
				if ((*m_vertex_bufferTL0uv_current)->GetCount() < iMaxVerts)
				{
					(*m_vertex_bufferTL0uv_current)->Create(GResourcesManager->GetStride(FVF::F_TL0uv), iMaxVerts, true);
				}
			}

			TL0uv_start_pv = (FVF::TL0uv*)(*m_vertex_bufferTL0uv_current)->Lock();
			BEAR_ASSERT(TL0uv_start_pv);

			TL0uv_pv = TL0uv_start_pv;
			break;
		}
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
		switch (m_PrimitiveType)
		{
		case IUIRender::ptTriList:
		case IUIRender::ptTriStrip:


			p_cnt = TL_pv - TL_start_pv;
			BEAR_ASSERT(p_cnt <= TL_iMaxVerts);
			(*m_vertex_bufferTL_current)->Unlock();
			if (p_cnt == 0)return;
			HW->Context->SetVertexBuffer((*m_vertex_bufferTL_current));
			m_vertex_bufferTL_current++;
			break;
		case IUIRender::ptLineStrip:
		case IUIRender::ptLineList:


			p_cnt = TL0uv_pv - TL0uv_start_pv;
			BEAR_ASSERT(p_cnt <= TL_iMaxVerts);
			(*m_vertex_bufferTL0uv_current)->Unlock();
			if (p_cnt == 0)return;
			HW->Context->SetVertexBuffer((*m_vertex_bufferTL0uv_current));
			m_vertex_bufferTL0uv_current++;
			break;
		}
		
		break;
	case IUIRender::pttLIT:
		p_cnt = LIT_pv - LIT_start_pv;
		BEAR_ASSERT(p_cnt <= LIT_iMaxVerts);
		(*m_vertex_bufferLIT_current)->Unlock();
		if (p_cnt == 0)return;
		HW->Context->SetVertexBuffer((*m_vertex_bufferLIT_current));

		m_vertex_bufferLIT_current++;
		break;
	default:
		break;
	}
	switch (m_PrimitiveType)
	{
	case IUIRender::ptNone:
		return;
		break;
	case IUIRender::ptTriList:

		GRenderInterface.UpdateDescriptorHeap(Blender->E[0]);
		if (!Blender->E[0].Set(HW->Context, FVF::F_TL)) { BEAR_ASSERT(Blender->DebugName.c_str()==0); return; }
		break;
	case IUIRender::ptTriStrip:

		GRenderInterface.UpdateDescriptorHeap(Blender->E[1]);
		if (!Blender->E[1].Set(HW->Context, FVF::F_TL)) { BEAR_ASSERT(Blender->DebugName.c_str() == 0); return; }
		break;
	case IUIRender::ptLineStrip:
		GRenderInterface.UpdateDescriptorHeap(Blender->E[3]);
		if (!Blender->E[3].Set(HW->Context, FVF::F_TL0uv)) { BEAR_ASSERT(Blender->DebugName.c_str() == 0); return; }
		break;
	case IUIRender::ptLineList:
		GRenderInterface.UpdateDescriptorHeap(Blender->E[2]);
		if (!Blender->E[2].Set(HW->Context, FVF::F_TL0uv)) { BEAR_ASSERT(Blender->DebugName.c_str() == 0); return; }
		break;
	default:
		break;
	}

	
	HW->Context->Draw(p_cnt);

	
}

void XRayUIRender::Flush()
{

	m_vertex_bufferTL0uv_current = m_vertex_buffersTL0uv.begin();
	m_vertex_bufferTL_current = m_vertex_buffersTL.begin();
	m_vertex_bufferLIT_current = m_vertex_buffersLIT.begin();
	m_Scissor = 0;
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