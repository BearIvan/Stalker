#pragma once
class  XRayBlenderModelEbB :public XRayBlender
{
public:
	XRayBlenderModelEbB();
	~XRayBlenderModelEbB();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
protected:
	string64	oT2_Name;		// name of secondary texture
	string64	oT2_xform;		// xform for secondary texture
	xrP_BOOL	oBlend;
};