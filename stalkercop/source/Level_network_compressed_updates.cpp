#include "stdafx.h"
#include "Level.h"
#include "physics/iphworld.h"
#include "xrServer_updates_compressor.h"

void CLevel::ProcessCompressedUpdate(NET_Packet& P, u8 const compress_type)
{
	BEAR_ASSERT(false); 
	return;
	/*
	NET_Packet	uncompressed_packet;
	u16 next_size;
	P.r_u16(next_size);
	Device.Statistic->netClientCompressor.Begin();
	while (next_size)
	{
		
		NODEFAULT;
		
		VERIFY2(uncompressed_packet.B.count <= sizeof(uncompressed_packet.B.data),
			"stack owerflow after decompressing");

		P.r_seek(P.r_tell() + next_size);
		uncompressed_packet.r_seek(0);
		Objects.net_Import(&uncompressed_packet);
		P.r_u16(next_size);
	}
	Device.Statistic->netClientCompressor.End();

	if (OnClient()) UpdateDeltaUpd(timeServer());
	IClientStatistic pStat = Level().GetStatistic();
	u32 dTime = 0;
	
	if ((Level().timeServer() + pStat.getPing()) < P.timeReceive)
	{
		dTime = pStat.getPing();
	}
	else
	{
		dTime = Level().timeServer() - P.timeReceive + pStat.getPing();
	}
	u32 NumSteps = physics_world()->CalcNumSteps(dTime);
	SetNumCrSteps(NumSteps);*/
}

void CLevel::init_compression()
{
	/*compression::init_lzo(
		m_lzo_working_memory,
		m_lzo_working_buffer,
		m_lzo_dictionary
	);*/
}

void CLevel::deinit_compression()
{
	
	/*if (m_lzo_working_buffer)
	{
		VERIFY(m_lzo_dictionary.data);
		compression::deinit_lzo(m_lzo_working_buffer, m_lzo_dictionary);
	}*/
}