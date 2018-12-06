#pragma once
class EnvironmentRef
{
public:
	virtual ~EnvironmentRef() {}
	virtual  void load()=0;
	virtual void unload()=0;
	virtual void OnFrame()=0;
	virtual void					mods_load() = 0;
};
class EnvDescriptorRef
{
public:

	virtual  const char* getEvnSkyName()=0;
	virtual  const char* getSkyName() = 0;
	virtual  const char* getCloudsName() = 0;
};