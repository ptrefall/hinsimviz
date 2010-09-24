#pragma once

#include "IWrapper.h"
#include <ClanLib/core.h>
#include <QtGui/QDoubleSpinBox>
#include <Depends/Entity/Property.h>

class QDoubleSpinBoxWrapper : public IWrapper
{
public:
	QDoubleSpinBoxWrapper(Engine::Entity::Property<double> property);
	QDoubleSpinBoxWrapper(Engine::Entity::Property<float> property);
	virtual ~QDoubleSpinBoxWrapper();

	QDoubleSpinBox *getQDoubleSpinBox() const { return spinbox; }
	Engine::Entity::Property<double> getDProperty() { return dProperty; }
	Engine::Entity::Property<float> getFProperty() { return fProperty; }

private:
	void OnDPropertyChanged(const double &oldValue, const double &newValue);
	void OnFPropertyChanged(const float &oldValue, const float &newValue);
	CL_Slot slotDPropertyChanged;
	CL_Slot slotFPropertyChanged;
	QDoubleSpinBox *spinbox;
	Engine::Entity::Property<double> dProperty;
	Engine::Entity::Property<float> fProperty;

	bool slotChanged;
	bool propChanged;

public slots:
	virtual void slotDValueChanged(double value);
	virtual void slotFValueChanged(double value);
};
