#pragma once
class XRCORE_API XrCompressor
{
	friend class XRCORE_API XrCore;
	static void Initialize();
	static void Destroy();
public:
	BEAR_CLASS_STATIC(XrCompressor);
	static bsize  PPMdCompress(void* dest_buffer, const bsize& dest_buffer_size, const void* source_buffer, const bsize& source_buffer_size);
	static bsize  PPMdTrainedCompress(void* dest_buffer, const bsize& dest_buffer_size, const void* source_buffer, const bsize& source_buffer_size, XrPPDStream* tmodel);
	static bsize  PPMdDecompress(void* dest_buffer, const bsize& dest_buffer_size, const void* source_buffer, const bsize& source_buffer_size);
	static bsize  PPMdTrainedDecompress(void* dest_buffer, const bsize& dest_buffer_size, const void* source_buffer, const bsize& source_buffer_size, XrPPDStream* tmodel);

	typedef fastdelegate::FastDelegate<void()> ppmd_yield_callback_t;
	static bsize  PPMdCompressMt(void* dest_buffer, const bsize& dest_buffer_size, const void* source_buffer, const bsize& source_buffer_size, ppmd_yield_callback_t ycb);
	static bsize  PPMdDecompressMt(void* dest_buffer, const bsize& dest_buffer_size, const void* source_buffer, const bsize& source_buffer_size, ppmd_yield_callback_t ycb);

	static uint16 PvCompress(const Fvector& vec);
	static void   PvDecompress(Fvector& vec, uint16 mVec);

	 bsize RtcCompress(void* dst, bsize dst_len, const void* src, bsize src_len);
	 bsize RtcDecompress(void* dst, bsize dst_len, const void* src, bsize src_len);
	 bsize RtcSize(bsize in);

	 bsize Rtc9Compress(void* dst, bsize dst_len, const void* src, bsize src_len);
	 bsize Rtc9Decompress(void* dst, bsize dst_len, const void* src, bsize src_len);
	 bsize Rtc9Size(bsize in);
};