#pragma once

#include "IWrapper.h"
#include <ClanLib/core.h>
#include <QtGui/QDoubleSpinBox>
#include <Depends/Entity/Property.h>
#include <vector>

class ObjectQtWrapper;

class QMatrixWrapper : public IWrapper
{
public:
	QMatrixWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<CL_Mat3f> property);
	QMatrixWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<CL_Mat4f> property);
	virtual ~QMatrixWrapper();

	QDoubleSpinBox *getQDoubleSpinBox(int i) const 
	{ 
		if(i > (int)spinboxes.size()-1 || i < 0)
			return NULL;
		else 
			return spinboxes[i];
	}
	Engine::Entity::Property<CL_Mat3f> get3Property() { return mat3Prop; }
	Engine::Entity::Property<CL_Mat4f> get4Property() { return mat4Prop; }
	
	int getMatType() const { return matType; }

private:
	void On3PropertyChanged(const CL_Mat3f &oldValue, const CL_Mat3f &newValue);
	void On4PropertyChanged(const CL_Mat4f &oldValue, const CL_Mat4f &newValue);
	CL_Slot slotPropertyChanged;
	std::vector<QDoubleSpinBox*> spinboxes;
	Engine::Entity::Property<CL_Mat3f> mat3Prop;
	Engine::Entity::Property<CL_Mat4f> mat4Prop;

	ObjectQtWrapper *qObj;

	bool slotChanged;
	bool propChanged;

	float *init_mat4values;
	int matType;

public slots:
	virtual void slot1ValueChanged(double value);
	virtual void slot2ValueChanged(double value);
	virtual void slot3ValueChanged(double value);
	virtual void slot4ValueChanged(double value);
	virtual void slot5ValueChanged(double value);
	virtual void slot6ValueChanged(double value);
	virtual void slot7ValueChanged(double value);
	virtual void slot8ValueChanged(double value);
	virtual void slot9ValueChanged(double value);
	virtual void slot10ValueChanged(double value);
	virtual void slot11ValueChanged(double value);
	virtual void slot12ValueChanged(double value);
	virtual void slot13ValueChanged(double value);
	virtual void slot14ValueChanged(double value);
	virtual void slot15ValueChanged(double value);
	virtual void slot16ValueChanged(double value);
};
