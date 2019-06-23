#include "stdafx.h"


// upper 3 bits
#define pvSIGN_MASK 0xe000
#define pvXSIGN_MASK 0x8000
#define pvYSIGN_MASK 0x4000
#define pvZSIGN_MASK 0x2000

// middle 6 bits - xbits
#define pvTOP_MASK 0x1f80

// lower 7 bits - ybits
#define pvBOTTOM_MASK 0x007f

// static lookup table for unit vector3 decompression
float pvUVAdjustment[0x2000];

void XrCompressor::Initialize()
{
	for (int idx = 0; idx < 0x2000; idx++)
	{
		long xbits = idx >> 7;
		long ybits = idx & pvBOTTOM_MASK;

		// map the numbers back to the triangle (0,0)-(0,127)-(127,0)
		if ((xbits + ybits) >= 127)
		{
			xbits = 127 - xbits;
			ybits = 127 - ybits;
		}

		// convert to 3D vectors
		float x = float(xbits);
		float y = float(ybits);
		float z = float(126 - xbits - ybits);

		// calculate the amount of normalization required
		pvUVAdjustment[idx] = XrMath::rsqrt(y*y + z * z + x * x);
	}
}
extern XrPPDStream* trained_model;

void XrCompressor::Destroy()
{
	if (trained_model)
	{
		void* buffer = trained_model->buffer();
		BearCore::bear_free(buffer);
		BearCore::bear_delete(trained_model);
	}
}

#include "PPMD/PPMd.h"
const u32 suballocator_size = 32;
const u32 order_model = 8;
const MR_METHOD restoration_method_cut_off = MRM_RESTART;

typedef XrPPDStream stream;


void _stdcall PrintInfo(_PPMD_FILE* DecodedFile, _PPMD_FILE* EncodedFile)
{
}



static LONG PPMd_Locked = 0;

static inline void
PPMd_Lock()
{
	while (::InterlockedExchange(&PPMd_Locked, 1))
		::Sleep(0);
}

static inline void
PPMd_Unlock()
{
	::InterlockedExchange(&PPMd_Locked, 0);
}


void ppmd_initialize()
{
	if (trained_model)
		trained_model->rewind();

	static bool initialized = false;
	if (initialized)
		return;

	initialized = true;
	if (StartSubAllocator(suballocator_size))
		return;

	exit(-1);
}

bsize XrCompressor::PPMdCompress(void * dest_buffer, const bsize & dest_buffer_size, const void * source_buffer, const bsize & source_buffer_size)
{
	PPMd_Lock();
	ppmd_initialize();

	stream source(source_buffer, source_buffer_size);
	stream dest(dest_buffer, dest_buffer_size);
	EncodeFile(&dest, &source, order_model, restoration_method_cut_off);

	PPMd_Unlock();
	return (dest.tell() + 1);
}

bsize XrCompressor::PPMdTrainedCompress(void * dest_buffer, const bsize & dest_buffer_size, const void * source_buffer, const bsize & source_buffer_size, XrPPDStream * tmodel)
{
	PPMd_Lock();

	stream* old_stream = trained_model;
	trained_model = tmodel;

	ppmd_initialize();

	stream source(source_buffer, source_buffer_size);
	stream dest(dest_buffer, dest_buffer_size);
	EncodeFile(&dest, &source, order_model, restoration_method_cut_off);

	trained_model = old_stream;

	PPMd_Unlock();
	return (dest.tell() + 1);
}

bsize XrCompressor::PPMdDecompress(void * dest_buffer, const bsize & dest_buffer_size, const void * source_buffer, const bsize & source_buffer_size)
{
	PPMd_Lock();
	ppmd_initialize();

	stream source(source_buffer, source_buffer_size);
	stream dest(dest_buffer, dest_buffer_size);
	DecodeFile(&dest, &source, order_model, restoration_method_cut_off);

	PPMd_Unlock();
	return (dest.tell());
}

