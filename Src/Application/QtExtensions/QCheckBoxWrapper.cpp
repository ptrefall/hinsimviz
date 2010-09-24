#include "QCheckBoxWrapper.h"

QCheckBoxWrapper::QCheckBoxWrapper(Engine::Entity::Property<bool> property)
: slotChanged(false), propChanged(false)
{
	this->property = property;

	checkbox = new QCheckBox();
	if(this->property.Get() == true)
		checkbox->setCheckState(Qt::Checked);
	else
		checkbox->setCheckState(Qt::Unchecked);

	slotPropertyChanged = this->property.ValueChanged().connect(this, &QCheckBoxWrapper::OnPropertyChanged);
	
	connect(checkbox, SIGNAL( stateChanged(int) ), this, SLOT( slotStateChanged(int) ));
}

QCheckBoxWrapper::~QCheckBoxWrapper()
{
}

void QCheckBoxWrapper::OnPropertyChanged(const bool &oldValue, const bool &newValue)
{
	if(newValue == true && !slotChanged)
	{
		propChanged = true;
		checkbox->setCheckState(Qt::Checked);
	}
	else if(!slotChanged)
	{
		propChanged = true;
		checkbox->setCheckState(Qt::Unchecked);
	}
	else if(slotChanged)
	{
		slotChanged = false;
	}
}

void QCheckBoxWrapper::slotStateChanged(int state)
{
	if(state == Qt::Checked && !propChanged)
	{
		slotChanged = true;
		property = true;
	}
	else if(state == Qt::Unchecked && !propChanged)
	{
		slotChanged = true;
		property = false;
	}
	else if(propChanged)
	{
		propChanged = false;
	}
}
