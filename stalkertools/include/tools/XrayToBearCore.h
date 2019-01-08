#pragma once
#define FS (*BearCore::FS)
class  XRCORE_API XRayBearReader:public IReader
{
	IC XRayBearReader(BearCore::BearStreamRef< BearCore::BearInputStream> stream_ref) :m_stream(**stream_ref), IReader(m_stream.Begin(), m_stream.Size())
	{}
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
		BearCore::bear_free(static_cast<XRayBearReader*>(stream));
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
		BearCore::bear_free(static_cast<XRayBearWriter*>(stream));
	}
	virtual ~XRayBearWriter();
	virtual void seek(u32 pos);
	virtual u32 tell();
	virtual void w(const void* ptr, u32 count);
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
		BearCore::bear_free(static_cast<XRayBearFileStream*>(stream));
	}
	IC void advance(u32 offset) { m_stream->Seek(offset+ m_stream->Tell()); }
	IC u32 elapsed() const { return  m_stream->Size() - m_stream->Tell(); }
	IC u32 length() const { return  m_stream->Size(); }
	IC void seek(u32 offset) { m_stream->Seek(offset); }
	IC u32 tell() const {return  m_stream->Tell(); }
	IC void r(void* _buffer, u32 buffer_size) { m_stream->Read(_buffer, buffer_size); }
	XRayBearFileStream* open_chunk(const u32& chunk_id) 
	{
		return Create(m_stream->ReadChunkAsInputStream(chunk_id));
	}
	u32 find_chunk(u32 ID, BOOL* bCompressed=0)
	{
		BEAR_ASSERT(bCompressed == 0);
		return m_stream->GoToChunk(ID);
	}
private:
	BearCore::BearStreamRef< BearCore::BearInputStream>m_stream;
};