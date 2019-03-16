#pragma once
class BearConsoleRender:public IConsoleRender
{
public:
	BearConsoleRender();
	virtual void Copy(IConsoleRender &_in);
	virtual void OnRender(bool bGame) ;
};