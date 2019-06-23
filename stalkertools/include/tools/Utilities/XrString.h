#pragma once

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
	_strlwr_s(S, strlen(S) + 1);
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

typedef std::basic_string<char, std::char_traits<char>, xalloc<char> > xr_string;