#include "pch.h"
#include "engine/Fmesh.h"
#define VLOAD_NOVERTICES 1<<0
XRayFVisual::XRayFVisual()
{
	CountIndex = 0;
	OffsetIndex = 0;
	CountVertex = 0;
	OffsetVertex = 0;
	FVF = 0;
}

XRayFVisual::~XRayFVisual()
{
}

void XRayFVisual::Load(const char* N, IReader* data, u32 dwFlags)
{
	XRayRenderVisual::Load(N, data, dwFlags);

	bool				Loaded = false;

	if (data->find_chunk(OGF_GCONTAINER))
	{
		Loaded = true;
		u32 ID = data->r_u32();
		OffsetVertex = data->r_u32();
		CountVertex = data->r_u32();

		BEAR_ASSERT(VertexBuffer.empty());

		VertexBuffer = GRenderInterface.GetVertexBuffer(ID);
		FVF = GRenderInterface.GetVertexState(ID);


		ID = data->r_u32();
		OffsetIndex = data->r_u32();
		CountIndex = data->r_u32();

		BEAR_ASSERT(IndexBuffer.empty());

		IndexBuffer = GRenderInterface.GetIndexBuffer(ID);
	}

	if (!Loaded && (dwFlags & VLOAD_NOVERTICES) == 0)
	{
		if (data->find_chunk(OGF_VCONTAINER))
		{
			R_ASSERT2(0, "pls notify andy about this.");
		}
		else
		{
			R_ASSERT(data->find_chunk(OGF_VERTICES));
			FVF = data->r_u32();

			//FIXME:Сделать статичным!!!
			CountVertex = data->r_u32();
			BEAR_ASSERT(VertexBuffer.empty());
			VertexBuffer = BearRenderInterface::CreateVertexBuffer();
			VertexBuffer->Create(GResourcesManager->GetStride(FVF), CountVertex, true);
			bear_copy(VertexBuffer->Lock(), data->pointer(), CountVertex * GResourcesManager->GetStride(FVF));
			VertexBuffer->Unlock();
		}
	}

	// indices
	if (!Loaded)
	{
		if (data->find_chunk(OGF_ICONTAINER))
		{
			R_ASSERT2(0, "pls notify andy about this.");
		}
		else
		{
			R_ASSERT(data->find_chunk(OGF_INDICES));
			CountIndex = data->r_u32();
			//FIXME:Сделать статичным!!!
			BEAR_ASSERT(IndexBuffer.empty());
			IndexBuffer = BearRenderInterface::CreateIndexBuffer();
			IndexBuffer->Create(CountIndex, true);
			uint32* dst = IndexBuffer->Lock();
			u16* src = (u16*)data->pointer();
			for (bsize i = 0; i < CountIndex; i++)
			{
				dst[i] = src[i];
			}
			IndexBuffer->Unlock();
		}

	}
}
#define PCOPY(a)	a = pFrom->a
void XRayFVisual::Copy(XRayRenderVisual* from)
{
	XRayRenderVisual::Copy(from);

	XRayFVisual* pFrom = dynamic_cast<XRayFVisual*> (from);

	PCOPY(FVF);

	PCOPY(VertexBuffer);
	PCOPY(OffsetVertex);
	PCOPY(CountVertex);

	PCOPY(IndexBuffer);
	PCOPY(OffsetIndex);
	PCOPY(CountIndex);
}

void XRayFVisual::Render(float LOD)
{
	GRenderInterface.UpdateDescriptorHeap(Shader.E[0]);
	if (!Shader.E[0].Set(HW->Context,FVF)) { return; }
	HW->Context->SetVertexBuffer(VertexBuffer);
	HW->Context->SetIndexBuffer(IndexBuffer);
	HW->Context->DrawIndex(CountIndex, OffsetIndex, OffsetVertex);
}
