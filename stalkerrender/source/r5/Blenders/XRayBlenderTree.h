#pragma once
class  XRayBlenderTree :public XRayBlender
{
public:
	XRayBlenderTree();
	~XRayBlenderTree();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
protected:
	xrP_BOOL	oBlend;
	xrP_BOOL	oNotAnTree;
};