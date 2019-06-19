#include"stdafx.h"
typedef BearCore::BearVector<  BearCore::BearThread*> ThreadVector;
static ThreadVector *m_threads = 0;
XRCORE_API void XrThread::Spawn(const bchar * name, ThreadFunction function, void * args)
{
	m_threads->push_back(BearCore::bear_new< BearCore::BearThread>(function, args));
	m_threads->back()->Join(name);
}

void XrThread::Initialize()
{
	m_threads = BearCore::bear_new< ThreadVector>();
	
}

void XrThread::Destroy()
{
	BearCore::bear_delete(m_threads);
}
