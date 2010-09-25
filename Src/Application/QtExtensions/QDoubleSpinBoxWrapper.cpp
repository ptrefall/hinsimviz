#include "QDoubleSpinBoxWrapper.h"
#include "ObjectQtWrapper.h"

QDoubleSpinBoxWrapper::QDoubleSpinBoxWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<double> property)
: slotChanged(false), propChanged(false)
{
	this->qObj = qObj;
	dProperty = property;

	spinbox = new QDoubleSpinBox();
	spinbox->setValue(dProperty.Get());

	slotDPropertyChanged = dProperty.ValueChanged().connect(this, &QDoubleSpinBoxWrapper::OnDPropertyChanged);
	connect(spinbox, SIGNAL( valueChanged(double) ), this, SLOT( slotDValueChanged(double) ));
}

QDoubleSpinBoxWrapper::QDoubleSpinBoxWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<float> property)
: slotChanged(false), propChanged(false)
{
	this->qObj = qObj;
	fProperty = property;

	spinbox = new QDoubleSpinBox();
	spinbox->setValue((double)fProperty.Get());

	slotFPropertyChanged = fProperty.ValueChanged().connect(this, &QDoubleSpinBoxWrapper::OnFPropertyChanged);
	connect(spinbox, SIGNAL( valueChanged(double) ), this, SLOT( slotFValueChanged(double) ));
}

QDoubleSpinBoxWrapper::~QDoubleSpinBoxWrapper()
{
}

void QDoubleSpinBoxWrapper::OnDPropertyChanged(const double &oldValue, const double &newValue)
{
	if(!qObj->isSelected())
		return;

	if(!slotChanged)
	{
		propChanged = true;
		spinbox->setValue(newValue);
	}
	else
	{
		slotChanged = false;
	}
}

void QDoubleSpinBoxWrapper::OnFPropertyChanged(const float &oldValue, const float &newValue)
{
	if(!qObj->isSelected())
		return;

	if(!slotChanged)
	{
		propChanged = true;
		spinbox->setValue((double)newValue);
	}
	else
	{
		slotChanged = false;
	}
}

void QDoubleSpinBoxWrapper::slotDValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		dProperty = value;
	}
	else
	{
		propChanged = false;
	}
}

void QDoubleSpinBoxWrapper::slotFValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		fProperty = (float)value;
	}
	else
	{
		propChanged = false;
	}
}
