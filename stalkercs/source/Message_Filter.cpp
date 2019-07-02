#include "stdafx.h"
#include "Message_Filter.h"
#include "NET_Queue.h"

message_filter::message_filter()
{
	m_onround_start_pause = fastdelegate::MakeDelegate(this, &message_filter::on_round_start_pause_impl);
	m_on_kill_pause	= fastdelegate::MakeDelegate(this, &message_filter::on_kill_pause_impl);
	m_on_artefactdelivering_pause = fastdelegate::MakeDelegate(this, &message_filter::on_artefactdelivering_pause_impl);
	m_msg_log_file = NULL;
	m_last_string[0]	= 0;
	m_strrepeat_count	= 0;
}
message_filter::~message_filter()
{
	//if (m_msg_log_file)
	//	FS.w_close(m_msg_log_file);
}

void message_filter::filter(u16 const & msg_type,
								u32 const & msg_subtype,
								msg_type_subtype_func_t const & found_func)
{
	msg_type_subtype_t msgid(msg_type, msg_subtype);
	R_ASSERT2(m_filters.find(msgid) == m_filters.end(), "message filter already exist");
	m_filters.insert(std::make_pair(msgid, found_func));
}

void message_filter::remove_filter(u16 const & msg_type, u32 const & msg_subtype)
{
	msg_type_subtype_t msgid(msg_type, msg_subtype);
	filters_map_t::iterator tmp_iter = m_filters.find(msgid);
	R_ASSERT2(tmp_iter != m_filters.end(), "message filter not found");
	m_filters.erase(tmp_iter);
}

void message_filter::msg_type_subtype_t::import(NET_Packet& packet)
{
	packet.r_begin(msg_type);
	switch (msg_type)
	{
	case M_EVENT:
		{
			u16				tmp_subtype;
			packet.r_u32			(msg_receive_time);
			packet.r_u16			(tmp_subtype);
			packet.r_u16			(dest_obj_id);
			msg_subtype		= static_cast<u32>(tmp_subtype);
		}break;
	case M_GAMEMESSAGE:
		{
			packet.r_u32			(msg_subtype);
		}break;
	};//switch (msg_type)
}

void message_filter::check_new_data	(NET_Packet & packet)
{
	u32			tmp_old_pos = packet.r_tell();
	
	msg_type_subtype_t	packet_mtype;
	packet_mtype.import	(packet);


	if (packet_mtype.msg_type == M_EVENT_PACK)
	{
		NET_Packet tmp_packet;
		while (!packet.r_eof())
		{
			tmp_packet.B.count = packet.r_u8();
			packet.r	(tmp_packet.B.data, tmp_packet.B.count);
			packet_mtype.import(tmp_packet);
			
			R_ASSERT2(packet_mtype.msg_type != M_EVENT_PACK, "M_EVENT_PACK in M_EVENT_PACK");
			dbg_print_msg	(tmp_packet, packet_mtype);

			filters_map_t::iterator tmp_iter = m_filters.find(packet_mtype);
			if (tmp_iter != m_filters.end())
			{
				tmp_iter->second(packet_mtype.msg_type,
					packet_mtype.msg_subtype,
					tmp_packet);
			}
		}
	} else {
		dbg_print_msg			(packet, packet_mtype);
		filters_map_t::iterator tmp_iter = m_filters.find(packet_mtype);
		if (tmp_iter != m_filters.end())
		{
			tmp_iter->second(packet_mtype.msg_type,
				packet_mtype.msg_subtype,
				packet);
		}
	}
	packet.r_seek	(tmp_old_pos);
}

void message_filter::dbg_set_message_log_file(string_path const & message_log_file)
{
	R_ASSERT(message_log_file);
	//m_msg_log_file = FS.w_open(message_log_file);
	if (!m_msg_log_file)
	{
		Msg("! ERROR: failed to open demo messages logging file");
	}
}

