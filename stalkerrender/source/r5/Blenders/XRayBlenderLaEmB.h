#pragma once
class  XRayBlenderLaEmB :public XRayBlender
{
public:
	XRayBlenderLaEmB();
	~XRayBlenderLaEmB();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
protected:
	BearString64	oT2_Name;		// name of secondary texture
	BearString64	oT2_xform;		// xform for secondary texture
	BearString64	oT2_const;
};