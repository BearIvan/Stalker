#include "stdafx.h"
#include "XrGameVersionController.h"

XRAPI_API const char*	ENGINE_VERSION = "1.7.01";

XRAPI_API GameVersionController* gameVersionController=NULL;

GameVersionController::Game GameVersionController::getGame() const
{
	switch (m_path)
	{
	case GameVersionController::NPath:
		break;
	case GameVersionController::SOC_1004:
		return SOC;
		break;
	case GameVersionController::SOC_1007:
		return SOC;
		break;
	case GameVersionController::CS_1510:
		return CS;
		break;
	case GameVersionController::COP_1602:
		return COP;
		break;
	default:
		break;
	}
	return Game::NGame;
}

GameVersionController::Path GameVersionController::getPath() const
{
	return m_path;
}

GameVersionController::GameVersionController(Path path) :m_path(path)
{
	
}

