#pragma once
class  XRayBlenderModel :public XRayBlenderCompiler
{
public:
	XRayBlenderModel();
	virtual ~XRayBlenderModel();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);

	virtual void Initialize();
	virtual void Compile(XRayShaderElement& shader);
protected:
	xrP_Integer	oAREF;
	xrP_BOOL	oBlend;
	xrP_TOKEN	oTessellation;
};