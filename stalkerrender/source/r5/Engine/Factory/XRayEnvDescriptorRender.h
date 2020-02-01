#pragma once
class XRayEnvDescriptorRender :public IEnvDescriptorRender
{
public:
	XRayEnvDescriptorRender();
	virtual void Copy(IEnvDescriptorRender &_in);

	virtual void OnDeviceCreate(EnvDescriptorRef *owner);
	virtual void OnDeviceDestroy();
};
