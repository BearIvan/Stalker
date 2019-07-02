#include "StdAfx.h"
#include "coop/game_sv_coop.h"
#include "xrserver_objects_alife_monsters.h"
#include "alife_simulator.h"
#include "alife_object_registry.h"
#include "alife_graph_registry.h"
#include "alife_time_manager.h"
#include "engine/object_broker.h"
#include "gamepersistent.h"
#include "xrServer.h"
#include "engine/x_ray.h"
#include "Level.h"
#include "game_cl_base.h"
#include "game_cl_base_weapon_usage_statistic.h"
#include "Actor.h"
game_sv_Coop::game_sv_Coop()
{
	m_alife_simulator = NULL;
	m_type = GAME_COOP;
	m_FirstPlayer = 0;
}

game_sv_Coop::~game_sv_Coop()
{
	if(m_alife_simulator)	delete_data(m_alife_simulator);

}
bool GAlifeCOOP = false;
void game_sv_Coop::Create(shared_str & options)
{
	inherited::Create(options);
	if (strstr(*options, "/alife"))
	{
		GAlifeCOOP = true;
		m_alife_simulator = xr_new<CALifeSimulator>(&server(), &options);
	}
	auto level = level_name(options);
	options.printf("%s/%s", level.c_str(), options.c_str());

}

void game_sv_Coop::OnCreate(u16 id_who)
{
	if (!ai().get_alife())
		return;

	CSE_Abstract			*e_who = get_entity_from_eid(id_who);
	VERIFY(e_who);
	if (!e_who->m_bALifeControl)
		return;

	CSE_ALifeObject			*alife_object = smart_cast<CSE_ALifeObject*>(e_who);
	if (!alife_object)
		return;

	alife_object->m_bOnline = true;

	if (alife_object->ID_Parent != 0xffff) {
		CSE_ALifeDynamicObject			*parent = ai().alife().objects().object(alife_object->ID_Parent, true);
		if (parent) {
			CSE_ALifeTraderAbstract		*trader = smart_cast<CSE_ALifeTraderAbstract*>(parent);
			if (trader)
				alife().create(alife_object);
			else
				alife_object->m_bALifeControl = false;
		}
		else
			alife_object->m_bALifeControl = false;
	}
	else
		alife().create(alife_object);
}

BOOL game_sv_Coop::OnTouch(u16 eid_who, u16 eid_what, BOOL bForced)
{
	CSE_Abstract*		e_who = get_entity_from_eid(eid_who);		VERIFY(e_who);
	CSE_Abstract*		e_what = get_entity_from_eid(eid_what);	VERIFY(e_what);

	if (ai().get_alife()) {
		CSE_ALifeInventoryItem	*l_tpALifeInventoryItem = smart_cast<CSE_ALifeInventoryItem*>	(e_what);
		CSE_ALifeDynamicObject	*l_tpDynamicObject = smart_cast<CSE_ALifeDynamicObject*>	(e_who);

		if (
			l_tpALifeInventoryItem &&
			l_tpDynamicObject &&
			ai().alife().graph().level().object(l_tpALifeInventoryItem->base()->ID, true) &&
			ai().alife().objects().object(e_who->ID, true) &&
			ai().alife().objects().object(e_what->ID, true)
			)
			alife().graph().attach(*e_who, l_tpALifeInventoryItem, l_tpDynamicObject->m_tGraphID, false, false);
#ifdef DEBUG
		else
			if (psAI_Flags.test(aiALife)) {
				Msg("Cannot attach object [%s][%s][%d] to object [%s][%s][%d]", e_what->name_replace(), *e_what->s_name, e_what->ID, e_who->name_replace(), *e_who->s_name, e_who->ID);
			}
#endif
	}
	return TRUE;
}

