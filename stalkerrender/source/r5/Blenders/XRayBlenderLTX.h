#pragma once
class  XRayBlenderLTX :public XRayBlender
{
public:
	XRayBlenderLTX(const bchar*name);
	~XRayBlenderLTX();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
	virtual void Compile(XRayShader&shader);
private:
	BearString m_shader_pixel;
	BearString m_shader_vertex;
	uint32 m_vertex_state;
	XRayShader::TransformType m_transform_type;
};