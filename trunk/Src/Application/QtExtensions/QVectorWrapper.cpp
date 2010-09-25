#include "QVectorWrapper.h"
#include "ObjectQtWrapper.h"

QVectorWrapper::QVectorWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<CL_Vec2f> property)
: slotChanged(false), propChanged(false), vecType(2)
{
	this->qObj = qObj;
	vec2Prop = property;

	spinbox1 = new QDoubleSpinBox();
	spinbox2 = new QDoubleSpinBox();

	spinbox1->setValue((double)vec2Prop.Get().x);
	spinbox2->setValue((double)vec2Prop.Get().y);

	slotPropertyChanged = vec2Prop.ValueChanged().connect(this, &QVectorWrapper::On2PropertyChanged);
	connect(spinbox1, SIGNAL( valueChanged(double) ), this, SLOT( slot1ValueChanged(double) ));
	connect(spinbox2, SIGNAL( valueChanged(double) ), this, SLOT( slot2ValueChanged(double) ));
}

QVectorWrapper::QVectorWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<CL_Vec3f> property)
: slotChanged(false), propChanged(false), vecType(3)
{
	this->qObj = qObj;
	vec3Prop = property;

	spinbox1 = new QDoubleSpinBox();
	spinbox2 = new QDoubleSpinBox();
	spinbox3 = new QDoubleSpinBox();

	spinbox1->setValue((double)vec3Prop.Get().x);
	spinbox2->setValue((double)vec3Prop.Get().y);
	spinbox3->setValue((double)vec3Prop.Get().z);

	slotPropertyChanged = vec3Prop.ValueChanged().connect(this, &QVectorWrapper::On3PropertyChanged);
	connect(spinbox1, SIGNAL( valueChanged(double) ), this, SLOT( slot1ValueChanged(double) ));
	connect(spinbox2, SIGNAL( valueChanged(double) ), this, SLOT( slot2ValueChanged(double) ));
	connect(spinbox3, SIGNAL( valueChanged(double) ), this, SLOT( slot3ValueChanged(double) ));
}

QVectorWrapper::QVectorWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<CL_Vec4f> property)
: slotChanged(false), propChanged(false), vecType(4)
{
	this->qObj = qObj;
	vec4Prop = property;

	spinbox1 = new QDoubleSpinBox();
	spinbox2 = new QDoubleSpinBox();
	spinbox3 = new QDoubleSpinBox();
	spinbox4 = new QDoubleSpinBox();

	spinbox1->setValue((double)vec4Prop.Get().x);
	spinbox2->setValue((double)vec4Prop.Get().y);
	spinbox3->setValue((double)vec4Prop.Get().z);
	spinbox4->setValue((double)vec4Prop.Get().w);

	slotPropertyChanged = vec4Prop.ValueChanged().connect(this, &QVectorWrapper::On4PropertyChanged);
	connect(spinbox1, SIGNAL( valueChanged(double) ), this, SLOT( slot1ValueChanged(double) ));
	connect(spinbox2, SIGNAL( valueChanged(double) ), this, SLOT( slot2ValueChanged(double) ));
	connect(spinbox3, SIGNAL( valueChanged(double) ), this, SLOT( slot3ValueChanged(double) ));
	connect(spinbox4, SIGNAL( valueChanged(double) ), this, SLOT( slot4ValueChanged(double) ));
}

QVectorWrapper::~QVectorWrapper()
{
}

void QVectorWrapper::On2PropertyChanged(const CL_Vec2f &oldValue, const CL_Vec2f &newValue)
{
	if(!qObj->isSelected())
		return;

	if(!slotChanged)
	{
		propChanged = true;
		spinbox1->setValue((double)newValue.x);
		spinbox2->setValue((double)newValue.y);
	}
	else
	{
		slotChanged = false;
	}
}

void QVectorWrapper::On3PropertyChanged(const CL_Vec3f &oldValue, const CL_Vec3f &newValue)
{
	if(!qObj->isSelected())
		return;

	if(!slotChanged)
	{
		propChanged = true;
		spinbox1->setValue((double)newValue.x);
		spinbox2->setValue((double)newValue.y);
		spinbox3->setValue((double)newValue.z);
	}
	else
	{
		slotChanged = false;
	}
}

void QVectorWrapper::On4PropertyChanged(const CL_Vec4f &oldValue, const CL_Vec4f &newValue)
{
	if(!qObj->isSelected())
		return;

	if(!slotChanged)
	{
		propChanged = true;
		spinbox1->setValue((double)newValue.x);
		spinbox2->setValue((double)newValue.y);
		spinbox3->setValue((double)newValue.z);
		spinbox4->setValue((double)newValue.w);
	}
	else
	{
		slotChanged = false;
	}
}

void QVectorWrapper::slot1ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(vecType == 2)
		{
			vec2Prop = CL_Vec2f((float)value, vec2Prop.Get().y);
		}
		else if(vecType == 3)
		{
			vec3Prop = CL_Vec3f((float)value, vec3Prop.Get().y, vec3Prop.Get().z);
		}
		else if(vecType == 4)
		{
			vec4Prop = CL_Vec4f((float)value, vec4Prop.Get().y, vec4Prop.Get().z, vec4Prop.Get().w);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QVectorWrapper::slot2ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(vecType == 2)
		{
			vec2Prop = CL_Vec2f(vec2Prop.Get().x, (float)value);
		}
		else if(vecType == 3)
		{
			vec3Prop = CL_Vec3f(vec3Prop.Get().x, (float)value, vec3Prop.Get().z);
		}
		else if(vecType == 4)
		{
			vec4Prop = CL_Vec4f(vec4Prop.Get().x, (float)value, vec4Prop.Get().z, vec4Prop.Get().w);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QVectorWrapper::slot3ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(vecType == 3)
		{
			vec3Prop = CL_Vec3f(vec3Prop.Get().x, vec3Prop.Get().y, (float)value);
		}
		else if(vecType == 4)
		{
			vec4Prop = CL_Vec4f(vec4Prop.Get().x, vec4Prop.Get().y, (float)value, vec4Prop.Get().w);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QVectorWrapper::slot4ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(vecType == 4)
		{
			vec4Prop = CL_Vec4f(vec4Prop.Get().x, vec4Prop.Get().y, vec4Prop.Get().z, (float)value);
		}
	}
	else
	{
		propChanged = false;
	}
}
