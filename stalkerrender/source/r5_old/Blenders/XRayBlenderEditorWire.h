#pragma once
class  XRayBlenderEditorWire :public XRayBlender
{
public:
	XRayBlenderEditorWire();
	~XRayBlenderEditorWire();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
protected:
	string64	oT_Factor;
};