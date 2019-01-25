#include "stdafx.h"
#include "dxThunderboltDescRender.h"

void dxThunderboltDescRender::Copy(IThunderboltDescRender&_in)
{
	*this = *((dxThunderboltDescRender*)&_in);
}

void dxThunderboltDescRender::CreateModel(LPCSTR m_name)
{
	IReader* F			= 0;
	F					=XRayBearReader::Create( FS.Read("%meshes%",m_name));
	l_model				= ::RImplementation.model_CreateDM(F);
	XRayBearReader::Destroy		(F);
}

void dxThunderboltDescRender::DestroyModel()
{
	::RImplementation.model_Delete(l_model);
}