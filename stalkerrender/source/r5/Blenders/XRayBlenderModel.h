#pragma once
class  XRayBlenderModel :public XRayBlender
{
public:
	XRayBlenderModel();
	~XRayBlenderModel();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
protected:
	xrP_Integer	oAREF;
	xrP_BOOL	oBlend;
	xrP_TOKEN	oTessellation;
};