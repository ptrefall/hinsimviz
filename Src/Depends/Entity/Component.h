/*Component-based Entity Engine
Copyright (c) 2009 Pål Trefall and Kenneth Gangstø

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Note: Some of the libraries Component-based Entity Engine may link to may have additional
requirements or restrictions.
*/

#pragma once

#include "precomp.h"
#include "Property.h"
#include "ComponentContainer.h"
#include "PropertyContainer.h"
#include <Engine/Events/CommandHandler.h>
#include <Engine/Events/IEvent.h>

namespace Engine{
namespace Player { class IPlayer; }
namespace Entity
{

class Component
{
public:
	virtual ~Component();

	virtual CL_String GetName() const { return name; }

    virtual void Update(double deltaTime) {};
	virtual void Interpolate(double deltaTime) {};
    //virtual void RequestCommands(std::vector<CL_String> &requestCommands, ServerPlayer *player) {};
	virtual void ExecuteCommand(const CL_String &command, Engine::Player::IPlayer *player) {};
	virtual void ExecuteEvent(const Events::IEvent &event, Engine::Player::IPlayer *player) {};

	template<class T>Property<T> AddProperty(const CL_String &propName, const T &defaultValue);
	template<class T>Property<T> GetProperty(const CL_String &propName);
	void RemoveProperty(const CL_String &propName);

	std::vector<CL_String> &GetCommands() { return commands; }

protected:
	Component(ComponentContainer* componentContainer, PropertyContainer *propertyContainer, const CL_String &name) : componentContainer(componentContainer), propertyContainer(propertyContainer), name(name) {};

	ComponentContainer *componentContainer;
	PropertyContainer *propertyContainer;
    CL_String name;
	std::vector<CL_String> commands;
};

template<class T>
inline Property<T> Component::AddProperty(const CL_String &propName, const T &defaultValue)
{
	CL_String compPropName = name + ":" + propName;
	return propertyContainer->AddProperty<T>(compPropName, defaultValue);
}

template<class T>
inline Property<T> Component::GetProperty(const CL_String &propName)
{
	CL_String compPropName = name + ":" + propName;
	return propertyContainer->GetProperty<T>(compPropName);
}

inline void Component::RemoveProperty(const CL_String &propName)
{
	CL_String compPropName = name + ":" + propName;
	propertyContainer->RemoveProperty(compPropName);
}

}}