bsize XrCompressor::PPMdTrainedDecompress(void * dest_buffer, const bsize & dest_buffer_size, const void * source_buffer, const bsize & source_buffer_size, XrPPDStream * tmodel)
{
	PPMd_Lock();

	stream* old_stream = trained_model;
	trained_model = tmodel;

	ppmd_initialize();



	stream source(source_buffer, source_buffer_size);
	stream dest(dest_buffer, dest_buffer_size);
	DecodeFile(&dest, &source, order_model, restoration_method_cut_off);

	trained_model = old_stream;

	PPMd_Unlock();
	return (dest.tell());
}
constexpr u32 compress_chunk_size = 100 * 1024; //100 kb
bsize XrCompressor::PPMdCompressMt(void * dest_buffer, const bsize & dest_buffer_size, const void * source_buffer, const bsize & source_buffer_size, ppmd_yield_callback_t ycb)
{
	PPMd_Lock();
	ppmd_initialize();

	u32 rest_src_buff = source_buffer_size;
	u8 const* src_buff_chunk = static_cast<u8 const*>(source_buffer);

	u8* curr_dst_buff = static_cast<u8*>(dest_buffer);
	u32 dst_buff_size = dest_buffer_size;

	u32 result_size = 0;

	while (rest_src_buff)
	{
		u32 to_compress = rest_src_buff > compress_chunk_size ?
			compress_chunk_size : rest_src_buff;

		stream source(src_buff_chunk, to_compress);
		stream dest(curr_dst_buff, dst_buff_size);
		EncodeFile(&dest, &source, order_model, restoration_method_cut_off);

		u32 dst_encoded = dest.tell();
		curr_dst_buff += dst_encoded;
		R_ASSERT(dest_buffer_size >= dst_encoded);
		dst_buff_size -= dst_encoded;
		result_size += dst_encoded;

		src_buff_chunk += to_compress;
		rest_src_buff -= to_compress;
		if (ycb)
			ycb();
	}

	PPMd_Unlock();
	return result_size;
}

bsize XrCompressor::PPMdDecompressMt(void * dest_buffer, const bsize & dest_buffer_size, const void * source_buffer, const bsize & source_buffer_size, ppmd_yield_callback_t ycb)
{
	PPMd_Lock();
	ppmd_initialize();

	u32 rest_src_buff = source_buffer_size;
	u8 const* src_buff_chunk = static_cast<u8 const*>(source_buffer);

	u8* curr_dst_buff = static_cast<u8*>(dest_buffer);
	u32 dst_buff_size = dest_buffer_size;

	u32 result_size = 0;

	while (rest_src_buff)
	{
		stream source(src_buff_chunk, rest_src_buff);
		stream dest(curr_dst_buff, dst_buff_size);
		DecodeFile(&dest, &source, order_model, restoration_method_cut_off);

		u32 src_decoded = source.tell();
		src_buff_chunk += src_decoded;
		R_ASSERT(rest_src_buff >= src_decoded);
		rest_src_buff -= src_decoded;

		u32 unpacked = dest.tell();
		curr_dst_buff += unpacked;
		R_ASSERT(dst_buff_size >= unpacked);
		dst_buff_size -= unpacked;

		result_size += unpacked;

		if (ycb)
			ycb();
	}

	PPMd_Unlock();
	return result_size;
}

