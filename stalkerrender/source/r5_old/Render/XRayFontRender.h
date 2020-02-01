#pragma once
class XRayFontRender:public IFontRender
{
public:
	XRayFontRender();
	~XRayFontRender();
	virtual void Initialize(LPCSTR cShader, LPCSTR cTexture) ;
	virtual void OnRender(CGameFont &owner) ;
private:
	XRayShader m_shader;
	bsize m_index_count;
	bsize m_vertex_count;
	BearGraphics::BearVertexBufferRef m_vertex_buffer;
	BearGraphics::BearIndexBufferRef m_index_buffer;
};
