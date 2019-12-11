#include"stdafx.h"
typedef BearVector<  BearThread*> ThreadVector;
static ThreadVector *m_threads = 0;
XRCORE_API void XrThread::Spawn(const bchar * name, ThreadFunction function, void * args)
{
	m_threads->push_back(bear_new< BearThread>(function, args));
	m_threads->back()->Join(name);
}

void XrThread::Initialize()
{
	m_threads = bear_new< ThreadVector>();
	
}

void XrThread::Destroy()
{
	bear_delete(m_threads);
}
