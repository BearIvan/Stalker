#pragma once
class XRCORE_API XrCompressor
{
	friend class XRCORE_API XrCore;
	static void Initialize();
	static void Destroy();
public:
	BEAR_CLASS_STATIC(XrCompressor);

	static uint16 PvCompress(const Fvector& vec);
	static void   PvDecompress(Fvector& vec, uint16 mVec);

	static bsize RtcCompress(void* dst, bsize dst_len, const void* src, bsize src_len);
	static bsize RtcDecompress(void* dst, bsize dst_len, const void* src, bsize src_len);
	static bsize RtcSize(bsize in);

	static bsize Rtc9Compress(void* dst, bsize dst_len, const void* src, bsize src_len);
	static bsize Rtc9Decompress(void* dst, bsize dst_len, const void* src, bsize src_len);
	static bsize Rtc9Size(bsize in);

	static bsize LZWrite(int hf, void* d, bsize size);
	static bsize LZRead(int hf, void*& d, bsize size);

	static void LZCompress(u8** dest, bsize* dest_sz, void* src, bsize src_sz);
	static void LZDecompress(u8** dest, bsize* dest_sz, void* src, bsize src_sz);

}; 