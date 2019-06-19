#include "stdafx.h"
#pragma hdrstop

#include "fs_internal.h"

#pragma warning(disable:4995)
#include <io.h>
#include <direct.h>
#include <fcntl.h>
#include <sys\stat.h>
#pragma warning(default:4995)


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//---------------------------------------------------
// memory*/
CMemoryWriter::~CMemoryWriter()
{
    xr_free(data);
}

void CMemoryWriter::w(const void* ptr, u32 count)
{
    if (position + count > mem_size)
    {
        // reallocate
        if (mem_size == 0) mem_size = 128;
        while (mem_size <= (position + count)) mem_size *= 2;
        if (0 == data) data = (BYTE*)BearCore::BearMemory::Malloc(mem_size
            , "CMemoryWriter - storage"
            );
        else data = (BYTE*)BearCore::BearMemory::Realloc(data, mem_size
            , "CMemoryWriter - storage"
            );
    }
    CopyMemory(data + position, ptr, count);
    position += count;
    if (position > file_size) file_size = position;
}

//static const u32 mb_sz = 0x1000000;
bool CMemoryWriter::save_to(LPCSTR fn)
{
	BEAR_ASSERT(false);
	return false;
   /* IWriter* F = FS.w_open(fn);
    if (F)
    {
        F->w(pointer(), size());
        FS.w_close(F);
        return true;
    }
    return false;*/
}


void IWriter::open_chunk(u32 type)
{
    w_u32(type);
    chunk_pos.push(tell());
    w_u32(0); // the place for 'size'
}
void IWriter::close_chunk()
{
    VERIFY(!chunk_pos.empty());

    int pos = tell();
    seek(chunk_pos.top());
    w_u32(pos - chunk_pos.top() - 4);
    seek(pos);
    chunk_pos.pop();
}
u32 IWriter::chunk_size() // returns size of currently opened chunk, 0 otherwise
{
    if (chunk_pos.empty()) return 0;
    return tell() - chunk_pos.top() - 4;
}

void IWriter::w_compressed(void* ptr, u32 count)
{
    BYTE* dest = 0;
    unsigned dest_sz = 0;
    _compressLZ(&dest, &dest_sz, ptr, count);

    // if (g_dummy_stuff)
    // g_dummy_stuff (dest,dest_sz,dest);

    if (dest && dest_sz)
        w(dest, dest_sz);
    xr_free(dest);
}

void IWriter::w_chunk(u32 type, void* data, u32 size)
{
    open_chunk(type);
    if (type & CFS_CompressMark) w_compressed(data, size);
    else w(data, size);
    close_chunk();
}
void IWriter::w_sdir(const Fvector& D)
{
    Fvector C;
    float mag = D.magnitude();
    if (mag > XrMath::EPS_S)
    {
        C.div(D, mag);
    }
    else
    {
        C.set(0, 0, 1);
        mag = 0;
    }
    w_dir(C);
    w_float(mag);
}
void IWriter::w_printf(const char* format, ...)
{
    va_list mark;
    char buf[1024];

    va_start(mark, format);
#ifndef _EDITOR
    vsprintf_s(buf, format, mark);
#else
    vsprintf(buf, format, mark);
#endif
    va_end(mark);

    w(buf, xr_strlen(buf));
}
// base stream
IReader* IReader::open_chunk(u32 ID)
{
	BOOL bCompressed;

	u32 dwSize = find_chunk(ID, &bCompressed);
	if (dwSize != 0)
	{
		if (bCompressed)
		{
			BYTE* dest;
			unsigned dest_sz;
			_decompressLZ(&dest, &dest_sz, pointer(), dwSize);
			return xr_new<CTempReader>(dest, dest_sz, tell() + dwSize);
		}
		else
		{
			return xr_new<IReader>(pointer(), dwSize, tell() + dwSize);
		}
	}
	else return 0;
};
void IReader::close()
{
	if (this)
	{
		this->~IReader();
		BearCore::bear_free((void*)this);
	}
}

#include "FS_impl.h"

#ifdef TESTING_IREADER
IReaderTestPolicy::~IReaderTestPolicy()
{
	xr_delete(m_test);
};
#endif // TESTING_IREADER

#ifdef FIND_CHUNK_BENCHMARK_ENABLE
find_chunk_counter g_find_chunk_counter;
#endif // FIND_CHUNK_BENCHMARK_ENABLE

u32 IReader::find_chunk(u32 ID, BOOL* bCompressed)
{
	return inherited::find_chunk(ID, bCompressed);
}

IReader* IReader::open_chunk_iterator(u32& ID, IReader* _prev)
{
	if (0 == _prev)
	{
		// first
		rewind();
	}
	else
	{
		// next
		seek(_prev->iterpos);
		_prev->close();
	}

	// open
	if (elapsed() < 8) return NULL;
	ID = r_u32();
	u32 _size = r_u32();
	if (ID & CFS_CompressMark)
	{
		// compressed
		u8* dest;
		unsigned dest_sz;
		_decompressLZ(&dest, &dest_sz, pointer(), _size);
		return xr_new<CTempReader>(dest, dest_sz, tell() + _size);
	}
	else
	{
		// normal
		return xr_new<IReader>(pointer(), _size, tell() + _size);
	}
}

void IReader::r(void* p, int cnt)
{
	VERIFY(Pos + cnt <= Size);
	CopyMemory(p, pointer(), cnt);
	advance(cnt);
};

IC BOOL is_term(char a) { return (a == 13) || (a == 10); };
IC u32 IReader::advance_term_string()
{
	u32 sz = 0;
	char* src = (char*)data;
	while (!eof())
	{
		Pos++;
		sz++;
		if (!eof() && is_term(src[Pos]))
		{
			while (!eof() && is_term(src[Pos]))
				Pos++;
			break;
		}
	}
	return sz;
}
void IReader::r_string(char* dest, u32 tgt_sz)
{
	char* src = (char*)data + Pos;
	u32 sz = advance_term_string();
	R_ASSERT2(sz < (tgt_sz - 1), "Dest string less than needed.");
	R_ASSERT(!IsBadReadPtr((void*)src, sz));

#ifdef _EDITOR
	CopyMemory(dest, src, sz);
#else
	strncpy_s(dest, tgt_sz, src, sz);
#endif
	dest[sz] = 0;
}
void IReader::r_string(xr_string& dest)
{
	char* src = (char*)data + Pos;
	u32 sz = advance_term_string();
	dest.assign(src, sz);
}
void IReader::r_stringZ(char* dest, u32 tgt_sz)
{
	char* src = (char*)data;
	u32 sz = xr_strlen(src);
	R_ASSERT2(sz < tgt_sz, "Dest string less than needed.");
	while ((src[Pos] != 0) && (!eof())) *dest++ = src[Pos++];
	*dest = 0;
	Pos++;
}
void IReader::r_stringZ(shared_str& dest)
{
	dest = (char*)(data + Pos);
	Pos += (dest.size() + 1);
}
void IReader::r_stringZ(xr_string& dest)
{
	dest = (char*)(data + Pos);
	Pos += int(dest.size() + 1);
};

void IReader::skip_stringZ()
{
	char* src = (char*)data;
	while ((src[Pos] != 0) && (!eof())) Pos++;
	Pos++;
};
CTempReader::~CTempReader()
{
	xr_free(data);
};
//---------------------------------------------------
// pack stream
