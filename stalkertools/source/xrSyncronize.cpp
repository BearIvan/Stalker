#include "stdafx.h"




xrCriticalSection::raii::raii(xrCriticalSection* critical_section)
    : critical_section(critical_section)
{
    VERIFY(critical_section);
    critical_section->Enter();
}

xrCriticalSection::raii::~raii()
{
    critical_section->Leave();
}
