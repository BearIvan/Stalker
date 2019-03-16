#include "pch.h"

BearEnvironmentRender::BearEnvironmentRender()
{
}

void BearEnvironmentRender::Copy(IEnvironmentRender & _in)
{
}

void BearEnvironmentRender::OnFrame(EnvironmentRef * env)
{
}

void BearEnvironmentRender::OnLoad()
{
}

void BearEnvironmentRender::OnUnload()
{
}

void BearEnvironmentRender::RenderSky(EnvironmentRef * env)
{
}

void BearEnvironmentRender::RenderClouds(EnvironmentRef * env)
{
}

void BearEnvironmentRender::OnDeviceCreate()
{
}

void BearEnvironmentRender::OnDeviceDestroy()
{
}
particles_systems::library_interface *null = 0;
particles_systems::library_interface const & BearEnvironmentRender::particles_systems_library()
{
	return *null;
}
