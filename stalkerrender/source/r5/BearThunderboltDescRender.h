#pragma once

class BearThunderboltDescRender:public IThunderboltDescRender
{
public:
	BearThunderboltDescRender();
	virtual void Copy(IThunderboltDescRender &_in) ;

	virtual void CreateModel(LPCSTR m_name) ;
	virtual void DestroyModel() ;
};
