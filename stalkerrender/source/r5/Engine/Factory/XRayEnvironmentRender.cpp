#include "pch.h"

XRayEnvironmentRender::XRayEnvironmentRender()
{
}

void XRayEnvironmentRender::Copy(IEnvironmentRender & _in)
{
}

void XRayEnvironmentRender::OnFrame(EnvironmentRef * env)
{
}

void XRayEnvironmentRender::OnLoad()
{
}

void XRayEnvironmentRender::OnUnload()
{
}

void XRayEnvironmentRender::RenderSky(EnvironmentRef * env)
{
}

void XRayEnvironmentRender::RenderClouds(EnvironmentRef * env)
{
}

void XRayEnvironmentRender::OnDeviceCreate()
{
}

void XRayEnvironmentRender::OnDeviceDestroy()
{
}
particles_systems::library_interface *null = 0;
particles_systems::library_interface const & XRayEnvironmentRender::particles_systems_library()
{
	return *null;
}
