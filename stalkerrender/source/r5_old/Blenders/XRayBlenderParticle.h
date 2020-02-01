#pragma once
class  XRayBlenderParticle :public XRayBlender
{
public:
	XRayBlenderParticle();
	~XRayBlenderParticle();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
protected:
	xrP_TOKEN	oBlend;
	xrP_Integer	oAREF;
	xrP_BOOL	oClamp;
};