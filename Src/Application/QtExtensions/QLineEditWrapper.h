#pragma once

#include "IWrapper.h"
#include <ClanLib/core.h>
#include <QtGui/QLineEdit>
#include <Depends/Entity/Property.h>

class QLineEditWrapper : public IWrapper
{
public:
	QLineEditWrapper(Engine::Entity::Property<CL_String> property);
	virtual ~QLineEditWrapper();

	QLineEdit *getQLineEdit() const { return lineedit; }
	Engine::Entity::Property<CL_String> getProperty() { return property; }

private:
	void OnPropertyChanged(const CL_String &oldValue, const CL_String &newValue);
	CL_Slot slotPropertyChanged;
	QLineEdit *lineedit;
	Engine::Entity::Property<CL_String> property;

	bool slotChanged;
	bool propChanged;

public slots:
	virtual void slotTextChanged(const QString &text);
};
