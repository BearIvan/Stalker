#pragma once
class  XRayBlenderEditorSelection :public XRayBlender
{
public:
	XRayBlenderEditorSelection();
	~XRayBlenderEditorSelection();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
protected:
	BearString64	oT_Factor;
};