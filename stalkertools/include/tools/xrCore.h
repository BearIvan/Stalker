#pragma once
#define _HAS_AUTO_PTR_ETC 1
#include <memory>
#include "BearCore.hpp"

// Warnings
#ifdef MSVC
#pragma warning(disable : 4251) // object needs DLL interface
#pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable : 4100) // unreferenced formal parameter
#pragma warning(disable : 4127) // conditional expression is constant
//#pragma warning (disable : 4530 ) // C++ exception handler used, but unwind semantics are not enabled
#pragma warning(disable : 4345)
#pragma warning(disable : 4714) // __forceinline not inlined

#ifndef DEBUG
#pragma warning(disable : 4189) // local variable is initialized but not refenced
#endif
#endif

#include <time.h>
#include <sys\utime.h>
// stl
#ifdef MSVC
#pragma warning(push)
#pragma warning(disable : 4702)
#endif
#include <functional>
#include <algorithm>
#include <limits>
#include <vector>
#include <stack>
#include <list>
#include <set>
#include <map>
#ifndef _EDITOR
#include <unordered_map>
#include <unordered_set>
#endif
#include <string>
#ifdef MSVC
#pragma warning(pop)
#pragma warning(disable : 4100) // unreferenced formal parameter
#endif
#ifdef XRCORE_EXPORTS
#define XRCORE_API BEARPROJECT_EXPORT
#else
#define XRCORE_API BEARPROJECT_IMPORT
#endif

#include "XrTypes.h"

#include "Debug/XrDebug.h"

#include "Template/XrFastdelegate.h"

#include "Utilities/XrRandom.h"

#include "Math/XrMath.h"

#include "Debug/XrLog.h"

#include "Utilities/XrColor.h"

#include "Utilities/XrClsid.h"

#include "Template/XrTypeTraits.h"

#include "Utilities/xrMemory.h"
#include "Utilities/XrString.h"

#include "Template/XrDeque.h"
#include "Template/XrHashMap.h"
#include "Template/XrList.h"
#include "Template/XrMap.h"
#include "Template/XrMultimap.h"
#include "Template/XrMultiset.h"
#include "Template/XrSet.h"
#include "Template/XrVector.h"
#include "Template/XrStack.h"
#include "Template/XrAlgorithm.h"

#include "Template/XrBufferVector.h"
#include "Template/XrFixedVector.h"
#include "Template/XrFixedSet.h"
#include "Template/XrFixedMap.h"

using std::swap;

#include "Utilities/XrClipboard.h"

#include "Utilities/XrSyncronize.h"

#include "Utilities/XrStringContainer.h"

#include "Utilities/XrToken.h"

#include "Compressor/XrCompressor.h"

#include "Filesystem/XrFS.h"

#include "Net/XrNetUtils.h"

#include "Timer/XrTimerController.h"
#include "Timer/XrTimer.h"

#include "Utilities/XrFlags.h"

#include "Utilities/XrThread.h"

DEFINE_VECTOR(shared_str, RStringVec, RStringVecIt);

#include "Parsers/XrTrims.h"

#include "Parsers/XrINI.h"

#include "Utilities/XrPool.h"
#include "Utilities/XrResource.h"

#include "Utilities/XrShared.h"
#include "Utilities/XrStringConcatenations.h"

#include "Debug/XrDumpString.h"

#include "Utilities/XrSharedMemory.h"

#include "Template/XrDestructor.h"

#include "Utilities/XrShortcut.h"

#include "Utilities/XrIntrusivePtr.h"

class XRCORE_API XrCore
{
public:
	static string64 UserName;
	static string64 CompName;
	static u32 dwFrame;

public:
	static void Initialize();
	static void Destroy();
};

#define _BCL
#define _BGCL
#define XR_ALIGNED  BEAR_ALIGNED