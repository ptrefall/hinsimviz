#include "QSpinBoxWrapper.h"
#include "ObjectQtWrapper.h"

QSpinBoxWrapper::QSpinBoxWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<int> property)
: slotChanged(false), propChanged(false)
{
	this->qObj = qObj;
	iProperty = property;

	spinbox = new QSpinBox();
	spinbox->setValue(iProperty.Get());

	slotIPropertyChanged = iProperty.ValueChanged().connect(this, &QSpinBoxWrapper::OnIPropertyChanged);
	connect(spinbox, SIGNAL( valueChanged(int) ), this, SLOT( slotIValueChanged(int) ));
}

QSpinBoxWrapper::QSpinBoxWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<unsigned int> property)
: slotChanged(false), propChanged(false)
{
	this->qObj = qObj;
	uProperty = property;

	spinbox = new QSpinBox();
	spinbox->setValue((int)uProperty.Get());

	slotUPropertyChanged = uProperty.ValueChanged().connect(this, &QSpinBoxWrapper::OnUPropertyChanged);
	connect(spinbox, SIGNAL( valueChanged(int) ), this, SLOT( slotUValueChanged(int) ));
}

QSpinBoxWrapper::~QSpinBoxWrapper()
{
}

void QSpinBoxWrapper::OnIPropertyChanged(const int &oldValue, const int &newValue)
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

void QSpinBoxWrapper::OnUPropertyChanged(const unsigned int &oldValue, const unsigned int &newValue)
{
	if(!qObj->isSelected())
		return;

	if(!slotChanged)
	{
		propChanged = true;
		spinbox->setValue((int)newValue);
	}
	else
	{
		slotChanged = false;
	}
}

void QSpinBoxWrapper::slotIValueChanged(int value)
{
	if(!propChanged)
	{
		slotChanged = true;
		iProperty = value;
	}
	else
	{
		propChanged = false;
	}
}

void QSpinBoxWrapper::slotUValueChanged(int value)
{
	if(!propChanged)
	{
		slotChanged = true;
		uProperty = (unsigned int)value;
	}
	else
	{
		propChanged = false;
	}
}