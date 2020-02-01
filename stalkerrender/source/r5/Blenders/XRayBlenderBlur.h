#pragma once
class  XRayBlenderBlur :public XRayBlenderCompiler
{
public:
	XRayBlenderBlur();
	virtual ~XRayBlenderBlur();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
};