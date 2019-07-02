#include "stdafx.h"
#include "Level.h"



void CLevel::SendClientDigestToServer()
{
	/*string128 tmp_digest;
	NET_Packet P;
	P.w_begin			(M_SV_DIGEST);
	m_client_digest		= ComputeClientDigest(tmp_digest, true);
	P.w_stringZ			(m_client_digest);
	P.w_stringZ			(ComputeClientDigest(tmp_digest, false));
	SecureSend			(P, net_flags(TRUE, TRUE, TRUE, TRUE));*/
}