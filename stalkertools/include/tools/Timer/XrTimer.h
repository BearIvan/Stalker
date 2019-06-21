#pragma once/*
using XrTimer = BearCore::BearTimer;
using XrTimerPause = BearCore::BearTimer;*/
class XRCORE_API  XrTimerStat :private  BearCore::BearTimer
{
public:
	XrTimerStat();
	~XrTimerStat();
	void Begin();
	void End();
	void FrameStart(); 
	void FrameEnd();
	IC	float GetResult()const
	{
		return m_R;
	}
	IC bsize GetCount()const
	{
		return m_count;
	}

private:
	bsize m_count;
	float m_R;
};
class XRCORE_API XrTimerPaused : public  BearCore::BearTimer
{
public:
	XrTimerPaused();
	~XrTimerPaused(); 

};