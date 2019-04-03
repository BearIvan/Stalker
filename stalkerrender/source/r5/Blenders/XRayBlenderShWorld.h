#pragma once
class  XRayBlenderShWorld :public XRayBlender
{
public:
	XRayBlenderShWorld();
	~XRayBlenderShWorld();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
};