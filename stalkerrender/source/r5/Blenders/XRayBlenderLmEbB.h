#pragma once
class  XRayBlenderLmEbB :public XRayBlenderCompiler
{
public:
	XRayBlenderLmEbB();
	virtual ~XRayBlenderLmEbB();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Initialize();
	virtual void Compile(XRayShaderElement& shader);
protected:
	BearString64	oT2_Name;		// name of secondary texture
	BearString64	oT2_xform;		// xform for secondary texture
	xrP_BOOL	oBlend;
};