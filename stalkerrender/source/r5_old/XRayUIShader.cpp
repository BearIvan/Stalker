#include "pch.h"

XRayUIShader::XRayUIShader()
{
}

void XRayUIShader::Copy(IUIShader & _in)
{
	shader = dynamic_cast<XRayUIShader*>(&_in)->shader;
}

void XRayUIShader::create(LPCSTR sh, LPCSTR tex)
{
	GXRayRenderResource->CreateShader(sh, shader);
	if(tex)
	shader.SetTextureToPixel(0, tex);
}

bool XRayUIShader::inited()
{
	return true;
}

void XRayUIShader::destroy()
{
	shader.~XRayShader();
}
