#pragma once
class  XRayBlenderScreenGray :public XRayBlender
{
public:
	XRayBlenderScreenGray();
	~XRayBlenderScreenGray();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
};