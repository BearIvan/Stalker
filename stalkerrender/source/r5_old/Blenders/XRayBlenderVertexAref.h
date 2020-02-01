#pragma once
class  XRayBlenderVertexAref :public XRayBlender
{
public:
	XRayBlenderVertexAref();
	~XRayBlenderVertexAref();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
protected:
	xrP_Integer	oAREF;
	xrP_BOOL	oBlend;
};