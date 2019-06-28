#include "stdafx.h"
#pragma hdrstop

extern xrSkin4W* skin4W_func;

struct SKIN_PARAMS {
	LPVOID Dest;
	LPVOID Src;
	u32 Count;
	LPVOID Data;
};

void Skin4W_Stream( LPVOID lpvParams )
{
	#ifdef _GPA_ENABLED	
		TAL_SCOPED_TASK_NAMED( "xrSkin4W_Stream()" );
	#endif // _GPA_ENABLED

	SKIN_PARAMS* sp = (SKIN_PARAMS*) lpvParams;

	vertRender*		D		= (vertRender*) sp->Dest;
	vertBoned4W*	S		= (vertBoned4W*) sp->Src;
	u32				vCount	= sp->Count;
	CBoneInstance*	Bones	= (CBoneInstance*) sp->Data;

	skin4W_func( D , S , vCount, Bones );
}

