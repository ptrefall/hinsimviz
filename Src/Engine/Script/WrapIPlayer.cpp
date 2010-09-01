#include "WrapIPlayer.h"
#include "WrapPlayerManager.h"
#include "WrapObjectManager.h"
#include "WrapObject.h"
#include "ScriptManager.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Player/PlayerManager.h>
#include <Engine/Player/IPlayer.h>
#include <Engine/Events/Event.h>

using namespace Engine;
using namespace Script;
using namespace LuaPlus;

WrapIPlayer::WrapIPlayer(Core::CoreManager *coreMgr, WrapPlayerManager *wPlayerMgr, Player::IPlayer *player, bool isHuman)
{
	this->coreMgr = coreMgr;
	this->wPlayerMgr = wPlayerMgr;
	this->player = player;
	this->isHuman = isHuman;
}

WrapIPlayer::~WrapIPlayer()
{
}

int WrapIPlayer::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	unsigned int playerId = player->getId();
	
	LuaObject &lPlayers = wPlayerMgr->getLPlayers();
	lPlayer = lPlayers.CreateTable((int)playerId);
	lPlayer.SetInteger("id", (int)playerId);
	lPlayer.SetString("name", player->getName().c_str());
	lMeta = lPlayer.CreateTable("MetaTable");
	lMeta.SetObject("__index", lMeta);

	lMeta.RegisterDirect("AddObject", *this, &WrapIPlayer::AddObject);
	lMeta.RegisterDirect("SendEvent", *this, &WrapIPlayer::SendEvent);
	lMeta.RegisterDirect("SendCommand", *this, &WrapIPlayer::SendCommand);
	
	lPlayer.SetLightUserData("__object", this);
	lPlayer.SetMetaTable(lMeta);

	//Human should be available both in Players table, and in the direct global table Human.
	if(isHuman)
	{
		LuaObject lHuman = globals.CreateTable("Human");
		lHuman.SetInteger("id", (int)playerId);
		lHuman.SetString("name", player->getName().c_str());
		LuaObject lHumanMeta = lHuman.CreateTable("MetaTable");
		lHumanMeta.SetObject("__index", lHumanMeta);
		lHumanMeta.RegisterDirect("AddObject", *this, &WrapIPlayer::AddObject);
		lHumanMeta.RegisterDirect("SendEvent", *this, &WrapIPlayer::SendEvent);
		lHumanMeta.RegisterDirect("SendCommand", *this, &WrapIPlayer::SendCommand);
		lHumanMeta.RegisterDirect("GetRelativeFocus", *this, &WrapIPlayer::GetRelativeFocus);
		lHuman.SetLightUserData("__object", this);
		lHuman.SetMetaTable(lHumanMeta);
		wPlayerMgr->setLHuman(lHuman);
		wPlayerMgr->setWHuman(this);
	}

	return 0;
}

void WrapIPlayer::AddObject(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lObj)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapIPlayer::AddObject", msg, Log::L_ERROR);
		return;
	}

	if(!lObj.IsTable())
	{
		CL_String msg = cl_format("Obj was not a table (it's a %1)", lObj.TypeName());
		coreMgr->getLogMgr()->log("WrapIPlayer::AddObject", msg, Log::L_ERROR);
		return;
	}

	LuaObject lObjId = lObj.GetByName("id");
	if(!lObjId.IsInteger())
	{
		CL_String msg = cl_format("Object ID was not an integer (it's a %1)", lObjId.TypeName());
		coreMgr->getLogMgr()->log("WrapIPlayer::AddObject", msg, Log::L_ERROR);
		return;
	}

	WrapObject *objWrapper = coreMgr->getScriptMgr()->getWObjMgr()->getWObj((unsigned int)lObjId.ToInteger());
	if(objWrapper == NULL)
	{
		CL_String msg = cl_format("Could not find the WrapObject in WrapObjectManager with Id: %1", lObjId.ToInteger());
		coreMgr->getLogMgr()->log("WrapIPlayer::AddObject", msg, Log::L_ERROR);
		return;
	}

	Scene::Object *iObj = objWrapper->getObj();
	if(iObj == NULL)
	{
		CL_String msg = cl_format("IObject of objWrapper with id %1 was NULL", lObjId.ToInteger());
		coreMgr->getLogMgr()->log("WrapIPlayer::AddObject", msg, Log::L_ERROR);
		return;
	}

	player->addGO(iObj);
}

void WrapIPlayer::SendCommand(LuaObject lSelf, LuaObject lCommand)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapIPlayer::SendCommand", msg, Log::L_ERROR);
		return;
	}

	if(!lCommand.IsString())
	{
		CL_String msg = cl_format("CommandType was not a string (it's a %1)", lCommand.TypeName());
		coreMgr->getLogMgr()->log("WrapIPlayer::SendCommand", msg, Log::L_ERROR);
		return;
	}

	player->sendCommand(lCommand.ToString());
}

void WrapIPlayer::SendEvent(LuaObject lSelf, LuaObject lEventType, LuaObject lEventArg)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapIPlayer::SendEvent", msg, Log::L_ERROR);
		return;
	}

	if(!lEventType.IsString())
	{
		CL_String msg = cl_format("EventType was not a string (it's a %1)", lEventType.TypeName());
		coreMgr->getLogMgr()->log("WrapIPlayer::SendEvent", msg, Log::L_ERROR);
		return;
	}

	if(!lEventArg.IsString() && !lEventArg.IsBoolean() && !lEventArg.IsNumber() && !lEventArg.IsNil())
	{
		CL_String msg = cl_format("EventArg was not a string, bool, number or nil (it's a %1)", lEventArg.TypeName());
		coreMgr->getLogMgr()->log("WrapIPlayer::SendEvent", msg, Log::L_ERROR);
		return;
	}

	CL_String eventType = lEventType.ToString();
	if(lEventArg.IsString())
	{
		CL_String arg = lEventArg.ToString();
		player->sendEvent(Engine::Events::Event<CL_String, Engine::Scene::Object*>(eventType.c_str(), arg, NULL));
	}
	else if(lEventArg.IsBoolean())
	{
		bool arg = lEventArg.GetBoolean();
		player->sendEvent(Engine::Events::Event<bool, Engine::Scene::Object*>(eventType.c_str(), arg, NULL));
	}
	else if(lEventArg.IsNumber())
	{
		float arg = lEventArg.GetFloat();
		player->sendEvent(Engine::Events::Event<float, Engine::Scene::Object*>(eventType.c_str(), arg, NULL));
	}
	else
	{
		player->sendEvent(Engine::Events::Event<Engine::Scene::Object*, Engine::Scene::Object*>(eventType.c_str(), NULL, NULL));
	}
}

LuaPlus::LuaObject WrapIPlayer::GetRelativeFocus(LuaPlus::LuaObject lSelf)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapIPlayer::GetRelativeFocus", msg, Log::L_ERROR);
		return LuaObject();
	}


	CL_Vec3f rel = coreMgr->getPlayerMgr()->getHuman()->getRelativeFocus();
	LuaObject lRel = coreMgr->getScriptMgr()->GetGlobalState()->Get()->CreateTable();
	lRel.SetNumber("x", rel.x);
	lRel.SetNumber("y", rel.y);
	return lRel;
}
