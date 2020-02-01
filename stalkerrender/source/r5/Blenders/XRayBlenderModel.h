#pragma once
class  XRayBlenderModel :public XRayBlenderCompiler
{
public:
	XRayBlenderModel();
	virtual ~XRayBlenderModel();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
protected:
	xrP_Integer	oAREF;
	xrP_BOOL	oBlend;
	xrP_TOKEN	oTessellation;
};