void game_sv_Coop::OnDetach(u16 eid_who, u16 eid_what)
{
	if (ai().get_alife())
	{
		CSE_Abstract*		e_who = get_entity_from_eid(eid_who);		VERIFY(e_who);
		CSE_Abstract*		e_what = get_entity_from_eid(eid_what);	VERIFY(e_what);

		CSE_ALifeInventoryItem *l_tpALifeInventoryItem = smart_cast<CSE_ALifeInventoryItem*>(e_what);
		if (!l_tpALifeInventoryItem)
			return;

		CSE_ALifeDynamicObject *l_tpDynamicObject = smart_cast<CSE_ALifeDynamicObject*>(e_who);
		if (!l_tpDynamicObject)
			return;

		if (
			ai().alife().objects().object(e_who->ID, true) &&
			!ai().alife().graph().level().object(l_tpALifeInventoryItem->base()->ID, true) &&
			ai().alife().objects().object(e_what->ID, true)
			)
			alife().graph().detach(*e_who, l_tpALifeInventoryItem, l_tpDynamicObject->m_tGraphID, false, false);
		else {
			if (!ai().alife().objects().object(e_what->ID, true)) {
				u16				id = l_tpALifeInventoryItem->base()->ID_Parent;
				l_tpALifeInventoryItem->base()->ID_Parent = 0xffff;

				CSE_ALifeDynamicObject *dynamic_object = smart_cast<CSE_ALifeDynamicObject*>(e_what);
				VERIFY(dynamic_object);
				dynamic_object->m_tNodeID = l_tpDynamicObject->m_tNodeID;
				dynamic_object->m_tGraphID = l_tpDynamicObject->m_tGraphID;
				dynamic_object->m_bALifeControl = true;
				dynamic_object->m_bOnline = true;
				alife().create(dynamic_object);
				l_tpALifeInventoryItem->base()->ID_Parent = id;
			}
#ifdef DEBUG
			else
				if (psAI_Flags.test(aiALife)) {
					Msg("Cannot detach object [%s][%s][%d] from object [%s][%s][%d]", l_tpALifeInventoryItem->base()->name_replace(), *l_tpALifeInventoryItem->base()->s_name, l_tpALifeInventoryItem->base()->ID, l_tpDynamicObject->base()->name_replace(), *l_tpDynamicObject->base()->s_name, l_tpDynamicObject->ID);
				}
#endif
		}
	}
}

void game_sv_Coop::Update()
{

	inherited::Update();
}

ALife::_TIME_ID game_sv_Coop::GetGameTime()
{
	if (ai().get_alife() && ai().alife().initialized())
		return(ai().alife().time_manager().game_time());
	else
		return(inherited::GetGameTime());
}

float game_sv_Coop::GetGameTimeFactor()
{
	if (ai().get_alife() && ai().alife().initialized())
		return(ai().alife().time_manager().time_factor());
	else
		return(inherited::GetGameTimeFactor());
}

void game_sv_Coop::SetGameTimeFactor(const float fTimeFactor)
{
	if (ai().get_alife() && ai().alife().initialized())
		return(alife().time_manager().set_time_factor(fTimeFactor));
	else
		return(inherited::SetGameTimeFactor(fTimeFactor));
}

ALife::_TIME_ID game_sv_Coop::GetEnvironmentGameTime()
{
	return(inherited::GetGameTime());
}

float game_sv_Coop::GetEnvironmentGameTimeFactor()
{
	return (inherited::GetGameTimeFactor());
}

void game_sv_Coop::SetEnvironmentGameTimeFactor(const float fTimeFactor)
{
	//return
}

bool game_sv_Coop::change_level(NET_Packet & net_packet, ClientID sender)
{
	if (ai().get_alife())
		return					(alife().change_level(net_packet));
	else
		return					(true);
}

void game_sv_Coop::save_game(NET_Packet & net_packet, ClientID sender)
{
	if (!ai().get_alife())
		return;

	alife().save(net_packet);
}

