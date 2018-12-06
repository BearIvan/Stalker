#include "stdafx.h"
#include "xrserver.h"
#include "xrmessages.h"
#pragma TODO("Миша: файл был изменён под зп!!!")
void xrServer::Perform_game_export	()
{
	if (!net_players.ClientsCount())		return;

	// Broadcase game state to every body
	// But it is slightly different view for each "player"

	struct  tmp{
		NET_Packet		P;
		u32				mode = net_flags(TRUE, TRUE);
		xrServer *server ;
		tmp(xrServer *s) :server(s) {}
		void operator()(IClient*client)
		{
			ClientID ID = client->ID;
			xrClientData*	CL = (xrClientData*)client;
			if (!CL->net_Accepted) return;
			P.w_begin(M_SV_CONFIG_GAME);
			server->game->net_Export_State(P, ID);
			server->SendTo(ID, P, mode);
		}
	}functor(this);
	 
	net_players.ForEachClientDo(functor);

	// Game config (all, info includes _new_ player)
	/*csPlayers.Enter		();
	for (u32 client=0; client<net_Players.size(); ++client)
	{
		ClientID ID						= net_Players[client]->ID;
		xrClientData*	CL				= (xrClientData*)net_Players[client];
		if (!CL->net_Accepted) continue;
		P.w_begin						(M_SV_CONFIG_GAME);
		game->net_Export_State			(P,ID);
		SendTo							(ID,P,mode);
	}
	csPlayers.Leave		();*/

	

	game->sv_force_sync	= FALSE;
}

void xrServer::Export_game_type(IClient* CL)
{
	NET_Packet			P;
	u32					mode = net_flags(TRUE,TRUE);
	//csPlayers.Enter		();

	P.w_begin			(M_SV_CONFIG_NEW_CLIENT);
	P.w_stringZ			(game->type_name() );
	SendTo				(CL->ID,P,mode);

	//csPlayers.Leave		();
}

