#include "pch.h"

XRayUIShader::XRayUIShader()
{
}

void XRayUIShader::Copy(IUIShader & _in)
{
	Blender = static_cast<XRayUIShader&>(_in).Blender;

}

void XRayUIShader::create(LPCSTR sh, LPCSTR tex)
{
	Blender.Clear();
	GResourcesManager->CompileBlender(Blender, sh, tex);
}

bool XRayUIShader::inited()
{
	return true;
}

void XRayUIShader::destroy()
{
	Blender.Clear();
}
