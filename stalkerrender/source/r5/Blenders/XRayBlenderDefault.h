#pragma once
class  XRayBlenderDefaultAref :public XRayBlender
{
public:
	XRayBlenderDefaultAref();
	~XRayBlenderDefaultAref();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
protected:
	xrP_Integer	oAREF;
	xrP_BOOL	oBlend;
};