void message_filter::dbg_print_msg(NET_Packet & packet, msg_type_subtype_t const & msg_type)
{
	string256	tmp_string;
	switch (msg_type.msg_type)
	{
	case M_EVENT:
		{
			switch (msg_type.msg_subtype)
			{
			case GE_DESTROY:
				{
					sprintf_s(tmp_string, "--- CL_EVENT [%7u][%5u]: GE_DESTROY", msg_type.msg_receive_time, msg_type.dest_obj_id);
				}break;
			case GE_OWNERSHIP_TAKE:
				{
					u16 tmp_id_what;
					packet.r_u16(tmp_id_what);
					sprintf_s(tmp_string, "--- CL_EVENT [%7u][%5u]: GE_OWNERSHIP_TAKE    [%d]", msg_type.msg_receive_time, msg_type.dest_obj_id, tmp_id_what);
				}break;
			case GE_OWNERSHIP_REJECT:
				{
					u16 tmp_id_what;
					packet.r_u16(tmp_id_what);
					sprintf_s(tmp_string, "--- CL_EVENT [%7u][%5u]: GE_OWNERSHIP_REJECT  [%d]", msg_type.msg_receive_time, msg_type.dest_obj_id, tmp_id_what);
				}break;
			default:
				{
					sprintf_s(tmp_string, "--- CL_EVENT [%7u][%5u]: EVENT_ID=[%d]", msg_type.msg_receive_time, msg_type.dest_obj_id, msg_type.msg_subtype);
				};
			}; //switch (mtype.msg_subtype)
		}break;
	case M_EVENT_PACK:
		{
			FATAL("can't print M_EVENT_PACK message");
		}break;
	case M_GAMEMESSAGE:
		{
			switch (msg_type.msg_subtype)
			{
			case GAME_EVENT_PLAYER_KILLED:
				{
					sprintf_s(tmp_string, "--- GM_EVENT [%7u]: GAME_EVENT_PLAYER_KILLED", msg_type.msg_receive_time);
				}break;
			case GAME_EVENT_ROUND_STARTED:
				{
					sprintf_s(tmp_string, "--- GM_EVENT [%7u]: GAME_EVENT_ROUND_STARTED", msg_type.msg_receive_time);
				}break;
			case GAME_EVENT_ARTEFACT_TAKEN:
				{
					sprintf_s(tmp_string, "--- GM_EVENT [%7u]: GAME_EVENT_ARTEFACT_TAKEN", msg_type.msg_receive_time);	
				}break;
			default:
				{
					sprintf_s(tmp_string, "--- GM_EVENT [%7u]: GAME_EVENT_ID=[%d]", msg_type.msg_receive_time, msg_type.msg_subtype);	
				};
			}
		}break;
	case M_CHAT_MESSAGE:
		{
		}break;
	case M_SPAWN:
		{
			sprintf_s(tmp_string, "--- M_SPAWN                [%7u]", msg_type.msg_receive_time);
		}break;
	case M_SV_CONFIG_NEW_CLIENT:
		{
			sprintf_s(tmp_string, "--- M_SV_CONFIG_NEW_CLIENT [%7u]", msg_type.msg_receive_time);
		}break;
	case M_SV_CONFIG_GAME:
		{
			sprintf_s(tmp_string, "--- M_SV_CONFIG_GAME       [%7u]", msg_type.msg_receive_time);
		}break;
	case M_SV_CONFIG_FINISHED:
		{
			sprintf_s(tmp_string, "--- M_SV_CONFIG_FINISHED   [%7u]", msg_type.msg_receive_time);
		}break;
	default:
		{
			sprintf_s(tmp_string, "--- MESSAGE_ID[%u]         [%7u]", msg_type.msg_type, msg_type.msg_receive_time);
		};
	}; //switch (m_type)
	if (!xr_strcmp(tmp_string, m_last_string))
	{
		++m_strrepeat_count;
		return;
	} 
	Msg(tmp_string);
	BearCore::BearString::Copy(m_last_string, tmp_string);
	if (m_msg_log_file)
	{
		if (m_strrepeat_count)
		{
			m_msg_log_file->w_printf(". %d\n", m_strrepeat_count);
		}
		strcat_s(tmp_string, "\n");
		m_msg_log_file->w_stringZ(tmp_string);
		m_msg_log_file->flush();
	}
	m_strrepeat_count = 0;
}

void __stdcall	message_filter::on_round_start_pause_impl(u32 message, u32 subtype, NET_Packet const & packet)
{
	Device.Pause(!Device.Paused(), TRUE, TRUE, "game event reached");
}

void __stdcall	message_filter::on_kill_pause_impl(u32 message, u32 subtype, NET_Packet const & packet)
{
}

void __stdcall	message_filter::on_artefactdelivering_pause_impl(u32 message, u32 subtype, NET_Packet const & packet)
{
}