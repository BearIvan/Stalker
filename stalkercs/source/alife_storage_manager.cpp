////////////////////////////////////////////////////////////////////////////
//	Module 		: alife_storage_manager.cpp
//	Created 	: 25.12.2002
//  Modified 	: 12.05.2004
//	Author		: Dmitriy Iassenev
//	Description : ALife Simulator storage manager
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "alife_storage_manager.h"
#include "alife_simulator_header.h"
#include "alife_time_manager.h"
#include "alife_spawn_registry.h"
#include "alife_object_registry.h"
#include "alife_graph_registry.h"
#include "alife_group_registry.h"
#include "alife_registry_container.h"
#include "xrserver.h"
#include "level.h"
#include "engine/x_ray.h"
#include "saved_game_wrapper.h"
#include "string_table.h"
#include "engine/igame_persistent.h"
#include "autosave_manager.h"

XRCORE_API string_path g_bug_report_file;

using namespace ALife;

extern string_path g_last_saved_game;

CALifeStorageManager::~CALifeStorageManager	()
{
	*g_last_saved_game			= 0;
}

void CALifeStorageManager::save	(LPCSTR save_name, bool update_name)
{
	BearString::Copy					(g_last_saved_game,sizeof(g_last_saved_game),save_name);

	string_path					save;
	BearString::Copy					(save,m_save_name);
	if (save_name) {
		strconcat				(sizeof(m_save_name),m_save_name,save_name,SAVE_EXTENSION);
	}
	else {
		if (!xr_strlen(m_save_name)) {
			Log					("There is no file name specified!");
			return;
		}
	}

	u32							source_count;
	u32							dest_count;
	void						*dest_data;
	{
		CMemoryWriter			stream;
		header().save			(stream);
		time_manager().save		(stream);
		spawns().save			(stream);
		objects().save			(stream);
		registry().save			(stream);

		source_count			= stream.tell();
		void					*source_data = stream.pointer();
		dest_count				=XrCompressor::RtcSize(source_count);
		dest_data				= xr_malloc(dest_count);
		dest_count				= XrCompressor::RtcCompress(dest_data,dest_count,source_data,source_count);
	}

	IWriter						*writer =XRayBearWriter::Create( FS.Write("%saves%", m_save_name,0));
	writer->w_u32				(u32(-1));
	writer->w_u32				(ALIFE_VERSION);
	
	writer->w_u32				(source_count);
	writer->w					(dest_data,dest_count);
	xr_free						(dest_data);
	XRayBearWriter::Destroy(writer);
#ifdef DEBUG
	Msg							("* Game %s is successfully saved to file '%s' (%d bytes compressed to %d)",m_save_name,m_save_name,source_count,dest_count + 4);
#else // DEBUG
	Msg							("* Game %s is successfully saved to file '%s'",m_save_name,m_save_name);
#endif // DEBUG

	if (!update_name)
		BearString::Copy					(m_save_name,save);
}

void CALifeStorageManager::load	(void *buffer, const u32 &buffer_size, LPCSTR file_name)
{
	IReader						source(buffer,buffer_size);
	header().load				(source);
	time_manager().load			(source);
	spawns().load				(source,file_name);
	graph().on_load				();
	objects().load				(source);

	VERIFY						(can_register_objects());
	can_register_objects		(false);
	CALifeObjectRegistry::OBJECT_REGISTRY::iterator	B = objects().objects().begin();
	CALifeObjectRegistry::OBJECT_REGISTRY::iterator	E = objects().objects().end();
	CALifeObjectRegistry::OBJECT_REGISTRY::iterator	I;
	for (I = B; I != E; ++I) {
		ALife::_OBJECT_ID		id = (*I).second->ID;
		(*I).second->ID			= server().PerformIDgen(id);
		VERIFY					(id == (*I).second->ID);
		register_object			((*I).second,false);
	}

	registry().load				(source);

	can_register_objects		(true);

	for (I = B; I != E; ++I)
		(*I).second->on_register();

	if (!g_pGameLevel)
		return;

	Level().autosave_manager().on_game_loaded	();
}

bool CALifeStorageManager::load	(LPCSTR save_name)
{
	BearTimer						timer;
	timer.restart					();
	string256					save;
	BearString::Copy						(save,m_save_name);
	if (!save_name) {
		if (!xr_strlen(m_save_name))
			R_ASSERT2			(false,"There is no file name specified!");
	}
	else
		strconcat				(sizeof(m_save_name),m_save_name,save_name,SAVE_EXTENSION);

	BearString::Copy					(g_last_saved_game, save_name);

	IReader						*stream;
	stream = XRayBearReader::Create( FS.Read("%saves%", m_save_name));
	if (!stream) {
		Msg						("* Cannot find saved game %s", m_save_name);
		BearString::Copy				(m_save_name,save);
		return					(false);
	}

	CHECK_OR_EXIT				(CSavedGameWrapper::valid_saved_game(*stream),make_string("%s\nSaved game version mismatch or saved game is corrupted", save_name));

	string512					temp;
	strconcat					(sizeof(temp),temp,CStringTable().translate("st_loading_saved_game").c_str()," \"",save_name,SAVE_EXTENSION,"\"");
	g_pGamePersistent->LoadTitle(temp);

	unload						();
	reload						(m_section);

	u32							source_count = stream->r_u32();
	void						*source_data = xr_malloc(source_count);
	XrCompressor::RtcDecompress				(source_data,source_count,stream->pointer(),stream->length() - 3*sizeof(u32));
	XRayBearReader::Destroy(stream);
	load						(source_data, source_count, m_save_name);
	xr_free						(source_data);

	groups().on_after_game_load	();

	VERIFY						(graph().actor());
	
	Msg							("* Game %s is successfully loaded from file '%s' (%.3fs)",save_name, save_name,timer.get_elapsed_time().asseconds());

	return						(true);
}

void CALifeStorageManager::save	(NET_Packet &net_packet)
{
	prepare_objects_for_save	();

	shared_str					game_name;
	net_packet.r_stringZ		(game_name);
	save						(*game_name,!!net_packet.r_u8());
}

void CALifeStorageManager::prepare_objects_for_save	()
{
	Level().ClientSend			();
	Level().ClientSave			();
}
