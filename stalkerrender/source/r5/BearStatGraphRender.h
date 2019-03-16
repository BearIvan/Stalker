#pragma once

class BearStatGraphRender:public IStatGraphRender
{
public:
	BearStatGraphRender();
	virtual void Copy(IStatGraphRender &_in) ;

	virtual void OnDeviceCreate() ;
	virtual void OnDeviceDestroy() ;

	virtual void OnRender(CStatGraph &owner) ;
};
