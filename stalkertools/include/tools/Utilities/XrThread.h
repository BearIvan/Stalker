#pragma once
class  XrThread
{
	typedef void ThreadFunction(void*);
public:
	XRCORE_API static void Spawn(const bchar*name , ThreadFunction function,void*args);
#ifndef XRCORE_EXPORTS
private:
#endif
	static void Initialize();
	static void Destroy();
};