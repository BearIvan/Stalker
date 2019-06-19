#pragma once
#include "BearCore.hpp"

#ifndef DEBUG
# define MASTER_GOLD
#endif // DEBUG

# define BENCH_SEC_CALLCONV
# define BENCH_SEC_SCRAMBLEVTBL1
# define BENCH_SEC_SCRAMBLEVTBL2
# define BENCH_SEC_SCRAMBLEVTBL3
# define BENCH_SEC_SIGN
# define BENCH_SEC_SCRAMBLEMEMBER1
# define BENCH_SEC_SCRAMBLEMEMBER2

#define XRAY_EXCEPTIONS 0 // XRAY

# include "xrCore_platform.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <typeinfo.h>

#define _inline inline
#define __inline inline
#define IC inline
#define ICF __forceinline 
# define ICN __declspec (noinline)


#include <time.h>

#define ALIGN(a) __declspec(align(a))
#include <sys\utime.h>

// Warnings
#pragma warning (disable : 4251 ) // object needs DLL interface
#pragma warning (disable : 4201 ) // nonstandard extension used : nameless struct/union
#pragma warning (disable : 4100 ) // unreferenced formal parameter
#pragma warning (disable : 4127 ) // conditional expression is constant
//#pragma warning (disable : 4530 ) // C++ exception handler used, but unwind semantics are not enabled
#pragma warning (disable : 4345 )
#pragma warning (disable : 4714 ) // __forceinline not inlined

#ifndef DEBUG
#pragma warning (disable : 4189 ) // local variable is initialized but not refenced
#endif 



// stl
#pragma warning (push)
#pragma warning (disable:4702)
#include <algorithm>
#include <limits>
#include <vector>
#include <stack>
#include <list>
#include <set>
#include <map>

#ifndef _EDITOR
# include <unordered_map>
# include <unordered_set>
#endif

#include <string>
#pragma warning (pop)
#pragma warning (disable : 4100 ) // unreferenced formal parameter

# ifdef XRCORE_EXPORTS
# define XRCORE_API BEARPROJECT_EXPORT
# else
# define XRCORE_API BEARPROJECT_IMPORT
# endif

#include "xrDebug.h"

#include "XrTypes.h"
#include "Utilities/XrRandom.h"
#include "Math/XrMath.h"


#include "vector.h"

#include "clsid.h"
#include "Utilities/XrSyncronize.h"
#include "Utilities/xrMemory.h"
#include "xrDebug.h"

#include "_stl_extensions.h"
#include "xrsharedmem.h"
#include "xrstring.h"
#include "xr_resource.h"
#include "rt_compressor.h"
#include "xr_shared.h"
#include "string_concatenations.h"
#include "Utilities/XrThread.h"
// stl ext
struct XRCORE_API xr_rtoken
{
    shared_str name;
    int id;
    xr_rtoken(LPCSTR _nm, int _id) { name = _nm; id = _id; }
public:
    void rename(LPCSTR _nm) { name = _nm; }
    bool equal(LPCSTR _nm) { return (0 == xr_strcmp(*name, _nm)); }
};

#pragma pack (push,1)
struct XRCORE_API xr_shortcut
{
    enum
    {
        flShift = 0x20,
        flCtrl = 0x40,
        flAlt = 0x80,
    };
    union
    {
        struct
        {
            u8 key;
            Flags8 ext;
        };
        u16 hotkey;
    };
    xr_shortcut(u8 k, BOOL a, BOOL c, BOOL s) :key(k) { ext.assign(u8((a ? flAlt : 0) | (c ? flCtrl : 0) | (s ? flShift : 0))); }
    xr_shortcut() { ext.zero(); key = 0; }
    bool similar(const xr_shortcut& v)const { return ext.equal(v.ext) && (key == v.key); }
};
#pragma pack (pop)

DEFINE_VECTOR(shared_str, RStringVec, RStringVecIt);
DEFINE_SET(shared_str, RStringSet, RStringSetIt);
DEFINE_VECTOR(xr_rtoken, RTokenVec, RTokenVecIt);

#define xr_pure_interface __interface

#include "FS.h"
#include "log.h"
#include "Utilities/XrTrims.h"
#include "xr_ini.h"
#include "FTimer.h"
#include "fastdelegate.h"
#include "intrusive_ptr.h"

#include "net_utils.h"

// destructor
template <class T>
class destructor
{
    T* ptr;
public:
    destructor(T* p) { ptr = p; }
    ~destructor() { xr_delete(ptr); }
    IC T& operator() ()
    {
        return *ptr;
    }
};

// ********************************************** The Core definition


#include "XrayToBearCore.h"
class XRCORE_API xrCore
{
public:
	static  string64 UserName;
	static string64 CompName;
	static u32 dwFrame;

public:
    static void Initialize(LogCallback cb = 0);
	static void Destroy();
};

#define _BCL 
#define _BGCL 

extern XRCORE_API xrCore Core;

