// xrXRC.cpp: implementation of the xrXRC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xrXRC.h"

XRCDB_API xrXRC XRC;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef	DEBUG
XrTimerStat	_clRAY;								// total: ray-testing
XrTimerStat	_clBOX;								// total: box query
XrTimerStat	_clFRUSTUM;							// total: frustum query
XrTimerStat	*cdb_clRAY		= &_clRAY;				// total: ray-testing
XrTimerStat	*cdb_clBOX		= &_clBOX;				// total: box query
XrTimerStat	*cdb_clFRUSTUM	= &_clFRUSTUM;			// total: frustum query
#endif

xrXRC::xrXRC()
{

}

xrXRC::~xrXRC()
{

}
