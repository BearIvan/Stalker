#include "stdafx.h"
BearVector< XrTimerPaused*>* XrTimerController::m_timers = 0;
bool XrTimerController::m_pause = false;
void XrTimerController::Register(XrTimerPaused *a)
{
	BEAR_ASSERT(a);
	if (m_timers == 0)Initialize();
	BEAR_ASSERT(m_timers);
	m_timers->push_back(a);
}

void XrTimerController::UnRegister(XrTimerPaused *a)
{
	BEAR_ASSERT(m_timers);
	for (auto b = m_timers->begin(),  e = m_timers->begin(); b !=e;b++)
	{
		if (*b == a)
		{
			m_timers->erase(b);
			return;
		}
	}
}

void XrTimerController::Initialize()
{
	m_timers = bear_new< BearVector< XrTimerPaused*>>();
}

void XrTimerController::Destroy()
{
	BEAR_ASSERT(m_timers);
	BEAR_ASSERT(m_timers->size()==0);
	bear_delete(m_timers);
}

void XrTimerController::Pause(bool a)
{
	if (m_pause == a)return;
	m_pause = a;
	BEAR_ASSERT(m_timers);
	for (auto b = m_timers->begin(), e = m_timers->end(); b != e; b++)
	{

		(*b)->pause(a);
	}
}
