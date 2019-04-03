#pragma once
class  XRayBlenderDetailStill :public XRayBlender
{
public:
	XRayBlenderDetailStill();
	~XRayBlenderDetailStill();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
protected:
	xrP_BOOL	oBlend;
};