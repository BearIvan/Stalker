#pragma once
class BearLensFlareRender:public ILensFlareRender
{
public:
	BearLensFlareRender();
	virtual void Copy(ILensFlareRender &_in);

	virtual void Render(CLensFlare &owner, BOOL bSun, BOOL bFlares, BOOL bGradient) ;
	virtual void OnDeviceCreate() ;
	virtual void OnDeviceDestroy();
};