bool game_sv_Coop::load_game(NET_Packet & net_packet, ClientID sender)
{
	if (!ai().get_alife())
		return					(inherited::load_game(net_packet, sender));
	shared_str						game_name;
	net_packet.r_stringZ(game_name);
	return						(alife().load_game(*game_name, true));
}

void game_sv_Coop::reload_game(NET_Packet & net_packet, ClientID sender)
{
}

void game_sv_Coop::switch_distance(NET_Packet & net_packet, ClientID sender)
{
	if (!ai().get_alife())
		return;

	alife().set_switch_distance(net_packet.r_float());
}

void game_sv_Coop::teleport_object(NET_Packet & net_packet, u16 id)
{
	if (!ai().get_alife())
		return;

	GameGraph::_GRAPH_ID		game_vertex_id;
	u32						level_vertex_id;
	Fvector					position;

	net_packet.r(&game_vertex_id, sizeof(game_vertex_id));
	net_packet.r(&level_vertex_id, sizeof(level_vertex_id));
	net_packet.r_vec3(position);

	alife().teleport_object(id, game_vertex_id, level_vertex_id, position);
}

void game_sv_Coop::add_restriction(NET_Packet & packet, u16 id)
{
	if (!ai().get_alife())
		return;

	ALife::_OBJECT_ID		restriction_id;
	packet.r(&restriction_id, sizeof(restriction_id));

	RestrictionSpace::ERestrictorTypes	restriction_type;
	packet.r(&restriction_type, sizeof(restriction_type));

	alife().add_restriction(id, restriction_id, restriction_type);
}

void game_sv_Coop::remove_restriction(NET_Packet & packet, u16 id)
{
	if (!ai().get_alife())
		return;

	ALife::_OBJECT_ID		restriction_id;
	packet.r(&restriction_id, sizeof(restriction_id));

	RestrictionSpace::ERestrictorTypes	restriction_type;
	packet.r(&restriction_type, sizeof(restriction_type));

	alife().remove_restriction(id, restriction_id, restriction_type);
}

void game_sv_Coop::remove_all_restrictions(NET_Packet & packet, u16 id)
{
	if (!ai().get_alife())
		return;

	RestrictionSpace::ERestrictorTypes	restriction_type;
	packet.r(&restriction_type, sizeof(restriction_type));

	alife().remove_all_restrictions(id, restriction_type);
}

void game_sv_Coop::sls_default()
{
	alife().update_switch();
}

shared_str game_sv_Coop::level_name(const shared_str & server_options) const
{
	if (!ai().get_alife())
		return				(inherited::level_name(server_options));
	return					(alife().level_name());
}

void game_sv_Coop::on_death(CSE_Abstract * e_dest, CSE_Abstract * e_src)
{
	inherited::on_death(e_dest, e_src);

	if (!ai().get_alife())
		return;

	alife().on_death(e_dest, e_src);
}

