#include "stdafx.h"

XRayBearWriter::XRayBearWriter(BearCore::BearStreamRef<BearCore::BearOutputStream> stream_ref):m_stream(stream_ref)
{
}

XRayBearWriter::~XRayBearWriter()
{
}

void XRayBearWriter::seek(u32 pos)
{
	m_stream->Seek(pos);
}

u32 XRayBearWriter::tell()
{
	return m_stream->Tell();
}

void XRayBearWriter::w(const void * ptr, u32 count)
{
	m_stream->Write((void*)ptr, count);
}

void XRayBearWriter::flush()
{
}
