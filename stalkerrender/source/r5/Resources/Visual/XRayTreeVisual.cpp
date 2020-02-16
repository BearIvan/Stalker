#include "pch.h"

void XRayTreeVisual::Render(float LOD)
{
}

void XRayTreeVisual::Load(LPCSTR N, IReader* data, u32 dwFlags)
{
	R_ASSERT(data->find_chunk(OGF_GCONTAINER));
	{
		// verts
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

	// load tree-def
	R_ASSERT(data->find_chunk(OGF_TREEDEF2));
	{
		data->r(&xform, sizeof(xform));
		data->r(&c_scale, sizeof(c_scale));	c_scale.rgb.mul(.5f);	c_scale.hemi *= .5f;	c_scale.sun *= .5f;
		data->r(&c_bias, sizeof(c_bias));	c_bias.rgb.mul(.5f);	c_bias.hemi *= .5f;	c_bias.sun *= .5f;
		//Msg				("hemi[%f / %f], sun[%f / %f]",c_scale.hemi,c_bias.hemi,c_scale.sun,c_bias.sun);
	}

}
#define PCOPY(a)	a = pFrom->a
void XRayTreeVisual::Copy(XRayRenderVisual* from)
{
	XRayRenderVisual::Copy(from);

	XRayTreeVisual* pFrom = dynamic_cast<XRayTreeVisual*> (from);
	PCOPY(xform);
	PCOPY(c_scale);
	PCOPY(c_bias);
	PCOPY(FVF);

	PCOPY(VertexBuffer);
	PCOPY(OffsetVertex);
	PCOPY(CountVertex);

	PCOPY(IndexBuffer);
	PCOPY(OffsetIndex);
	PCOPY(CountIndex);
}


XRayTreeVisual::XRayTreeVisual(void)
{
}

XRayTreeVisual::~XRayTreeVisual(void)
{
}

XRayTreeVisual_ST::XRayTreeVisual_ST(void)
{
}

XRayTreeVisual_ST::~XRayTreeVisual_ST(void)
{
}

void XRayTreeVisual_ST::Render(float LOD)
{
}

void XRayTreeVisual_ST::Load(LPCSTR N, IReader* data, u32 dwFlags)
{
	inherited::Load(N, data, dwFlags);

}

void XRayTreeVisual_ST::Copy(XRayRenderVisual* pFrom)
{
	inherited::Copy(pFrom);
}

XRayTreeVisual_PM::XRayTreeVisual_PM(void)
{
}

XRayTreeVisual_PM::~XRayTreeVisual_PM(void)
{
}

void XRayTreeVisual_PM::Render(float LOD)
{
}

void XRayTreeVisual_PM::Load(LPCSTR N, IReader* data, u32 dwFlags)
{
	inherited::Load(N, data, dwFlags);
	R_ASSERT(data->find_chunk(OGF_SWICONTAINER));
	{
		u32 ID = data->r_u32();
		pSWI = GRenderInterface.GetSWI(ID);
	}
}

void XRayTreeVisual_PM::Copy(XRayRenderVisual* from)
{
	inherited::Copy(from);
	XRayTreeVisual_PM* pFrom = dynamic_cast<XRayTreeVisual_PM*> (from);
	PCOPY(pSWI);
}
