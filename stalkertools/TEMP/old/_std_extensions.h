#ifndef _STD_EXT_internal
#define _STD_EXT_internal

#define BREAK_AT_STRCMP
#ifndef DEBUG
#undef BREAK_AT_STRCMP
#endif
#ifdef _EDITOR
#undef BREAK_AT_STRCMP
#endif

#ifdef abs
#undef abs
#endif

#ifdef _MIN
#undef _MIN
#endif

#ifdef _MAX
#undef _MAX
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#ifdef _EDITOR
IC char* strncpy_s(char* strDestination, size_t sizeInBytes, const char* strSource, size_t count)
{
    return strncpy(strDestination, strSource, count);
}

IC char* xr_strcpy(char* strDestination, size_t sizeInBytes, const char* strSource)
{
    return strcpy(strDestination, strSource);
}

IC char* xr_strcpy(char* strDestination, const char* strSource)
{
    return strcpy(strDestination, strSource);
}

IC char* _strlwr_s(char* strDestination, size_t sizeInBytes)
{
    return strlwr(strDestination);
}

IC char* xr_strcat(char* strDestination, size_t sizeInBytes, const char* strSource)
{
    return strncat(strDestination, strSource, sizeInBytes);
}

IC char* xr_strcat(char* strDestination, const char* strSource)
{
    return strcat(strDestination, strSource);
}

IC int xr_sprintf(char* dest, size_t sizeOfBuffer, const char* format, ...)
{
    va_list mark;
    va_start(mark, format);
    int sz = _vsnprintf(dest, sizeOfBuffer, format, mark);
    dest[sizeOfBuffer - 1] = 0;
    va_end(mark);
    return sz;
}
#endif

// token type definition
struct XRCORE_API xr_token
{
    LPCSTR name;
    int id;
};

IC LPCSTR get_token_name(xr_token* tokens, int key)
{
    for (int k = 0; tokens[k].name; k++)
        if (key == tokens[k].id) return tokens[k].name;
    return "";
}

IC int get_token_id(xr_token* tokens, LPCSTR key)
{
    for (int k = 0; tokens[k].name; k++)
        if (_stricmp(tokens[k].name, key) == 0)
            return tokens[k].id;
    return -1;
}

struct XRCORE_API xr_token2
{
    LPCSTR name;
    LPCSTR info;
    int id;
};

// generic

IC BOOL _valid(const float x)
{
    // check for: Signaling NaN, Quiet NaN, Negative infinity ( –INF), Positive infinity (+INF), Negative denormalized, Positive denormalized
    int cls = _fpclass(double(x));
    if (cls&(_FPCLASS_SNAN + _FPCLASS_QNAN + _FPCLASS_NINF + _FPCLASS_PINF + _FPCLASS_ND + _FPCLASS_PD))
        return false;

    /* *****other cases are*****
    _FPCLASS_NN Negative normalized non-zero
    _FPCLASS_NZ Negative zero ( – 0)
    _FPCLASS_PZ Positive 0 (+0)
    _FPCLASS_PN Positive normalized non-zero
    */
    return true;
}



IC BOOL _valid(const double x)
{
    // check for: Signaling NaN, Quiet NaN, Negative infinity ( –INF), Positive infinity (+INF), Negative denormalized, Positive denormalized
    int cls = _fpclass(x);
    if (cls&(_FPCLASS_SNAN + _FPCLASS_QNAN + _FPCLASS_NINF + _FPCLASS_PINF + _FPCLASS_ND + _FPCLASS_PD))
        return false;

    /* *****other cases are*****
    _FPCLASS_NN Negative normalized non-zero
    _FPCLASS_NZ Negative zero ( – 0)
    _FPCLASS_PZ Positive 0 (+0)
    _FPCLASS_PN Positive normalized non-zero
    */
    return true;
}


IC u32 xr_strlen(const char* S);

// string management

// return pointer to ".ext"
IC char* strext(const char* S)
{
    return (char*)strrchr(S, '.');
}

IC u32 xr_strlen(const char* S)
{
	if (S == 0)return 0;
    return (u32)strlen(S);
}

IC char* xr_strlwr(char* S)
{
    _strlwr_s(S,strlen(S)+1);
	return S;
}

#ifdef BREAK_AT_STRCMP
XRCORE_API int xr_strcmp(const char* S1, const char* S2);
#else
IC int xr_strcmp(const char* S1, const char* S2)
{
    return (int)strcmp(S1, S2);
}
#endif

#ifndef _EDITOR
#ifndef MASTER_GOLD

inline errno_t xr_strcpy(LPSTR destination, size_t const destination_size, LPCSTR source)
{
    return strcpy_s(destination, destination_size, source);
}

inline errno_t xr_strcat(LPSTR destination, size_t const buffer_size, LPCSTR source)
{
    return strcat_s(destination, buffer_size, source);
}

inline int __cdecl xr_sprintf(LPSTR destination, size_t const buffer_size, LPCSTR format_string, ...)
{
    va_list args;
    va_start(args, format_string);
    return vsprintf_s(destination, buffer_size, format_string, args);
}

template <int count>
inline int __cdecl xr_sprintf(char(&destination)[count], LPCSTR format_string, ...)
{
    va_list args;
    va_start(args, format_string);
    return vsprintf_s(destination, count, format_string, args);
}
#else // #ifndef MASTER_GOLD

inline errno_t xr_strcpy(LPSTR destination, size_t const destination_size, LPCSTR source)
{
    return strncpy_s(destination, destination_size, source, destination_size);
}

inline errno_t xr_strcat(LPSTR destination, size_t const buffer_size, LPCSTR source)
{
    size_t const destination_length = xr_strlen(destination);
    LPSTR i = destination + destination_length;
    LPSTR const e = destination + buffer_size - 1;
    if (i > e)
        return 0;

    for (LPCSTR j = source; *j && (i != e); ++i, ++j)
        *i = *j;

    *i = 0;
    return 0;
}

inline int __cdecl xr_sprintf(LPSTR destination, size_t const buffer_size, LPCSTR format_string, ...)
{
    va_list args;
    va_start(args, format_string);
    return vsnprintf_s(destination, buffer_size, buffer_size - 1, format_string, args);
}

template <int count>
inline int __cdecl xr_sprintf(char(&destination)[count], LPCSTR format_string, ...)
{
    va_list args;
    va_start(args, format_string);
    return vsnprintf_s(destination, count, count - 1, format_string, args);
}
#endif // #ifndef MASTER_GOLD

# pragma deprecated( strcpy, strcpy_s, sprintf, sprintf_s, strcat, strcat_s )

template <int count>
inline errno_t xr_strcpy(char(&destination)[count], LPCSTR source)
{
    return xr_strcpy(destination, count, source);
}

template <int count>
inline errno_t xr_strcat(char(&destination)[count], LPCSTR source)
{
    return xr_strcat(destination, count, source);
}
#endif // #ifndef _EDITOR

XRCORE_API char* timestamp(string64& dest);

extern XRCORE_API u32 crc32(const void* P, u32 len);
extern XRCORE_API u32 crc32(const void* P, u32 len, u32 starting_crc);
extern XRCORE_API u32 path_crc32(const char* path, u32 len); // ignores '/' and '\'

#endif // _STD_EXT_internal