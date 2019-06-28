// FS.h: interface for the CFS class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4661)
#endif

//------------------------------------------------------------------------------------
// Write
//------------------------------------------------------------------------------------
class XRCORE_API IWriter
{
private:
    xr_stack<bsize> chunk_pos;
private:
	static constexpr u32 CFS_CompressMark = 1ul << 31ul;
public:
    shared_str fName;
public:
    IWriter()
    {
    }
    virtual ~IWriter()
    {
        R_ASSERT3(chunk_pos.empty(), "Opened chunk not closed.", *fName);
    }

    // kernel
    virtual void seek(bsize pos) = 0;
    virtual bsize tell() = 0;

    virtual void w(const void* ptr, bsize count) = 0;

    // generalized writing functions
    IC void w_u64(u64 d) { w(&d, sizeof(u64)); }
    IC void w_u32(u32 d) { w(&d, sizeof(u32)); }
    IC void w_u16(u16 d) { w(&d, sizeof(u16)); }
    IC void w_u8(u8 d) { w(&d, sizeof(u8)); }
    IC void w_s64(s64 d) { w(&d, sizeof(s64)); }
    IC void w_s32(s32 d) { w(&d, sizeof(s32)); }
    IC void w_s16(s16 d) { w(&d, sizeof(s16)); }
    IC void w_s8(s8 d) { w(&d, sizeof(s8)); }
    IC void w_float(float d) { w(&d, sizeof(float)); }
    IC void w_string(const char* p) { w(p, (bsize)BearCore::BearString::GetSize(p)); w_u8(13); w_u8(10); }
    IC void w_stringZ(const char* p) { w(p, (bsize)BearCore::BearString::GetSize(p) + 1); }
    IC void w_stringZ(const shared_str& p) { w(*p ? *p : "", p.size()); w_u8(0); }
    IC void w_stringZ(shared_str& p) { w(*p ? *p : "", p.size()); w_u8(0); }
    IC void w_stringZ(const xr_string& p) { w(p.c_str() ? p.c_str() : "", (bsize)p.size()); w_u8(0); }
    IC void w_fcolor(const Fcolor& v) { w(&v, sizeof(Fcolor)); }
    IC void w_fvector4(const Fvector4& v) { w(&v, sizeof(Fvector4)); }
    IC void w_fvector3(const Fvector3& v) { w(&v, sizeof(Fvector3)); }
    IC void w_fvector2(const Fvector2& v) { w(&v, sizeof(Fvector2)); }
    IC void w_ivector4(const Ivector4& v) { w(&v, sizeof(Ivector4)); }
    IC void w_ivector3(const Ivector3& v) { w(&v, sizeof(Ivector3)); }
    IC void w_ivector2(const Ivector2& v) { w(&v, sizeof(Ivector2)); }

    // quant writing functions
    IC void w_float_q16(float a, float min, float max)
    {
        VERIFY(a >= min && a <= max);
        float q = (a - min) / (max - min);
        w_u16(u16(XrMath::iFloor(q*65535.f + .5f)));
    }
    IC void w_float_q8(float a, float min, float max)
    {
        VERIFY(a >= min && a <= max);
        float q = (a - min) / (max - min);
        w_u8(u8(XrMath::iFloor(q*255.f + .5f)));
    }
    IC void w_angle16(float a) { w_float_q16(XrMath::angle_normalize(a), 0, XrMath::PI_MUL_2); }
    IC void w_angle8(float a) { w_float_q8(XrMath::angle_normalize(a), 0, XrMath::PI_MUL_2); }
    IC void w_dir(const Fvector& D) { w_u16(XrCompressor::PvCompress(D)); }
    void w_sdir(const Fvector& D);
    void __cdecl w_printf(const char* format, ...);

    // generalized chunking
	bsize align();
    void open_chunk(u32 type);
    void close_chunk();
	bsize chunk_size(); // returns size of currently opened chunk, 0 otherwise
    void w_compressed(void* ptr, bsize count);
    void w_chunk(u32 type, void* data, bsize size);
    virtual bool valid() { return true; }
    virtual void flush() = 0;
};

