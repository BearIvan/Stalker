#pragma once
class  XRayBlenderVertex :public XRayBlender
{
public:
	XRayBlenderVertex();
	~XRayBlenderVertex();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
protected:
	xrP_TOKEN	oTessellation;
};