#pragma once
class  XRayBlenderDefault :public XRayBlenderCompiler
{
public:
	XRayBlenderDefault();
	virtual ~XRayBlenderDefault();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
protected:
	xrP_TOKEN	oTessellation;
};