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
#include "IProperty.h"
#include "Property.h"

namespace Engine
{
namespace Core { class CoreManager; }
namespace Entity
{
class Component;

class PropertyContainer
{
public:
	PropertyContainer(Core::CoreManager *coreMgr) { this->coreMgr = coreMgr; }
	virtual ~PropertyContainer() { RemoveAllProperties(); }

	bool HasProperty(const CL_String& propName);

	template<class T>Property<T> AddProperty(const CL_String& propName, const T &defaultValue);
	void AddProperty(IProperty *property);
	void PropertyAdded(IProperty *property);

	template<class T>Property<T> GetProperty(const CL_String& propName);
	IProperty *GetIProperty(const CL_String& propName);
	
	void RemoveProperty(const CL_String& propName, bool postponeDelete = false);
	void RemoveAllProperties();
	
	std::map<CL_String, IProperty*> &GetProperties() { return properties; }

	std::vector<IProperty*> &GetNewProperties() { return addedProperties; }
	std::vector<IProperty*> &GetDeletedProperties() { return deletedProperties; }

	void UpdateProperties(int deltaTime);
        CL_Signal_v1<int> SigUpdateProperties;

protected:
	void ClearDirtyProperties();
	void ClearAddedProperties();
	void ClearDeletedProperties();

	std::map<CL_String, IProperty*> properties;

	std::vector<IProperty*> addedProperties;
	std::vector<IProperty*> deletedProperties;

	Core::CoreManager *coreMgr;
};

inline bool PropertyContainer::HasProperty(const CL_String& propName)
{
	std::map<CL_String, IProperty*>::iterator it = properties.find(propName);
	if(it != properties.end())
		return true;
	else
		return false;
}

inline void PropertyContainer::AddProperty(IProperty *property)
{
	std::map<CL_String, IProperty*>::iterator it = properties.find(property->GetName());
	if(it == properties.end())
	{
		addedProperties.push_back(property);
		properties[property->GetName()] = property;
		PropertyAdded(property);
	}
}

template<class T>
inline Property<T> PropertyContainer::AddProperty(const CL_String& propName, const T &defaultValue)
{
	std::map<CL_String, IProperty*>::iterator it = properties.find(propName);
	if(it != properties.end())
	{
		Property<T>* property = dynamic_cast< Property<T>* >(it->second);
		if(!property)
			throw T_Exception("Property " + propName + " already exists, but with another type!");
		return *property;
	}

	Property<T> *property = new Property<T>(propName);
	property->Set(defaultValue);
	addedProperties.push_back(property);
	properties[property->GetName()] = property;
	PropertyAdded(property);

	//return *property;
	return GetProperty<T>(propName);
}

template<class T>
inline Property<T> PropertyContainer::GetProperty(const CL_String& propName)
{
	std::map<CL_String, IProperty*>::iterator it = properties.find(propName);
	if(it != properties.end())
	{
		Property<T>* property = dynamic_cast< Property<T>* >(it->second);
		if(!property)
			throw T_Exception("Tried to get property " + propName + ", but the type was wrong!");
		return *property;
	}
	else
		throw T_Exception("Unable to get property " + propName);
}

inline IProperty *PropertyContainer::GetIProperty(const CL_String& propName)
{
	std::map<CL_String, IProperty*>::iterator it = properties.find(propName);
	if(it != properties.end())
	{
		return it->second;
	}
	else
		throw T_Exception("Unable to get property " + propName);
}

inline void PropertyContainer::RemoveProperty(const CL_String& propName, bool postponeDelete)
{
	std::map<CL_String, IProperty*>::iterator it = properties.find(propName);
	if(it != properties.end())
	{
		IProperty* property = (*it).second;
		if(postponeDelete)
			deletedProperties.push_back(property);
		else
			delete property;
		properties.erase(it);
	}
}

inline void PropertyContainer::RemoveAllProperties()
{
	std::map<CL_String, IProperty*>::iterator it;
	for(it = properties.begin(); it != properties.end(); ++it)
	{
		IProperty* property = (*it).second;
		delete property;
	}
	properties.clear();
	ClearAddedProperties();
	ClearDeletedProperties();
}

inline void PropertyContainer::UpdateProperties(int deltaTime)
{
	SigUpdateProperties.invoke(deltaTime);

	ClearDirtyProperties();
	ClearAddedProperties();
	ClearDeletedProperties();
}

inline void PropertyContainer::ClearDirtyProperties()
{
	std::map<CL_String, IProperty*>::iterator it = properties.begin();
	for(; it != properties.end(); ++it)
	{
		IProperty* property = (*it).second;
		if(property->IsDirty())
		{
			property->ClearDirty();
		}
	}
}

inline void PropertyContainer::ClearAddedProperties()
{
	addedProperties.clear();
}

inline void PropertyContainer::ClearDeletedProperties()
{
	std::vector<IProperty*>::iterator it;
	for(it = deletedProperties.begin(); it != deletedProperties.end(); ++it)
	{
		IProperty* property = (*it);
		delete property;
	}
	deletedProperties.clear();
}

}}