uint16 XrCompressor::PvCompress(const Fvector & vec)
{
	Fvector tmp = vec;

	// input vector3 does not have to be unit length
	u16 mVec = 0;

	if (tmp.x < 0) { mVec |= pvXSIGN_MASK; tmp.x = -tmp.x; }
	if (tmp.y < 0) { mVec |= pvYSIGN_MASK; tmp.y = -tmp.y; }
	if (tmp.z < 0) { mVec |= pvZSIGN_MASK; tmp.z = -tmp.z; }

	// project the normal onto the plane that goes through
	// X0=(1,0,0),Y0=(0,1,0),Z0=(0,0,1).

	// on that plane we choose an (projective!) coordinate system
	// such that X0->(0,0), Y0->(126,0), Z0->(0,126),(0,0,0)->Infinity

	// a little slower... old pack was 4 multiplies and 2 adds.
	// This is 2 multiplies, 2 adds, and a divide....
	float w = 126.0f / (tmp.x + tmp.y + tmp.z);
	int xbits = XrMath::iFloor(tmp.x * w);
	int ybits = XrMath::iFloor(tmp.y * w);

	/*
	VERIFY( xbits < 127 );
	VERIFY( xbits >= 0 );
	VERIFY( ybits < 127 );
	VERIFY( ybits >= 0 );
	*/

	// Now we can be sure that 0<=xp<=126, 0<=yp<=126, 0<=xp+yp<=126

	// however for the sampling we want to transform this triangle
	// into a rectangle.
	if (xbits >= 64)
	{
		xbits = 127 - xbits;
		ybits = 127 - ybits;
	}

	// now we that have xp in the range (0,127) and yp in the range (0,63),
	// we can pack all the bits together
	mVec |= (xbits << 7);
	mVec |= ybits;

	return mVec;
}

void XrCompressor::PvDecompress(Fvector & vec, uint16 mVec)
{// if we do a straightforward backward transform
    // we will get points on the plane X0,Y0,Z0
    // however we need points on a sphere that goes through these points.
    // therefore we need to adjust x,y,z so that x^2+y^2+z^2=1

    // by normalizing the vector3. We have already precalculated the amount
    // by which we need to scale, so all we do is a table lookup and a
    // multiplication

    // get the x and y bits
	int xbits = ((mVec & pvTOP_MASK) >> 7);
	int ybits = (mVec & pvBOTTOM_MASK);

	// map the numbers back to the triangle (0,0)-(0,126)-(126,0)
	if ((xbits + ybits) >= 127)
	{
		xbits = 127 - xbits;
		ybits = 127 - ybits;
	}

	// do the inverse transform and normalization
	// costs 3 extra multiplies and 2 subtracts. No big deal.
	float uvadj = pvUVAdjustment[mVec & ~pvSIGN_MASK];
	vec.x = uvadj * float(xbits);
	vec.y = uvadj * float(ybits);
	vec.z = uvadj * float(126 - xbits - ybits);

	// set all the sign bits
	if (mVec & pvXSIGN_MASK) vec.x = XrMath::min(-vec.x, vec.x);
	if (mVec & pvYSIGN_MASK) vec.y = XrMath::min(-vec.y, vec.y);
	if (mVec & pvZSIGN_MASK) vec.z = XrMath::min(-vec.z, vec.z);
}

bsize XrCompressor::RtcCompress(void * dst, bsize dst_len, const void * src, bsize src_len)
{
	return BearCore::BearLZO::Compressor(src, src_len, dst, dst_len);
}

bsize XrCompressor::RtcDecompress(void * dst, bsize dst_len, const void * src, bsize src_len)
{
	return BearCore::BearLZO::Decompressor(src, src_len, dst, dst_len);
}

bsize XrCompressor::RtcSize(bsize in)
{
	VERIFY(in);
	return in + in / 64 + 16 + 3;

}

bsize XrCompressor::Rtc9Compress(void * dst, bsize dst_len, const void * src, bsize src_len)
{
	return BearCore::BearLZO::Compressor9(src, src_len, dst, dst_len);
}

bsize XrCompressor::Rtc9Decompress(void * dst, bsize dst_len, const void * src, bsize src_len)
{
	return BearCore::BearLZO::Decompressor9(src, src_len, dst, dst_len);
}

bsize XrCompressor::Rtc9Size(bsize in)
{
	VERIFY(in);
	return in + in / 64 + 16 + 3;
}
