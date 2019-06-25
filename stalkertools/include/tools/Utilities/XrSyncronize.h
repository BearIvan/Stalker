#pragma once
// Desc: Simple wrapper for critical section
class xrCriticalSection
{
	BEAR_CLASS_NO_COPY(xrCriticalSection);
public:
	class XRCORE_API raii
	{
	public:
		raii(xrCriticalSection*);
		~raii();

	private:
		xrCriticalSection* critical_section;
	};

private:
	BearCore::BearMutex m_mutex;
public:
	xrCriticalSection() {}
	IC ~xrCriticalSection() {}

	IC void Enter(){m_mutex.Lock();}
	IC void Leave() { m_mutex.Unlock(); }
	IC BOOL TryEnter() { m_mutex.TryLock(); }
};
