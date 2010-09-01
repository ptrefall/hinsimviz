#include "Agent.h"
#include <Scene/Object.h>

using namespace Engine;
using namespace Player;

Agent::Agent(unsigned int id, const CL_String &name, Core::CoreManager *coreMgr)
: IPlayer(id, name, coreMgr)
{
	this->id = id;
	this->name = name;
	this->coreMgr = coreMgr;
}

Agent::~Agent()
{
}

void Agent::addGO(Scene::Object *go)
{
	gos.push_back(go);
}

void Agent::removeGO(Scene::Object *go)
{
	for(unsigned int i = 0; i < gos.size(); i++)
	{
		if(gos[i] == go)
		{
			gos.erase((gos.begin()+i));
			break;
		}
	}
}

Scene::Object *Agent::getGO(unsigned int i)
{
	if(i >= gos.size())
		return NULL;
	return gos[i];
}

unsigned int Agent::getGOCount() const
{
	return gos.size();
}


void Agent::sendCommand(const CL_String &command)
{
	for(unsigned int i = 0; i < gos.size(); i++)
	{
		gos[i]->executeCommand(command, this);
	}
}

void Agent::sendEvent(const Events::IEvent &event)
{
	for(unsigned int i = 0; i < gos.size(); i++)
	{
		gos[i]->executeEvent(event, this);
	}
}
