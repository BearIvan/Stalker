#ifndef r_DStreamsH
#define r_DStreamsH
#pragma once

enum
{
	LOCKFLAGS_FLUSH		= D3DLOCK_DISCARD,
	LOCKFLAGS_APPEND	= D3DLOCK_NOOVERWRITE
};

class  ECORE_API _VertexStream
{
private :
	ID3DVertexBuffer*		pVB;
	bsize							mSize;			// size in bytes
	bsize							mPosition;		// position in bytes
	u32							mDiscardID;		// ID of discard - usually for caching
public:
	ID3DVertexBuffer*		old_pVB;
#ifdef DEBUG
	u32							dbg_lock;
#endif
private:
	void						_clear			();
public:
	void						Create			();
	void						Destroy			();
	void						reset_begin		();
	void						reset_end		();

	IC ID3DVertexBuffer*	Buffer()		{ return pVB;			}
	IC u32						DiscardID()		{ return mDiscardID;	}
	IC void						Flush()			{ mPosition=mSize;		}

	void*						Lock			( bsize vl_Count, bsize Stride, bsize& vOffset );
	void						Unlock			( bsize Count, bsize Stride);
	bsize							GetSize()		{ return mSize;}

	_VertexStream();
	~_VertexStream()			{ Destroy();	};
};

class  ECORE_API _IndexStream
{
private :
	ID3DIndexBuffer*		pIB;
	bsize							mSize;		// real size (usually mCount, aligned on 512b boundary)
	bsize							mPosition;
	u32							mDiscardID;
public:
	ID3DIndexBuffer*		old_pIB;
private:
	void						_clear	()
	{
		pIB			= NULL;
		mSize		= 0;
		mPosition	= 0;
		mDiscardID	= 0;
	}
public:
	void						Create			();
	void						Destroy			();
	void						reset_begin		();
	void						reset_end		();

	IC ID3DIndexBuffer*	Buffer()		{ return pIB;			}
	IC u32						DiscardID()		{ return mDiscardID;	}
	void						Flush()			{ mPosition=mSize;		}

	u16*						Lock			( bsize Count, bsize& vOffset );
	void						Unlock			(bsize RealCount);

	_IndexStream()				{ _clear();		};
	~_IndexStream()				{ Destroy();	};
};
#endif
