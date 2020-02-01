#pragma once
class XRayFontRender:public IFontRender
{
public:
	XRayFontRender();
	~XRayFontRender();
	virtual void Initialize(LPCSTR cShader, LPCSTR cTexture) ;
	virtual void OnRender(CGameFont &owner) ;
	void Flush();
private:
	BearVector < BearFactoryPointer<BearRHI::BearRHIVertexBuffer>> m_vertex_buffers;
	BearVector < BearFactoryPointer<BearRHI::BearRHIVertexBuffer>>::iterator m_vertex_buffer_current;

	XRayBlender Blender;
};
