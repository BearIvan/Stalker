////////////////////////////////////////////////////////////////////////////
//	Module 		: saved_game_wrapper.cpp
//	Created 	: 21.02.2006
//  Modified 	: 21.02.2006
//	Author		: Dmitriy Iassenev
//	Description : saved game wrapper class
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "saved_game_wrapper.h"
#include "alife_time_manager.h"
#include "alife_object_registry.h"
#include "xrServer_Objects_ALife_Monsters.h"
#include "ai_space.h"
#include "game_graph.h"
#include "alife_simulator_header.h"

extern LPCSTR alife_section;

LPCSTR CSavedGameWrapper::saved_game_full_name	(LPCSTR saved_game_name, string_path& result)
{
	string_path					temp;
	strconcat					(sizeof(temp),temp,saved_game_name,SAVE_EXTENSION);
	xr_strcpy				(result,temp);
	return						(result);
}

bool CSavedGameWrapper::saved_game_exist		(LPCSTR saved_game_name)
{
	string_path					file_name;
	return						(!!FS.ExistFile(TEXT("%saves%"),saved_game_full_name(saved_game_name,file_name)));
}

bool CSavedGameWrapper::valid_saved_game		(IReader &stream)
{
	if (stream.length() < 8)
		return					(false);

	if (stream.r_u32() != u32(-1))
		return					(false);

	if (stream.r_u32() < 0x0002)
		return					(false);

	return						(true);
}

bool CSavedGameWrapper::valid_saved_game		(LPCSTR saved_game_name)
{
	string_path					file_name;
	if (!FS.ExistFile(TEXT("%saves%"), saved_game_full_name(saved_game_name, file_name)))
		return					(false);

	IReader						*stream =XRayBearReader::Create( FS.Read(TEXT("%saves%"),file_name));
	bool						result = valid_saved_game(*stream);
	XRayBearReader::Destroy( stream);
	return						(result);
}

CSavedGameWrapper::CSavedGameWrapper		(LPCSTR saved_game_name)
{
	string_path					file_name;
	saved_game_full_name		(saved_game_name,file_name);
	R_ASSERT3					(FS.ExistFile(TEXT("%saves%"), file_name),"There is no saved game ",file_name);
	
	IReader						*stream = XRayBearReader::Create(FS.Read(TEXT("%saves%"), file_name));
	if (!valid_saved_game(*stream)) {
		XRayBearReader::Destroy(stream);
		CALifeTimeManager		time_manager(alife_section);
		m_game_time				= time_manager.game_time();
		m_actor_health			= 1.f;
		m_level_id				= ai().game_graph().header().levels().begin()->first;
		return;
	}

	u32							source_count = stream->r_u32();
	void						*source_data = xr_malloc(source_count);
	rtc_decompress				(source_data,source_count,stream->pointer(),stream->length() - 3*sizeof(u32));
	XRayBearReader::Destroy(stream);

	IReader						reader(source_data,source_count);

	{
		CALifeTimeManager		time_manager(alife_section);
		time_manager.load		(reader);
		m_game_time				= time_manager.game_time();
	}

	{
		R_ASSERT2				(reader.find_chunk(OBJECT_CHUNK_DATA),"Can't find chunk OBJECT_CHUNK_DATA!");
		u32						count = reader.r_u32();
		VERIFY					(count > 0);
		CSE_ALifeDynamicObject	*object = CALifeObjectRegistry::get_object(reader);
		VERIFY					(object->ID == 0);
		CSE_ALifeCreatureActor	*actor = smart_cast<CSE_ALifeCreatureActor*>(object);
		VERIFY					(actor);

		m_actor_health			= actor->g_Health();
		m_level_id				= ai().game_graph().vertex(object->m_tGraphID)->level_id();

		F_entity_Destroy		(object);
	}

	xr_free						(source_data);
}
