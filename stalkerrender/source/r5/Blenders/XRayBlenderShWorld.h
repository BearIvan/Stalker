#pragma once
class  XRayBlenderShWorld :public XRayBlenderCompiler
{
public:
	XRayBlenderShWorld();
	virtual ~XRayBlenderShWorld();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);

	virtual void Initialize();
	virtual void Compile(XRayShaderElement& shader);
};