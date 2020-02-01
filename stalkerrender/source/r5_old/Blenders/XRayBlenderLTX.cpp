#include "pch.h"

XRayBlenderLTX::XRayBlenderLTX(const bchar * name)
{
	BearINI ini;
	ini.LoadFromStream(**FS.Read(TEXT("%cur_shaders%"), name, TEXT(".ltx")),BearEncoding::UTF8);
	m_shader_pixel = ini.Key(TEXT("blender"), TEXT("pixel"))[0];
	m_shader_vertex = ini.Key(TEXT("blender"), TEXT("vertex"))[0];
	BearString16 TypeState;
	BearString::Copy(TypeState, *ini.Key(TEXT("blender"), TEXT("state"))[0]);

	if (BearString::Compare(TypeState, TEXT("F_TL"))==0)
	{
		m_vertex_state = FVF::F_TL;
	}
	else if (BearString::Compare(TypeState, TEXT("F_LIT")) == 0)
	{
		m_vertex_state = FVF::F_LIT;
	}
	else if (BearString::Compare(TypeState, TEXT("F_L")) == 0)
	{
		m_vertex_state = FVF::F_L;
	}
	else if (BearString::Compare(TypeState, TEXT("F_V")) == 0)
	{
		m_vertex_state = FVF::F_V;
	}
	else if (BearString::Compare(TypeState, TEXT("F_TL0uv")) == 0)
	{
		m_vertex_state = FVF::F_TL0uv;
	}
	else if (BearString::Compare(TypeState, TEXT("F_TL2uv")) == 0)
	{
		m_vertex_state = FVF::F_TL2uv;
	}
	else if (BearString::Compare(TypeState, TEXT("F_TL4uv")) == 0)
	{
		m_vertex_state = FVF::F_TL4uv;
	}
	else
	{
		BEAR_FATALERROR(false, TEXT("Неизветный VertexState[%s]"), TypeState);
	}
	BearString16 TransformType;
	BearString::Copy(TransformType, *ini.Key(TEXT("blender"), TEXT("transform_type"))[0]);
	if (BearString::Compare(TransformType, TEXT("TT_Screen")) == 0)
	{
		m_transform_type = XRayShader::TT_Screen;
	}
	else if (BearString::Compare(TransformType, TEXT("TT_Matrix")) == 0)
	{
		m_transform_type = XRayShader::TT_Matrix;
	}
	else
	{
		BEAR_FATALERROR(false, TEXT("Неизветный TransformType[%s]"), TransformType);
	}
}

XRayBlenderLTX::~XRayBlenderLTX()
{
}

LPCSTR XRayBlenderLTX::getComment()
{
	return LPCSTR();
}

void XRayBlenderLTX::Save(IWriter & fs)
{
}

void XRayBlenderLTX::Load(IReader & fs, u16 version)
{
}

void XRayBlenderLTX::Destroy()
{
	bear_delete(this);
}

void XRayBlenderLTX::Compile(XRayShader & shader)
{
	shader.SetVertexState(m_vertex_state);
	shader.SetShader(*m_shader_pixel, *m_shader_vertex);
	shader.SetTransformType(m_transform_type);
}
