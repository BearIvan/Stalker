#if !defined(AFX_NET_COMPRESSOR_H__21E1ED1C_BF92_4BF0_94A8_18A27486EBFD__INCLUDED_)
#define AFX_NET_COMPRESSOR_H__21E1ED1C_BF92_4BF0_94A8_18A27486EBFD__INCLUDED_
#pragma once

#include "tools/Utilities/XrSyncronize.h"

class	XRNETSERVER_API  NET_Compressor
{
	xrCriticalSection		CS;



public:
						NET_Compressor		();
						~NET_Compressor		();

	u16					compressed_size		(const bsize &count);
	u16					Compress			(BYTE* dest, const bsize &dest_size, BYTE* src, const bsize &count);	// return size of compressed
	u16					Decompress			(BYTE* dest, const bsize &dest_size, BYTE* src, const bsize &count);	// return size of compressed
	void				DumpStats			(bool brief);
};

#endif // !defined(AFX_NET_COMPRESSOR_H__21E1ED1C_BF92_4BF0_94A8_18A27486EBFD__INCLUDED_)

