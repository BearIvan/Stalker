#pragma once
class  XRayBlenderBlur :public XRayBlender
{
public:
	XRayBlenderBlur();
	~XRayBlenderBlur();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
};