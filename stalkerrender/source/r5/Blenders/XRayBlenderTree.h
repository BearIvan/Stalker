#pragma once
class  XRayBlenderTree :public XRayBlenderCompiler
{
public:
	XRayBlenderTree();
	virtual 	~XRayBlenderTree();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
protected:
	xrP_BOOL	oBlend;
	xrP_BOOL	oNotAnTree;
};