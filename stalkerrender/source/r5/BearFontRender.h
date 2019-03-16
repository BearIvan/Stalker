#pragma once
class BearFontRender:public IFontRender
{
public:
	BearFontRender();

	virtual void Initialize(LPCSTR cShader, LPCSTR cTexture) ;
	virtual void OnRender(CGameFont &owner) ;
};
