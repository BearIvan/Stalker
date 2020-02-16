#pragma once
class  XRayBlenderVertexAref :public XRayBlenderCompiler
{
public:
	XRayBlenderVertexAref();
	virtual	~XRayBlenderVertexAref();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Initialize();
	virtual void Compile(XRayShaderElement& shader);
protected:
	xrP_Integer	oAREF;
	xrP_BOOL	oBlend;
};