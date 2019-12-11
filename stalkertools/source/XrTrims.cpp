#include "stdafx.h"
#pragma hdrstop

 bchar* XrTrims::TrimLeft( bchar* str)
{
     bchar* p = str;
    while (*p && (u8(*p) <= u8(' '))) p++;
    if (p != str)
    {
		 bchar* t;
		for (t = str; *p; t++, p++) 
			*t = *p;
			*t = 0;
    }
    return str;
}

 bchar* XrTrims::TrimRight( bchar* str)
{
     bchar* p = str + BearString::GetSize(str);
    while ((p != str) && (u8(*p) <= u8(' '))) p--;
    *(++p) = 0;
    return str;
}

 bchar* XrTrims::Trim( bchar* str)
{
    TrimLeft(str);
    TrimRight(str);
    return str;
}

const bchar* XrTrims::SetPos(const bchar* src, bsize pos, char separator)
{
    const bchar* res = src;
    bsize p = 0;
    while ((p < pos) && (0 != (res = strchr(res, separator))))
    {
        res++;
        p++;
    }
    return res;
}

const bchar* XrTrims::CopyVal(const bchar* src,  bchar* dst, char separator)
{
    const bchar* p;
    size_t n;
    p = strchr(src, separator);
    n = (p > 0) ? (p - src) : BearString::GetSize(src);
	for (size_t i = 0; i < n; i++)
	{
		dst[i] = src[i];
	}
	//BearString::Copy(dst, n + 1, src);
   // strncpy(dst, src, n);
    dst[n] = 0;
    return dst;
}

bsize XrTrims::GetItemCount(const bchar* src, char separator)
{
    bsize cnt = 0;
    if (src&&src[0])
    {
        const bchar* res = src;
        const bchar* last_res = res;
        while (0 != (res = strchr(res, separator)))
        {
            res++;
            last_res = res;
            cnt++;
            if (res[0] == separator) break;
        }
        if (BearString::GetSize(last_res)) cnt++;
    }
    return cnt;
}

 bchar* XrTrims::GetItem(const bchar* src, bsize index,  bchar* dst, bsize const dst_size, char separator, const bchar* def, bool trim)
{
    const bchar* ptr;
    ptr = SetPos(src, index, separator);
    if (ptr) CopyVal(ptr, dst, separator);
    else BearString::Copy(dst, dst_size, def);
    if (trim) Trim(dst);
    return dst;
}

 bchar* XrTrims::GetItems(const bchar* src, bsize idx_start, bsize idx_end,  bchar* dst, char separator)
{
     bchar* n = dst;
    bsize level = 0;
    for (const bchar* p = src; *p != 0; p++)
    {
        if ((level >= idx_start) && (level < idx_end))
            *n++ = *p;
        if (*p == separator) level++;
        if (level >= idx_end) break;
    }
    *n = '\0';
    return dst;
}

bsize XrTrims::ParseItem(const bchar* src, xr_token* token_list)
{
    for (bsize i = 0; token_list[i].name; i++)
        if (!_stricmp(src, token_list[i].name))
            return token_list[i].id;
    return bsize(-1);
}

bsize XrTrims::ParseItem( bchar* src, bsize ind, xr_token* token_list)
{
    char dst[128];
    GetItem(src, ind, dst);
    return ParseItem(dst, token_list);
}

 bchar* XrTrims::ReplaceItems(const bchar* src, bsize idx_start, bsize idx_end, const bchar* new_items,  bchar* dst, char separator)
{
     bchar* n = dst;
    bsize level = 0;
    bool bCopy = true;
    for (const bchar* p = src; *p != 0; p++)
    {
        if ((level >= idx_start) && (level < idx_end))
        {
            if (bCopy)
            {
                for (const bchar* itm = new_items; *itm != 0;) *n++ = *itm++;
                bCopy = false;
            }
            if (*p == separator) *n++ = separator;
        }
        else
        {
            *n++ = *p;
        }
        if (*p == separator) level++;
    }
    *n = '\0';
    return dst;
}

 bchar* XrTrims::ReplaceItem(const bchar* src, bsize index, const bchar* new_item,  bchar* dst, char separator)
{
     bchar* n = dst;
    bsize level = 0;
    bool bCopy = true;
    for (const bchar* p = src; *p != 0; p++)
    {
        if (level == index)
        {
            if (bCopy)
            {
                for (const bchar* itm = new_item; *itm != 0;) *n++ = *itm++;
                bCopy = false;
            }
            if (*p == separator) *n++ = separator;
        }
        else
        {
            *n++ = *p;
        }
        if (*p == separator) level++;
    }
    *n = '\0';
    return dst;
}

 bchar* XrTrims::ChangeSymbol( bchar* name, char src, char dest)
{
    char* sTmpName = name;
    while (sTmpName[0])
    {
        if (sTmpName[0] == src) sTmpName[0] = dest;
        sTmpName++;
    }
    return name;
}