class XRCORE_API CMemoryWriter : public IWriter
{
    u8* data;
	bsize position;
	bsize mem_size;
	bsize file_size;
public:
    CMemoryWriter()
    {
        data = 0;
        position = 0;
        mem_size = 0;
        file_size = 0;
    }
    virtual ~CMemoryWriter();

    // kernel
    virtual void w(const void* ptr, bsize count);

    virtual void seek(bsize pos) { position = pos; }
    virtual bsize tell() { return position; }

    // specific
    IC u8* pointer() { return data; }
    IC bsize size() const { return file_size; }
    IC void clear() { file_size = 0; position = 0; }
#pragma warning(push)
#pragma warning(disable:4995)
    IC void free() { file_size = 0; position = 0; mem_size = 0; xr_free(data); }
#pragma warning(pop)
    bool save_to(LPCSTR fn);
    virtual void flush() { };
};

//------------------------------------------------------------------------------------
// Read
//------------------------------------------------------------------------------------

// Uncomment following line to try other implementations in FS_impl.h
//#define TESTING_IREADER

#ifdef TESTING_IREADER
struct IReaderBase_Test;

struct XRCORE_API IReaderTestPolicy
{
    IReaderBase_Test* m_test;
    IReaderTestPolicy() { m_test = NULL; }
    ~IReaderTestPolicy(); // defined in FS.cpp
};
#endif // TESTING_IREADER

template <typename implementation_type>
class IReaderBase

#ifdef TESTING_IREADER
    : public IReaderTestPolicy // inheriting
#endif //TESTING_IREADER

{
public:
    IC IReaderBase() : m_last_pos(0) {}
    virtual ~IReaderBase() {}

    IC implementation_type& impl() { return *(implementation_type*)this; }
    IC const implementation_type& impl() const { return *(implementation_type*)this; }

    IC BOOL eof() const { return impl().elapsed() <= 0; };

    IC void r(void* p, bsize cnt) { impl().r(p, cnt); }

    IC Fvector r_vec3() { Fvector tmp; r(&tmp, 3 * sizeof(float)); return tmp; };
    IC Fvector4 r_vec4() { Fvector4 tmp; r(&tmp, 4 * sizeof(float)); return tmp; };
    IC u64 r_u64() { u64 tmp; r(&tmp, sizeof(tmp)); return tmp; };
    IC u32 r_u32() { u32 tmp; r(&tmp, sizeof(tmp)); return tmp; };
    IC u16 r_u16() { u16 tmp; r(&tmp, sizeof(tmp)); return tmp; };
    IC u8 r_u8() { u8 tmp; r(&tmp, sizeof(tmp)); return tmp; };
    IC s64 r_s64() { s64 tmp; r(&tmp, sizeof(tmp)); return tmp; };
    IC s32 r_s32() { s32 tmp; r(&tmp, sizeof(tmp)); return tmp; };
    IC s16 r_s16() { s16 tmp; r(&tmp, sizeof(tmp)); return tmp; };
    IC s8 r_s8() { s8 tmp; r(&tmp, sizeof(tmp)); return tmp; };
    IC float r_float() { float tmp; r(&tmp, sizeof(tmp)); return tmp; };
    IC void r_fvector4(Fvector4& v) { r(&v, sizeof(Fvector4)); }
    IC void r_fvector3(Fvector3& v) { r(&v, sizeof(Fvector3)); }
    IC void r_fvector2(Fvector2& v) { r(&v, sizeof(Fvector2)); }
    IC void r_ivector4(Ivector4& v) { r(&v, sizeof(Ivector4)); }
    IC void r_ivector4(Ivector3& v) { r(&v, sizeof(Ivector3)); }
    IC void r_ivector4(Ivector2& v) { r(&v, sizeof(Ivector2)); }
    IC void r_fcolor(Fcolor& v) { r(&v, sizeof(Fcolor)); }

    IC float r_float_q16(float min, float max)
    {
        u16 val = r_u16();
        float A = (float(val)*(max - min)) / 65535.f + min; // floating-point-error possible
        VERIFY((A >= min - XrMath::EPS_S) && (A <= max + XrMath::EPS_S));
        return A;
    }
    IC float r_float_q8(float min, float max)
    {
        u8 val = r_u8();
        float A = (float(val) / 255.0001f) *(max - min) + min; // floating-point-error possible
        VERIFY((A >= min) && (A <= max));
        return A;
    }
    IC float r_angle16() { return r_float_q16(0, XrMath::PI_MUL_2); }
    IC float r_angle8() { return r_float_q8(0, XrMath::PI_MUL_2); }
    IC void r_dir(Fvector& A) { u16 t = r_u16(); XrCompressor::PvDecompress(A, t); }
    IC void r_sdir(Fvector& A)
    {
        u16 t = r_u16();
        float s = r_float();
        XrCompressor::PvDecompress(A, t);
        A.mul(s);
    }
    // Set file pointer to start of chunk data (0 for root chunk)
    IC void rewind() { impl().seek(0); }

	IC bsize find_chunk(u32 ID, BOOL* bCompressed); /*{ return u32(-1); }*/

    IC BOOL r_chunk(u32 ID, void* dest) // чтение XR Chunk'ов (4b-ID,4b-size,??b-data)
    {
		bsize dwSize = ((implementation_type*)this)->find_chunk(ID);
        if (dwSize != 0)
        {
            r(dest, dwSize);
            return TRUE;
        }
        else return FALSE;
    }

    IC BOOL r_chunk_safe(u32 ID, void* dest, bsize dest_size) // чтение XR Chunk'ов (4b-ID,4b-size,??b-data)
    {
		bsize dwSize = ((implementation_type*)this)->find_chunk(ID);
        if (dwSize != 0)
        {
            R_ASSERT(dwSize == dest_size);
            r(dest, dwSize);
            return TRUE;
        }
        else return FALSE;
    }

private:
	static constexpr u32 CFS_CompressMark = 1ul << 31ul;
	
//#define CFS_HeaderChunkID	(666)

	bsize m_last_pos;
};


