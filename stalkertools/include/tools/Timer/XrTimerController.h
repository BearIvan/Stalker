#pragma once
class XrTimerPaused;
class XRCORE_API XrTimerController 
{
	BEAR_CLASS_STATIC(XrTimerController);
#ifdef STALKERTOOLS_EXPORTS
public:
#else
private:
#endif
	static void Register(XrTimerPaused*);
	static void UnRegister(XrTimerPaused*);
	static void Initialize();
	static void Destroy();
private:
	static BearVector< XrTimerPaused*>* m_timers;
	static bool m_pause;
public:
	static void Pause(bool);
	IC static bool Paused()
	{
		return m_pause;
	}
};