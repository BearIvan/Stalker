#pragma once/*
using XrTimer = BearTimer;
using XrTimerPause = BearTimer;*/
class XRCORE_API  XrTimerStat :private  BearTimer
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
		return m_R.asseconds();
	}
	IC bsize GetCount()const
	{
		return m_count;
	}

private:
	bsize m_count;
	BearTime m_R;
};
class XRCORE_API XrTimerPaused : public  BearTimer
{
public:
	XrTimerPaused();
	~XrTimerPaused(); 

};