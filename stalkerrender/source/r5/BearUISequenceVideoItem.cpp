#include "pch.h"

BearUISequenceVideoItem::BearUISequenceVideoItem()
{
}

void BearUISequenceVideoItem::Copy(IUISequenceVideoItem & _in)
{
}

bool BearUISequenceVideoItem::HasTexture()
{
	return false;
}

void BearUISequenceVideoItem::CaptureTexture()
{
}

void BearUISequenceVideoItem::ResetTexture()
{
}

BOOL BearUISequenceVideoItem::video_IsPlaying()
{
	return 0;
}

void BearUISequenceVideoItem::video_Sync(u32 _time)
{
}

void BearUISequenceVideoItem::video_Play(BOOL looped, u32 _time)
{
}

void BearUISequenceVideoItem::video_Stop()
{
}
