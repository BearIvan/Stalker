#include "stdafx.h"

XrTimerPaused::XrTimerPaused()
{
	//BearCore::BearTimer timer;

	XrTimerController::Register(this);
}

XrTimerPaused::~XrTimerPaused()
{
	XrTimerController::UnRegister(this);
}

void XrTimerStat::Begin()
{
	play(); m_count++;
}

void XrTimerStat::End()
{
	pause();
}

void XrTimerStat::FrameStart()
{
	restart();
	m_count = 0;
}

void XrTimerStat::FrameEnd()
{
	m_R += get_elapsed_time().asseconds();
}

XrTimerStat::XrTimerStat():m_R(0)
{

}
XrTimerStat::~XrTimerStat()
{

}