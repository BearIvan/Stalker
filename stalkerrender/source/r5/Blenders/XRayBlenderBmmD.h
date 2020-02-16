#pragma once
class  XRayBlenderBmmD :public XRayBlenderCompiler
{

public:
	XRayBlenderBmmD();
	virtual ~XRayBlenderBmmD();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Initialize();
	virtual void Compile(XRayShaderElement& shader);
protected:
	BearString64	oT2_Name;	// name of secondary texture
	BearString64	oT2_xform;	// xform for secondary texture
	BearString64	oR_Name;	//. задел на будущее
	BearString64	oG_Name;	//. задел на будущее
	BearString64	oB_Name;	//. задел на будущее
	BearString64	oA_Name;	//. задел на будущее
};