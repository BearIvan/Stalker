#pragma once
class XRayShaderIncluder :public BearIncluder
{
public:
	XRayShaderIncluder() {}
	virtual BearRef<BearInputStream> OpenAsStream(const bchar* name)
	{
		return FS.Read(TEXT("%cur_shaders%"), name);
	}
	virtual BearRef<BearBufferedReader> OpenAsBuffer(const bchar* name)
	{
		return BearRef<BearBufferedReader>(bear_new<BearMemoryStream>(**FS.Read(TEXT("%cur_shaders%"), name)));
	}
};
