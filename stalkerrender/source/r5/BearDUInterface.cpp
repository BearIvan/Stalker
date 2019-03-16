#include "pch.h"

BearDUInterface::BearDUInterface()
{
}

void __stdcall BearDUInterface::DrawCross(const Fvector & p, float szx1, float szy1, float szz1, float szx2, float szy2, float szz2, u32 clr, BOOL bRot45)
{
	
}

void __stdcall BearDUInterface::DrawCross(const Fvector & p, float sz, u32 clr, BOOL bRot45)
{
	
}

void __stdcall BearDUInterface::DrawFlag(const Fvector & p, float heading, float height, float sz, float sz_fl, u32 clr, BOOL bDrawEntity)
{
	
}

void __stdcall BearDUInterface::DrawRomboid(const Fvector & p, float radius, u32 clr)
{
	
}

void __stdcall BearDUInterface::DrawJoint(const Fvector & p, float radius, u32 clr)
{
	
}

void __stdcall BearDUInterface::DrawSpotLight(const Fvector & p, const Fvector & d, float range, float phi, u32 clr)
{
	
}

void __stdcall BearDUInterface::DrawDirectionalLight(const Fvector & p, const Fvector & d, float radius, float range, u32 clr)
{
	
}

void __stdcall BearDUInterface::DrawPointLight(const Fvector & p, float radius, u32 clr)
{
	
}

void __stdcall BearDUInterface::DrawSound(const Fvector & p, float radius, u32 clr)
{
	
}

void __stdcall BearDUInterface::DrawLineSphere(const Fvector & p, float radius, u32 clr, BOOL bCross)
{
	
}

void __stdcall BearDUInterface::dbgDrawPlacement(const Fvector & p, int sz, u32 clr, LPCSTR caption, u32 clr_font)
{
	
}

void __stdcall BearDUInterface::dbgDrawVert(const Fvector & p0, u32 clr, LPCSTR caption)
{
	
}

void __stdcall BearDUInterface::dbgDrawEdge(const Fvector & p0, const Fvector & p1, u32 clr, LPCSTR caption)
{
	
}

void __stdcall BearDUInterface::dbgDrawFace(const Fvector & p0, const Fvector & p1, const Fvector & p2, u32 clr, LPCSTR caption)
{
	
}

void __stdcall BearDUInterface::DrawFace(const Fvector & p0, const Fvector & p1, const Fvector & p2, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall BearDUInterface::DrawLine(const Fvector & p0, const Fvector & p1, u32 clr)
{
	
}

void __stdcall BearDUInterface::DrawLink(const Fvector & p0, const Fvector & p1, float sz, u32 clr)
{
	
}

void __stdcall BearDUInterface::DrawFaceNormal(const Fvector & p0, const Fvector & p1, const Fvector & p2, float size, u32 clr)
{
	
}

void __stdcall BearDUInterface::DrawFaceNormal(const Fvector * p, float size, u32 clr)
{
	
}

void __stdcall BearDUInterface::DrawFaceNormal(const Fvector & C, const Fvector & N, float size, u32 clr)
{
	
}

void __stdcall BearDUInterface::DrawSelectionBox(const Fvector & center, const Fvector & size, u32 * c)
{
	
}

void __stdcall BearDUInterface::DrawSelectionBoxB(const Fbox & box, u32 * c)
{
	
}

void __stdcall BearDUInterface::DrawIdentSphere(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w)
{
	
}

void __stdcall BearDUInterface::DrawIdentSpherePart(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w)
{
	
}

void __stdcall BearDUInterface::DrawIdentCone(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w)
{
	
}

void __stdcall BearDUInterface::DrawIdentCylinder(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w)
{
	
}

void __stdcall BearDUInterface::DrawIdentBox(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w)
{
	
}

void __stdcall BearDUInterface::DrawBox(const Fvector & offs, const Fvector & Size, BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w)
{
	
}

void __stdcall BearDUInterface::DrawAABB(const Fvector & p0, const Fvector & p1, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall BearDUInterface::DrawAABB(const Fmatrix & parent, const Fvector & center, const Fvector & size, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall BearDUInterface::DrawOBB(const Fmatrix & parent, const Fobb & box, u32 clr_s, u32 clr_w)
{
	
}

void __stdcall BearDUInterface::DrawSphere(const Fmatrix & parent, const Fvector & center, float radius, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall BearDUInterface::DrawSphere(const Fmatrix & parent, const Fsphere & S, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall BearDUInterface::DrawCylinder(const Fmatrix & parent, const Fvector & center, const Fvector & dir, float height, float radius, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall BearDUInterface::DrawCone(const Fmatrix & parent, const Fvector & apex, const Fvector & dir, float height, float radius, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall BearDUInterface::DrawPlane(const Fvector & center, const Fvector2 & scale, const Fvector & rotate, u32 clr_s, u32 clr_w, BOOL bCull, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall BearDUInterface::DrawPlane(const Fvector & p, const Fvector & n, const Fvector2 & scale, u32 clr_s, u32 clr_w, BOOL bCull, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall BearDUInterface::DrawRectangle(const Fvector & o, const Fvector & u, const Fvector & v, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall BearDUInterface::DrawGrid()
{
	
}

void __stdcall BearDUInterface::DrawPivot(const Fvector & pos, float sz)
{
	
}

void __stdcall BearDUInterface::DrawAxis(const Fmatrix & T)
{
	
}

void __stdcall BearDUInterface::DrawObjectAxis(const Fmatrix & T, float sz, BOOL sel)
{
	
}

void __stdcall BearDUInterface::DrawSelectionRect(const Ivector2 & m_SelStart, const Ivector2 & m_SelEnd)
{
	
}

void __stdcall BearDUInterface::DrawIndexedPrimitive(int prim_type, u32 pc, const Fvector & pos, const Fvector * vb, const u32 & vb_size, const u32 * ib, const u32 & ib_size, const u32 & clr_argb, float scale)
{
	
}

void __stdcall BearDUInterface::OutText(const Fvector & pos, LPCSTR text, u32 color, u32 shadow_color)
{
	
}

void __stdcall BearDUInterface::OnDeviceDestroy()
{
	
}
