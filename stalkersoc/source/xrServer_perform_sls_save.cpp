#include "stdafx.h"
#include "xrserver.h"
#include "xrmessages.h"
#include "xrserver_objects.h"

void xrServer::SLS_Save	(IWriter& fs)
{
	// Generate spawn+update
	NET_Packet		P;
	bsize				position;
	xrS_entities::iterator	I=entities.begin(),E=entities.end();
	for (u32 C=0; I!=E; ++I,++C)
	{
		CSE_Abstract*	E1	= I->second;

		fs.open_chunk		(C);

		// Spawn
		E1->Spawn_Write		(P,TRUE);
		fs.w_u16			(u16(P.B.count));
		fs.w				(P.B.data,P.B.count);

		// Update
		P.w_begin			(M_UPDATE);
		P.w_u16				(E1->ID);
		P.w_chunk_open8		(position);
		E1->UPDATE_Write		(P);
		P.w_chunk_close8	(position);

		fs.w_u16			(u16(P.B.count));
		fs.w				(P.B.data,P.B.count);

		fs.close_chunk		();
	}
}
