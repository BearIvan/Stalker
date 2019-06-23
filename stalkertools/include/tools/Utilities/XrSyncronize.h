#pragma once
// Desc: Simple wrapper for critical section
class XRCORE_API xrCriticalSection
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
    void* pmutex;

public:
    xrCriticalSection();
    ~xrCriticalSection();

    void Enter();
    void Leave();
    BOOL TryEnter();
};
