#pragma once
class  XRayBlenderScreenGray :public XRayBlenderCompiler
{
public:
	XRayBlenderScreenGray();
	virtual 	~XRayBlenderScreenGray();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
};