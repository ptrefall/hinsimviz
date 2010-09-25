#include "QLineEditWrapper.h"
#include "ObjectQtWrapper.h"

QLineEditWrapper::QLineEditWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<CL_String> property)
: slotChanged(false), propChanged(false)
{
	this->qObj = qObj;
	this->property = property;

	lineedit = new QLineEdit();
	lineedit->setText(this->property.Get().c_str());

	slotPropertyChanged = this->property.ValueChanged().connect(this, &QLineEditWrapper::OnPropertyChanged);
	connect(lineedit, SIGNAL( textChanged(const QString &) ), this, SLOT( slotTextChanged(const QString &) ));
}

QLineEditWrapper::~QLineEditWrapper()
{
}

void QLineEditWrapper::OnPropertyChanged(const CL_String &oldValue, const CL_String &newValue)
{
	if(!qObj->isSelected())
		return;

	if(!slotChanged)
	{
		propChanged = true;
		lineedit->setText(newValue.c_str());
	}
	else
	{
		slotChanged = false;
	}
}

void QLineEditWrapper::slotTextChanged(const QString & text)
{
	if(!propChanged)
	{
		slotChanged = true;
		property = CL_String(text.toStdString().c_str());
	}
	else
	{
		propChanged = false;
	}
}
