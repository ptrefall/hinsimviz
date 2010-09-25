#pragma once

#include "IWrapper.h"
#include <ClanLib/core.h>
#include <QtGui/QSpinBox>
#include <Depends/Entity/Property.h>

class ObjectQtWrapper;

class QSpinBoxWrapper : public IWrapper
{
public:
	QSpinBoxWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<int> property);
	QSpinBoxWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<unsigned int> property);
	virtual ~QSpinBoxWrapper();

	QSpinBox *getQSpinBox() const { return spinbox; }
	Engine::Entity::Property<int> getIProperty() { return iProperty; }
	Engine::Entity::Property<unsigned int> getUProperty() { return uProperty; }

private:
	void OnIPropertyChanged(const int &oldValue, const int &newValue);
	void OnUPropertyChanged(const unsigned int &oldValue, const unsigned int &newValue);
	CL_Slot slotIPropertyChanged;
	CL_Slot slotUPropertyChanged;
	QSpinBox *spinbox;
	Engine::Entity::Property<int> iProperty;
	Engine::Entity::Property<unsigned int> uProperty;

	ObjectQtWrapper *qObj;

	bool slotChanged;
	bool propChanged;

public slots:
	virtual void slotIValueChanged(int value);
	virtual void slotUValueChanged(int value);
};
