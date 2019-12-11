#pragma once

IC char* strext(const char* S)
{
	return (char*)BearString::ToChar(S, '.');
}

IC bsize xr_strlen(const char* S)
{
	if (S == 0)return 0;
	return BearString::GetSize(S);
}

IC char* xr_strlwr(char* S)
{
	BearString::ToLower(S);
	return S;
}

IC int xr_strcmp(const char* S1, const char* S2)
{
	return BearString::Compare(S1, S2);
}

IC void xr_strcpy(LPSTR destination, bsize const destination_size, LPCSTR source)
{
	BearString::Copy(destination, destination_size, source);
}

IC void xr_strcat(LPSTR destination, bsize const buffer_size, LPCSTR source)
{
	BearString::Contact(destination, buffer_size, source);
}

IC bsize __cdecl xr_sprintf(LPSTR destination, bsize const buffer_size, LPCSTR format_string, ...)
{

	va_list args;
	va_start(args, format_string);
	bsize result = BearString::PrintfVa(destination, buffer_size, format_string, args);
	va_end(args);
	return result;
}

template <bsize buffer_size>
IC bsize __cdecl xr_sprintf(char(&destination)[buffer_size], LPCSTR format_string, ...)
{
	va_list args;
	va_start(args, format_string);
	bsize result = BearString::PrintfVa(destination, buffer_size, format_string, args);
	va_end(args);
	return result;
}


template <int count>
inline void xr_strcpy(char(&destination)[count], LPCSTR source)
{
	 xr_strcpy(destination, count, source);
}

template <int count>
inline void xr_strcat(char(&destination)[count], LPCSTR source)
{
	 xr_strcat(destination, count, source);
}

typedef std::basic_string<char, std::char_traits<char>, xalloc<char> > xr_string;

IC bchar*xr_strdup(const bchar*str) { return BearString::Duplicate(str); }
IC void xr_strlwr(xr_string& src) { for (xr_string::iterator it = src.begin(); it != src.end(); it++) *it = xr_string::value_type(tolower(*it)); }

