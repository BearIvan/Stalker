#include "stdafx.h"
#include "XRayDiscordAPI.h"
#include "discord/discord_rpc.h"
#include "api/XrGameVersionController.h"
#include "Modloader.h"
//#ifdef DEBUG
#undef NO_DISCORD
//#endif
XRayDiscordAPI::XRayDiscordAPI()
{
#ifndef NO_DISCORD 
	Device.seqFrame.Add(this);
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	handlers.ready = Ready;
	handlers.errored = Errored;
	handlers.disconnected = Disconnected;
	handlers.joinGame = JoinGame;
	handlers.spectateGame = SpectateGame;
	handlers.joinRequest = JoinRequest;
	m_StartTime = time(0);

	Discord_Initialize("556516569476497421", &handlers, 1, "1234");
#endif
}

XRayDiscordAPI::~XRayDiscordAPI()
{
#ifndef NO_DISCORD 
	Discord_Shutdown();
	Device.seqFrame.Remove(this);
#endif
}

void XRayDiscordAPI::OnFrame()
{
#ifndef NO_DISCORD 
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = "";
	switch (gameVersionController->getPath())
	{
	case GameVersionController::Path::SOC_1004:
		discordPresence.details = "Shadow of Chernobyl";
		discordPresence.largeImageKey = "icon1";
		discordPresence.state = "Original 1.0.04";
		break;
	case GameVersionController::Path::SOC_1007:
		discordPresence.details = "Shadow of Chernobyl";
		discordPresence.largeImageKey = "icon1";
		discordPresence.state = "Original 1.0.06";
		break;
	case GameVersionController::Path::CS_1510:
		discordPresence.details = "Clear sky";
		discordPresence.largeImageKey = "icon2";
		discordPresence.state = "Original 1.5.10";
		break;
	case GameVersionController::Path::COP_1602:
		discordPresence.details = "Call of Pripyat";
		discordPresence.largeImageKey = "icon3";
		discordPresence.state = "Original 1.6.02";
		break;

	default:
		break;
	}
	static BearStringAnsi64 state;
	if (Modloader::GetNameMod()[0])
	{
		BearString::Copy(state, "Mod:");
		BearString::Contact(state, Modloader::GetNameMod());
		discordPresence.state = state;
	}

	discordPresence.startTimestamp = m_StartTime;
	discordPresence.largeImageText = ENGINE_VERSION;
	discordPresence.partyId = "ae488379-351d-4a4f-ad32-2b9b01c91657";
	Discord_UpdatePresence(&discordPresence);
#endif
}

void XRayDiscordAPI::Ready(const DiscordUser * request)
{
}

void XRayDiscordAPI::Disconnected(int errorCode, const char * message)
{
}

void XRayDiscordAPI::Errored(int errorCode, const char * message)
{
}

void XRayDiscordAPI::JoinGame(const char * joinSecret)
{
}

void XRayDiscordAPI::SpectateGame(const char * spectateSecret)
{
}

void XRayDiscordAPI::JoinRequest(const DiscordUser * request)
{
}
