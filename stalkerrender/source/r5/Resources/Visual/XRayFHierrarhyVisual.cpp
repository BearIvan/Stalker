#include "pch.h"
#include "engine/Fmesh.h"
void XRayFHierrarhyVisual::Load(const char* N, IReader* data, u32 dwFlags)
{
	XRayRenderVisual::Load(N, data, dwFlags);
	if (data->find_chunk(OGF_CHILDREN_L))
	{
		// From Link
		u32 count = data->r_u32();
		children.resize(count);
		for (u32 i = 0; i < count; i++) {

			u32 ID = data->r_u32();
			children[i] = GRenderInterface.GetVisual(ID);
		}
		m_DontDelete = TRUE;
	}
	else
	{
		if (data->find_chunk(OGF_CHILDREN))
		{
			// From stream
			IReader* OBJ = data->open_chunk(OGF_CHILDREN);
			if (OBJ) {
				IReader* O = OBJ->open_chunk(0);
				for (int count = 1; O; count++) {
					string_path			name_load, short_name, num;
					xr_strcpy(short_name, N);
					if (strext(short_name)) *strext(short_name) = 0;
					BearString32 Num;
					BearString::Printf(Num, "%d", num);
					strconcat(sizeof(name_load), name_load, short_name, ":", Num);
					children.push_back((XRayRenderVisual*)GRenderInterface.model_CreateChild(name_load, O));
					O->close();
					O = OBJ->open_chunk(count);
				}
				OBJ->close();
			}
			m_DontDelete = FALSE;
		}
		else
		{
			FATAL("Invalid visual");
		}
	}
}

void XRayFHierrarhyVisual::Copy(XRayRenderVisual* from)
{
	XRayRenderVisual::Copy(from);

	XRayFHierrarhyVisual* pFrom = (XRayFHierrarhyVisual*)from;

	children.clear();
	children.reserve(pFrom->children.size());
	for (u32 i = 0; i < pFrom->children.size(); i++) {
		XRayRenderVisual* p = (XRayRenderVisual*) ::Render->model_Duplicate(pFrom->children[i]);
		children.push_back(p);
	}
	m_DontDelete = FALSE;
}

XRayFHierrarhyVisual::XRayFHierrarhyVisual()
{
}

XRayFHierrarhyVisual::~XRayFHierrarhyVisual()
{
	if (!m_DontDelete) {
		for (u32 i = 0; i < children.size(); i++)
			::Render->model_Delete((IRenderVisual*&)children[i]);
	}
	children.clear();
}
