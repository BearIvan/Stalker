#pragma once
class  XRayBlenderDefault :public XRayBlender
{
public:
	XRayBlenderDefault();
	~XRayBlenderDefault();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
protected:
	xrP_TOKEN	oTessellation;
};