xr_string& XrTrims::ChangeSymbol(xr_string& name, char src, char dest)
{
    for (xr_string::iterator it = name.begin(); it != name.end(); it++)
        if (*it == src) *it = xr_string::value_type(dest);
    return name;
}

#ifdef M_BORLAND
AnsiString& _ReplaceItem(const bchar* src, bsize index, const bchar* new_item, AnsiString& dst, char separator)
{
    dst = "";
    bsize level = 0;
    bool bCopy = true;
    for (const bchar* p = src; *p != 0; p++)
    {
        if (level == index)
        {
            if (bCopy)
            {
                for (const bchar* itm = new_item; *itm != 0;) dst += *itm++;
                bCopy = false;
            }
            if (*p == separator) dst += separator;
        }
        else
        {
            dst += *p;
        }
        if (*p == separator) level++;
    }
    return dst;
}

AnsiString& _ReplaceItems(const bchar* src, bsize idx_start, bsize idx_end, const bchar* new_items, AnsiString& dst, char separator)
{
    dst = "";
    bsize level = 0;
    bool bCopy = true;
    for (const bchar* p = src; *p != 0; p++)
    {
        if ((level >= idx_start) && (level < idx_end))
        {
            if (bCopy)
            {
                for (const bchar* itm = new_items; *itm != 0;) dst += *itm++;
                bCopy = false;
            }
            if (*p == separator) dst += separator;
        }
        else
        {
            dst += *p;
        }
        if (*p == separator) level++;
    }
    return dst;
}

AnsiString& _Trim(AnsiString& str)
{
    return str = str.Trim();
}

const bchar* _CopyVal(const bchar* src, AnsiString& dst, char separator)
{
    const bchar* p;
    bsize n;
    p = strchr(src, separator);
    n = (p > 0) ? (p - src) : BearString::GetSize(src);
    dst = src;
    dst = dst.Delete(n + 1, dst.Length());
    return dst.c_str();
}

const bchar* _GetItems(const bchar* src, bsize idx_start, bsize idx_end, AnsiString& dst, char separator)
{
    bsize level = 0;
    for (const bchar* p = src; *p != 0; p++)
    {
        if ((level >= idx_start) && (level < idx_end))
            dst += *p;
        if (*p == separator) level++;
        if (level >= idx_end) break;
    }
    return dst.c_str();
}

const bchar* _GetItem(const bchar* src, bsize index, AnsiString& dst, char separator, const bchar* def, bool trim)
{
    const bchar* ptr;
    ptr = _SetPos(src, index, separator);
    if (ptr) _CopyVal(ptr, dst, separator);
    else dst = def;
    if (trim) dst = dst.Trim();
    return dst.c_str();
}

AnsiString _ListToSequence(const AStringVec& lst)
{
    AnsiString out;
    out = "";
    if (lst.size())
    {
        out = lst.front();
        for (AStringVec::const_iterator s_it = lst.begin() + 1; s_it != lst.end(); s_it++)
            out += AnsiString(",") + (*s_it);
    }
    return out;
}

AnsiString _ListToSequence2(const AStringVec& lst)
{
    AnsiString out;
    out = "";
    if (lst.size())
    {
        out = lst.front();
        for (AStringVec::const_iterator s_it = lst.begin() + 1; s_it != lst.end(); s_it++)
        {
            out += AnsiString("\n") + (*s_it);
        }
    }
    return out;
}

void _SequenceToList(AStringVec& lst, const bchar* in, char separator)
{
    lst.clear();
    bsize t_cnt = _GetItemCount(in, separator);
    AnsiString T;
    for (bsize i = 0; i < t_cnt; i++)
    {
        _GetItem(in, i, T, separator, 0);
        _Trim(T);
        if (!T.IsEmpty()) lst.push_back(T);
    }
}

AnsiString FloatTimeToStrTime(float v, bool _h, bool _m, bool _s, bool _ms)
{
    AnsiString buf = "";
    bsize h = 0, m = 0, s = 0, ms;
    AnsiString t;
    if (_h) { h = iFloor(v / 3600); t.sprintf("%02d", h); buf += t; }
    if (_m) { m = iFloor((v - h * 3600) / 60); t.sprintf("%02d", m); buf += buf.IsEmpty() ? t : ":" + t; }
    if (_s) { s = iFloor(v - h * 3600 - m * 60); t.sprintf("%02d", s); buf += buf.IsEmpty() ? t : ":" + t; }
    if (_ms) { ms = iFloor((v - h * 3600 - m * 60 - s)*1000.f); t.sprintf("%03d", ms); buf += buf.IsEmpty() ? t : "." + t; }
    return buf;
}

