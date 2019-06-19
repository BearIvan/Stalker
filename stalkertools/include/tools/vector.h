#ifndef _vector_included
#define _vector_included

// Undef some macros
#ifdef M_PI
#undef M_PI
#endif

#ifdef PI
#undef PI
#endif

#ifdef FLT_MAX
#undef FLT_MAX
#undef FLT_MIN
#endif

// Select platform
#ifdef _MSC_VER
#define M_VISUAL
#endif
#ifdef __BORLANDC__
#define M_BORLAND
#endif

// Constants
#ifdef M_VISUAL




#endif
#ifdef M_BORLAND
#define EPS_S 0.0000001f
#define EPS 0.0000100f
#define EPS_L 0.0010000f

#define M_PI 3.1415926535897932384626433832795f
#define PI 3.1415926535897932384626433832795f
#define PI_MUL_2 6.2831853071795864769252867665590f
#define PI_MUL_3 9.4247779607693797153879301498385f
#define PI_MUL_4 12.566370614359172953850573533118f
#define PI_DIV_3 1.0471975511965977461542144610932f
#define PI_MUL_6 18.849555921538759430775860299677f
#define PI_MUL_8 25.132741228718345907701147066236f
#define PI_MUL_2 6.2831853071795864769252867665590f
#define PI_DIV_2 1.5707963267948966192313216916398f
#define PI_DIV_4 0.7853981633974483096156608458199f
#define PI_DIV_6 0.5235987755982988730771072305466f
#define PI_DIV_8 0.3926990816987241548078304229099f
#endif


// Define types and namespaces (CPU & FPU)

#include "_math.h"
#include "_std_extensions.h"

// comparisions

// degree 2 radians and vice-versa

// pre-definitions
template <class T> struct _quaternion;

#pragma pack(push)
#pragma pack(1)

#include "Utilities/XrColor.h"
#include "_compressed_normal.h"
#include "_flags.h"
#ifdef DEBUG
#include "dump_string.h"
#endif
#pragma pack(pop)



//----------------------------------------------------------------------------------------------
// Deprecate some features
#ifndef XRCORE_EXPORTS
//. #pragma deprecated("MIN","MAX","ABS",fabs,fabsf,sqrt,sqrtf,malloc,free,calloc,realloc,memcpy,memmove,memset,strdup,strlen,strcmp,sin,cos,sinf,cosf)
#pragma deprecated("MIN","MAX","ABS",fabs,fabsf,sqrt,sqrtf,malloc,free,calloc,realloc,memmove,memset,strdup,strlen,strcmp,sin,cos,sinf,cosf)
#endif

#endif