class XRCORE_API IReader : public IReaderBase < IReader >
{
private:
	static constexpr u32 CFS_CompressMark = 1ul << 31ul;
protected:
	char* data;
	bsize Pos;
	bsize Size;
	bsize iterpos;

public:
	IC IReader()
	{
		Pos = 0;
	}

	virtual ~IReader() {}

	IC IReader(void* _data, bsize _size, bsize _iterpos = 0)
	{
		data = (char*)_data;
		Size = _size;
		Pos = 0;
		iterpos = _iterpos;
	}

protected:
	IC u32 correction(u32 p)
	{
		if (p % 16)
		{
			return ((p % 16) + 1) * 16 - p;
		}
		return 0;
	}

	u32 advance_term_string();

public:
	IC bsize elapsed() const { return Size - Pos; };
	IC bsize tell() const { return Pos; };
	IC void seek(bsize ptr) { Pos = ptr; VERIFY((Pos <= Size) && (Pos >= 0)); };
	IC bsize length() const { return Size; };
	IC void* pointer() const { return &(data[Pos]); };
	IC void advance(bint cnt) { Pos += cnt; VERIFY((Pos <= Size) && (Pos >= 0)); };

public:
	void r(void* p, bsize cnt);

	void r_string(char* dest, u32 tgt_sz);
	void r_string(xr_string& dest);

	void skip_stringZ();

	void r_stringZ(char* dest, u32 tgt_sz);
	void r_stringZ(shared_str& dest);
	void r_stringZ(xr_string& dest);

public:
	void close();

public:
	// поиск XR Chunk'ов - возврат - размер или 0
	IReader* open_chunk(u32 ID);

	// iterators
	IReader* open_chunk_iterator(u32& ID, IReader* previous = NULL); // NULL=first

