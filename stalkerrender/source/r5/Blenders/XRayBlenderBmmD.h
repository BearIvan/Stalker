#pragma once
class  XRayBlenderBmmD :public XRayBlenderCompiler
{

public:
	XRayBlenderBmmD();
	virtual ~XRayBlenderBmmD();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
protected:
	BearString64	oT2_Name;	// name of secondary texture
	BearString64	oT2_xform;	// xform for secondary texture
	BearString64	oR_Name;	//. ����� �� �������
	BearString64	oG_Name;	//. ����� �� �������
	BearString64	oB_Name;	//. ����� �� �������
	BearString64	oA_Name;	//. ����� �� �������
};