////////////////////////////////////////////////////////////////////////////
// Module : XrClipboard.cpp
// Created : 21.02.2008
// Author : Evgeniy Sokolov
// Description : os clipboard class implementation
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma hdrstop

void XrClipboard::copy_to_clipboard(LPCSTR buf)
{
    if (!OpenClipboard(0))
        return;
    u32 handle_size = (BearCore::BearString::GetSize(buf) + 1) * sizeof(char);
    HGLOBAL handle = GlobalAlloc(GHND, handle_size);
    if (!handle)
    {
        CloseClipboard();
        return;
    }

    char* memory = (char*)GlobalLock(handle);
    BearCore::BearString::Copy(memory, handle_size, buf);
    GlobalUnlock(handle);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, handle);
    CloseClipboard();
}

void XrClipboard::paste_from_clipboard(LPSTR buffer, u32 const& buffer_size)
{
    VERIFY(buffer);
    VERIFY(buffer_size > 0);

    if (!OpenClipboard(0))
        return;

    HGLOBAL hmem = GetClipboardData(CF_TEXT);
    if (!hmem)
        return;

    LPCSTR clipdata = (LPCSTR)GlobalLock(hmem);
    strncpy_s(buffer, buffer_size, clipdata, buffer_size - 1);
    buffer[buffer_size - 1] = 0;
    for (u32 i = 0; i < strlen(buffer); ++i)
    {
        char c = buffer[i];
        if (((isprint(c) == 0) && (c != char(-1))) || c == '\t' || c == '\n')// "�" = -1
        {
            buffer[i] = ' ';
        }
    }

    GlobalUnlock(hmem);
    CloseClipboard();
}

void XrClipboard::update_clipboard(LPCSTR string)
{
    if (!OpenClipboard(0))
        return;

    HGLOBAL handle = GetClipboardData(CF_TEXT);
    if (!handle)
    {
        CloseClipboard();
        copy_to_clipboard(string);
        return;
    }

    LPSTR memory = (LPSTR)GlobalLock(handle);
    int memory_length = (int)strlen(memory);
    int string_length = (int)strlen(string);
    int buffer_size = (memory_length + string_length + 1) * sizeof(char);
#ifndef _EDITOR
    LPSTR buffer = (LPSTR)_alloca(buffer_size);
#else // #ifndef _EDITOR
    LPSTR buffer = (LPSTR)xr_alloc<char>( buffer_size );
#endif // #ifndef _EDITOR
	BearCore::BearString::Copy(buffer, buffer_size, memory);
    GlobalUnlock(handle);

	BearCore::BearString::Contact(buffer, buffer_size, string);
    CloseClipboard();
    copy_to_clipboard(buffer);
#ifdef _EDITOR
    xr_free (buffer);
#endif // #ifdef _EDITOR
}