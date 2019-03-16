#pragma once
class BearEnvDescriptorMixerRender:public  IEnvDescriptorMixerRender
{
public:
	BearEnvDescriptorMixerRender();
	virtual void Copy(IEnvDescriptorMixerRender &_in);

	virtual void Destroy();
	virtual void Clear();
	virtual void lerp(IEnvDescriptorRender *inA, IEnvDescriptorRender *inB);
};
