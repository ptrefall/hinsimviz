#pragma once

#include "IWrapper.h"
#include <ClanLib/core.h>
#include <QtGui/QCheckBox>
#include <Depends/Entity/Property.h>

class ObjectQtWrapper;

class QCheckBoxWrapper : public IWrapper
{
public:
	QCheckBoxWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<bool> property);
	virtual ~QCheckBoxWrapper();

	QCheckBox *getQCheckBox() const { return checkbox; }
	Engine::Entity::Property<bool> getProperty() { return property; }

private:
	void OnPropertyChanged(const bool &oldValue, const bool &newValue);
	CL_Slot slotPropertyChanged;
	QCheckBox *checkbox;
	Engine::Entity::Property<bool> property;

	ObjectQtWrapper *qObj;

	bool slotChanged;
	bool propChanged;

public slots:
	virtual void slotStateChanged(int state);
};
