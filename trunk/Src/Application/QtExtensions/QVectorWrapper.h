#pragma once

#include "IWrapper.h"
#include <ClanLib/core.h>
#include <QtGui/QDoubleSpinBox>
#include <Depends/Entity/Property.h>

class ObjectQtWrapper;

class QVectorWrapper : public IWrapper
{
public:
	QVectorWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<CL_Vec2f> property);
	QVectorWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<CL_Vec3f> property);
	QVectorWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<CL_Vec4f> property);
	virtual ~QVectorWrapper();

	QDoubleSpinBox *getQDoubleSpinBox(int i) const 
	{ 
		if(i == 0) return spinbox1;
		else if(i == 1) return spinbox2;
		else if(i == 2) return spinbox3;
		else if(i == 3) return spinbox4;
		else return NULL;
	}
	Engine::Entity::Property<CL_Vec2f> get2Property() { return vec2Prop; }
	Engine::Entity::Property<CL_Vec3f> get3Property() { return vec3Prop; }
	Engine::Entity::Property<CL_Vec4f> get4Property() { return vec4Prop; }

	int getVecType() const { return vecType; }

private:
	void On2PropertyChanged(const CL_Vec2f &oldValue, const CL_Vec2f &newValue);
	void On3PropertyChanged(const CL_Vec3f &oldValue, const CL_Vec3f &newValue);
	void On4PropertyChanged(const CL_Vec4f &oldValue, const CL_Vec4f &newValue);
	CL_Slot slotPropertyChanged;
	QDoubleSpinBox *spinbox1;
	QDoubleSpinBox *spinbox2;
	QDoubleSpinBox *spinbox3;
	QDoubleSpinBox *spinbox4;
	Engine::Entity::Property<CL_Vec2f> vec2Prop;
	Engine::Entity::Property<CL_Vec3f> vec3Prop;
	Engine::Entity::Property<CL_Vec4f> vec4Prop;

	ObjectQtWrapper *qObj;

	bool slotChanged;
	bool propChanged;

	int vecType;

public slots:
	virtual void slot1ValueChanged(double value);
	virtual void slot2ValueChanged(double value);
	virtual void slot3ValueChanged(double value);
	virtual void slot4ValueChanged(double value);
};