void game_sv_Coop::restart_simulator(LPCSTR saved_game_name)
{
	shared_str				&options = *alife().server_command_line();

	delete_data(m_alife_simulator);
	server().clear_ids();

	BearCore::BearString::Copy(g_pGamePersistent->m_game_params.m_game_or_spawn, saved_game_name);
	BearCore::BearString::Copy(g_pGamePersistent->m_game_params.m_new_or_load, "load");

	pApp->LoadBegin();
	m_alife_simulator = xr_new<CALifeSimulator>(&server(), &options);
	g_pGamePersistent->LoadTitle("st_client_synchronising");
	Device.PreCache(60, true, true);
	pApp->LoadEnd();
}
void game_sv_Coop::OnPlayerConnectFinished(ClientID id_who)
{
	
	if (m_FirstPlayer == 0)
	{
		m_FirstPlayer = m_server->ID_to_client(id_who);;
		game_PlayerState*	ps_who = m_FirstPlayer->ps;
		SetSkin(m_FirstPlayer->owner, ps_who->skin);
		OnPlayerEnteredGame(id_who);
		return;
	}
	SpawnPlayer(id_who);
	return;
}
void game_sv_Coop::SetSkin(CSE_Abstract * E,  u16 ID)
{
	if (!E) return;
	//-------------------------------------------
	CSE_Visual* pV = smart_cast<CSE_Visual*>(E);
	if (!pV) return;
	//-------------------------------------------
	BearCore::BearStringPath SkinName;
	BearCore::BearString::Copy(SkinName, pSettings->r_string("coop", "other_player_model"));
	BearCore::BearString::Contact(SkinName, TEXT(".ogf"));

	int len = xr_strlen(SkinName);
	R_ASSERT2(len < 64, "Skin Name is too LONG!!!");
	pV->set_visual(SkinName);
}
void game_sv_Coop::SpawnPlayer(ClientID id_who)
{
	const char*N = "actor";
	xrClientData* CL = m_server->ID_to_client(id_who);
	CL->net_PassUpdates = TRUE;


	game_PlayerState*	ps_who = CL->ps;
	ps_who->setFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD);

	// Spawn "actor"
	CSE_Abstract*	E = spawn_begin(N);													// create SE

	E->set_name_replace(get_name_id(id_who));					// name

	E->s_flags.assign(M_SPAWN_OBJECT_LOCAL | M_SPAWN_OBJECT_ASPLAYER);	// flags

	CSE_ALifeCreatureActor	*pA = smart_cast<CSE_ALifeCreatureActor*>(E);

	R_ASSERT2(pA, "Respawned Client is not Actor nor Spectator");
	//pA->s_team = u8(ps_who->team);
//	assign_RP(pA, ps_who);
	SetSkin(E, ps_who->skin);
	ps_who->resetFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD);

//	if (!ps_who->RespawnTime)
	OnPlayerEnteredGame(id_who);

	ps_who->RespawnTime = Device.dwTimeGlobal;

	Game().m_WeaponUsageStatistic->OnPlayerSpawned(ps_who);

	/*if (pA)
	{
		pA->s_team = u8(ps_who->team);
		assign_RP(pA, ps_who);
		SetSkin(E, pA->s_team, ps_who->skin);
		ps_who->resetFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD);

		if (!ps_who->RespawnTime)
			OnPlayerEnteredGame(id);

		ps_who->RespawnTime = Device.dwTimeGlobal;

		Game().m_WeaponUsageStatistic->OnPlayerSpawned(ps_who);
	}
	else
		if (pS)
		{
			Fvector Pos, Angle;
			if (!GetPosAngleFromActor(id, Pos, Angle)) assign_RP(E, ps_who);
			else
			{
				E->o_Angle.set(Angle);
				E->o_Position.set(Pos);
			}
		};
	*/
	Msg("* %s respawned as %s", get_name_id(id_who), (0 == pA) ? "spectator" : "actor");
	ToActor(pA);
	spawn_end(E, id_who);

	ps_who->SetGameID(CL->owner->ID);

	signal_Syncronize();
}
void game_sv_Coop::ToActor(CSE_ALifeCreatureActor * E)
{
	CSE_ALifeCreatureActor*GActor = smart_cast<CSE_ALifeCreatureActor*>(m_FirstPlayer->owner);
	BEAR_ASSERT(GActor);
	E->m_tGraphID = GActor->m_tGraphID;
	E->m_tNodeID = GActor->m_tNodeID;
	E->o_Position = GActor->o_Position;
}
void game_sv_Coop::OnPlayerEnteredGame(ClientID id_who)
{
	xrClientData* xrCData = m_server->ID_to_client(id_who);
	if (!xrCData) return;

	NET_Packet			P;
	GenerateGameMessage(P);
	P.w_u32(GAME_EVENT_PLAYER_ENTERED_GAME);
	P.w_stringZ(xrCData->name.c_str());
	u_EventSend(P);
}