#pragma once
class BearEnvDescriptorRender :public IEnvDescriptorRender
{
public:
	BearEnvDescriptorRender();
	virtual void Copy(IEnvDescriptorRender &_in);

	virtual void OnDeviceCreate(EnvDescriptorRef *owner);
	virtual void OnDeviceDestroy();
};