	bsize find_chunk(u32 ID, BOOL* bCompressed = 0);

private:
	typedef IReaderBase<IReader> inherited;
};
#include "XrFS_impl.h"
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#pragma once
#define FS (*BearCore::FS)
class  XRCORE_API XRayBearReader:public IReader
{
	IC XRayBearReader(BearCore::BearStreamRef< BearCore::BearInputStream> stream_ref) :m_stream(**stream_ref)
	{
		data = (char*)m_stream.Begin();
		Size = m_stream.Size();
		Pos = 0;
		iterpos = 0;
	}
public:
	IC static XRayBearReader* Create(BearCore::BearStreamRef< BearCore::BearInputStream> stream_ref)
	{
		XRayBearReader*result = BearCore::bear_alloc< XRayBearReader>(1);
		new(result)XRayBearReader(stream_ref);
		return result;
	}
	IC static XRayBearReader* Create(const bchar*file)
	{
		XRayBearReader*result = BearCore::bear_alloc< XRayBearReader>(1);
		new(result)XRayBearReader(BearCore::bear_new<BearCore::BearFileStream>(file));
		return result;
	}
	IC static void Destroy(IReader*stream)
	{
		BearCore::bear_delete(static_cast<XRayBearReader*>(stream));
	}
private:
	BearCore::BearMemoryStream m_stream;
};
class XRCORE_API XRayBearWriter :public IWriter
{
	XRayBearWriter(BearCore::BearStreamRef< BearCore::BearOutputStream> stream_ref);
public:
	IC static XRayBearWriter* Create(BearCore::BearStreamRef< BearCore::BearOutputStream> stream_ref)
	{
		XRayBearWriter*result = BearCore::bear_alloc< XRayBearWriter>(1);
		new(result)XRayBearWriter(stream_ref);
		return result;
	}
	IC static XRayBearWriter* Create(const bchar*file)
	{
		XRayBearWriter*result = BearCore::bear_alloc< XRayBearWriter>(1);
		auto fs=	BearCore::bear_new<BearCore::BearFileStream>(file);
		fs->Open(file, fs->M_Write);
		new(result)XRayBearWriter(fs);
		return result;
	}
	IC static void Destroy(IWriter*stream)
	{
		BearCore::bear_delete(static_cast<XRayBearWriter*>(stream));
	}
	virtual ~XRayBearWriter();
	virtual void seek(bsize pos);
	virtual bsize tell();
	virtual void w(const void* ptr, bsize count);
	virtual void flush();
private:
	BearCore::BearStreamRef< BearCore::BearOutputStream> m_stream;

};

class  XRCORE_API XRayBearFileStream :public  IReaderBase < XRayBearFileStream >
{
	XRayBearFileStream(BearCore::BearStreamRef< BearCore::BearInputStream> stream_ref):m_stream(stream_ref){}
public:
	IC static XRayBearFileStream* Create(BearCore::BearStreamRef< BearCore::BearInputStream> stream_ref)
	{
		XRayBearFileStream*result = BearCore::bear_alloc< XRayBearFileStream>(1);
		new(result)XRayBearFileStream(stream_ref);
		return result;
	}
	IC static void Destroy(XRayBearFileStream*stream)
	{
		BearCore::bear_delete(static_cast<XRayBearFileStream*>(stream));
	}
	IC void advance(bsize offset) { m_stream->Seek(offset+ m_stream->Tell()); }
	IC bsize elapsed() const { return  m_stream->Size() - m_stream->Tell(); }
	IC bsize length() const { return  m_stream->Size(); }
	IC void seek(bsize offset) { m_stream->Seek(offset); }
	IC bsize tell() const {return  m_stream->Tell(); }
	IC void r(void* _buffer, bsize buffer_size) { m_stream->Read(_buffer, buffer_size); }
	XRayBearFileStream* open_chunk(const uint32& chunk_id)
	{
		return Create(m_stream->ReadChunkAsInputStream(chunk_id));
	}
	bsize find_chunk(u32 ID, BOOL* bCompressed=0)
	{
		BEAR_ASSERT(bCompressed == 0);
		return m_stream->GoToChunk(ID);
	}
private:
	BearCore::BearStreamRef< BearCore::BearInputStream>m_stream;
};
class CTempReader : public IReader
{
public:
	CTempReader(void *_data, bsize _size, bsize _iterpos) : IReader(_data, _size, _iterpos) {}
	virtual		~CTempReader();
};