float StrTimeToFloatTime(const bchar* buf, bool _h, bool _m, bool _s, bool _ms)
{
    float t[4] = {0.f, 0.f, 0.f, 0.f};
    bsize rm[4];
    bsize idx = 0;
    if (_h) rm[0] = idx++;
    if (_m) rm[1] = idx++;
    if (_s) rm[2] = idx++;
    if (_ms)rm[3] = idx;
    bsize cnt = _GetItemCount(buf, ':');
    AnsiString tmp;
    for (bsize k = 0; k < cnt; k++)
    {
        _GetItem(buf, k, tmp, ':');
        t[rm[k]] = atof(tmp.c_str());
    }
    return t[0] * 3600 + t[1] * 60 + t[2];
}
#endif

void XrTrims::SequenceToList(LPSTRVec& lst, const bchar* in, char separator)
{
    bsize t_cnt = GetItemCount(in, separator);
    string1024 T;
    for (bsize i = 0; i < t_cnt; i++)
    {
        GetItem(in, i, T, separator, 0);
        Trim(T);
        if (BearString::GetSize(T)) lst.push_back(xr_strdup(T));
    }
}

void XrTrims::SequenceToList(RStringVec& lst, const bchar* in, char separator)
{
    lst.clear();
    bsize t_cnt = GetItemCount(in, separator);
    xr_string T;
    for (bsize i = 0; i < t_cnt; i++)
    {
        GetItem(in, i, T, separator, 0);
        Trim(T);
        if (T.size()) lst.push_back(T.c_str());
    }
}

void XrTrims::SequenceToList(SStringVec& lst, const bchar* in, char separator)
{
    lst.clear();
    bsize t_cnt = GetItemCount(in, separator);
    xr_string T;
    for (bsize i = 0; i < t_cnt; i++)
    {
        GetItem(in, i, T, separator, 0);
        Trim(T);
        if (T.size()) lst.push_back(T.c_str());
    }
}

xr_string XrTrims::ListToSequence(const SStringVec& lst)
{
    static xr_string out;
    out = "";
    if (lst.size())
    {
        out = lst.front();
        for (SStringVec::const_iterator s_it = lst.begin() + 1; s_it != lst.end(); s_it++)
            out += xr_string(",") + (*s_it);
    }
    return out;
}

xr_string& XrTrims::TrimLeft(xr_string& str)
{
    const bchar* b = str.c_str();
    const bchar* p = str.c_str();
    while (*p && (u8(*p) <= u8(' '))) p++;
    if (p != b)
        str.erase(0, p - b);
    return str;
}

xr_string&XrTrims::TrimRight(xr_string& str)
{
    const bchar* b = str.c_str();
    size_t l = str.length();
    if (l)
    {
        const bchar* p = str.c_str() + l - 1;
        while ((p != b) && (u8(*p) <= u8(' '))) p--;
        if (p != (str + b)) str.erase(p - b + 1, l - (p - b));
    }
    return str;
}

xr_string& XrTrims::Trim(xr_string& str)
{
    TrimLeft(str);
    TrimRight(str);
    return str;
}

const bchar* XrTrims::CopyVal(const bchar* src, xr_string& dst, char separator)
{
    const bchar* p;
    ptrdiff_t n;
    p = strchr(src, separator);
    n = (p > 0) ? (p - src) : BearString::GetSize(src);
    dst = src;
    dst = dst.erase(n, dst.length());
    return dst.c_str();
}

const bchar* XrTrims::GetItem(const bchar* src, bsize index, xr_string& dst, char separator, const bchar* def, bool trim)
{
    const bchar* ptr;
    ptr = SetPos(src, index, separator);
    if (ptr) CopyVal(ptr, dst, separator);
    else dst = def;
    if (trim)XrTrims::Trim(dst);
    return dst.c_str();
}

shared_str XrTrims::ListToSequence(const RStringVec& lst)
{
    xr_string out;
    if (lst.size())
    {
        out = *lst.front();
        for (RStringVec::const_iterator s_it = lst.begin() + 1; s_it != lst.end(); s_it++)
        {
            out += ",";
            out += **s_it;
        }
    }
    return shared_str(out.c_str());
}

