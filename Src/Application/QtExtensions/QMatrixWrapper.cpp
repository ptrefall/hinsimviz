#include "QMatrixWrapper.h"
#include "ObjectQtWrapper.h"

QMatrixWrapper::QMatrixWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<CL_Mat3f> property)
: slotChanged(false), propChanged(false), matType(3)
{
	this->qObj = qObj;
	init_mat4values = NULL;
	mat3Prop = property;

	for(unsigned int i = 0; i < 3*3; i++)
	{
		QDoubleSpinBox *spinbox = new QDoubleSpinBox();
		spinbox->setValue((double)mat3Prop.Get()[i]);
		spinboxes.push_back(spinbox);
	}

	slotPropertyChanged = mat3Prop.ValueChanged().connect(this, &QMatrixWrapper::On3PropertyChanged);
	
	connect(spinboxes[0], SIGNAL( valueChanged(double) ), this, SLOT( slot1ValueChanged(double) ));
	connect(spinboxes[1], SIGNAL( valueChanged(double) ), this, SLOT( slot2ValueChanged(double) ));
	connect(spinboxes[2], SIGNAL( valueChanged(double) ), this, SLOT( slot3ValueChanged(double) ));
	connect(spinboxes[3], SIGNAL( valueChanged(double) ), this, SLOT( slot4ValueChanged(double) ));
	connect(spinboxes[4], SIGNAL( valueChanged(double) ), this, SLOT( slot5ValueChanged(double) ));
	connect(spinboxes[5], SIGNAL( valueChanged(double) ), this, SLOT( slot6ValueChanged(double) ));
	connect(spinboxes[6], SIGNAL( valueChanged(double) ), this, SLOT( slot7ValueChanged(double) ));
	connect(spinboxes[7], SIGNAL( valueChanged(double) ), this, SLOT( slot8ValueChanged(double) ));
	connect(spinboxes[8], SIGNAL( valueChanged(double) ), this, SLOT( slot9ValueChanged(double) ));
}

QMatrixWrapper::QMatrixWrapper(ObjectQtWrapper *qObj, Engine::Entity::Property<CL_Mat4f> property)
: slotChanged(false), propChanged(false), matType(4)
{
	this->qObj = qObj;
	init_mat4values = new float[16];
	mat4Prop = property;

	for(unsigned int i = 0; i < 4*4; i++)
	{
		QDoubleSpinBox *spinbox = new QDoubleSpinBox();
		spinbox->setValue((double)mat4Prop.Get()[i]);
		spinboxes.push_back(spinbox);
	}

	slotPropertyChanged = mat4Prop.ValueChanged().connect(this, &QMatrixWrapper::On4PropertyChanged);
	
	connect(spinboxes[0],  SIGNAL( valueChanged(double) ), this, SLOT( slot1ValueChanged(double) ));
	connect(spinboxes[1],  SIGNAL( valueChanged(double) ), this, SLOT( slot2ValueChanged(double) ));
	connect(spinboxes[2],  SIGNAL( valueChanged(double) ), this, SLOT( slot3ValueChanged(double) ));
	connect(spinboxes[3],  SIGNAL( valueChanged(double) ), this, SLOT( slot4ValueChanged(double) ));
	connect(spinboxes[4],  SIGNAL( valueChanged(double) ), this, SLOT( slot5ValueChanged(double) ));
	connect(spinboxes[5],  SIGNAL( valueChanged(double) ), this, SLOT( slot6ValueChanged(double) ));
	connect(spinboxes[6],  SIGNAL( valueChanged(double) ), this, SLOT( slot7ValueChanged(double) ));
	connect(spinboxes[7],  SIGNAL( valueChanged(double) ), this, SLOT( slot8ValueChanged(double) ));
	connect(spinboxes[8],  SIGNAL( valueChanged(double) ), this, SLOT( slot9ValueChanged(double) ));
	connect(spinboxes[9],  SIGNAL( valueChanged(double) ), this, SLOT( slot10ValueChanged(double) ));
	connect(spinboxes[10], SIGNAL( valueChanged(double) ), this, SLOT( slot11ValueChanged(double) ));
	connect(spinboxes[11], SIGNAL( valueChanged(double) ), this, SLOT( slot12ValueChanged(double) ));
	connect(spinboxes[12], SIGNAL( valueChanged(double) ), this, SLOT( slot13ValueChanged(double) ));
	connect(spinboxes[13], SIGNAL( valueChanged(double) ), this, SLOT( slot14ValueChanged(double) ));
	connect(spinboxes[14], SIGNAL( valueChanged(double) ), this, SLOT( slot15ValueChanged(double) ));
	connect(spinboxes[15], SIGNAL( valueChanged(double) ), this, SLOT( slot16ValueChanged(double) ));
}

QMatrixWrapper::~QMatrixWrapper()
{
	if(init_mat4values)
		delete init_mat4values;
}

void QMatrixWrapper::On3PropertyChanged(const CL_Mat3f &oldValue, const CL_Mat3f &newValue)
{
	if(!qObj->isSelected())
		return;

	if(!slotChanged)
	{
		propChanged = true;
		for(unsigned int i = 0; i < 3*3; i++)
		{
			spinboxes[i]->setValue((double)mat3Prop.Get()[i]);
		}
	}
	else
	{
		slotChanged = false;
	}
}

void QMatrixWrapper::On4PropertyChanged(const CL_Mat4f &oldValue, const CL_Mat4f &newValue)
{
	if(!qObj->isSelected())
		return;

	if(!slotChanged)
	{
		propChanged = true;
		for(unsigned int i = 0; i < 4*4; i++)
		{
			spinboxes[i]->setValue((double)mat4Prop.Get()[i]);
		}
	}
	else
	{
		slotChanged = false;
	}
}

void QMatrixWrapper::slot1ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(matType == 3)
		{
			mat3Prop = CL_Mat3f((float)value, mat3Prop.Get()[1], mat3Prop.Get()[2], 
								 mat3Prop.Get()[3], mat3Prop.Get()[4], mat3Prop.Get()[5], 
								 mat3Prop.Get()[6], mat3Prop.Get()[7], mat3Prop.Get()[8]);
		}
		else if(matType == 4)
		{
			for(unsigned int i = 0; i < 4*4; i++)
			{
				if(i == 0)
					init_mat4values[i] = (float)value;
				else
					init_mat4values[i] = mat4Prop.Get()[i];
			}
			mat4Prop = CL_Mat4f(init_mat4values);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QMatrixWrapper::slot2ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(matType == 3)
		{
			mat3Prop = CL_Mat3f(mat3Prop.Get()[0], (float)value, mat3Prop.Get()[2], 
								 mat3Prop.Get()[3], mat3Prop.Get()[4], mat3Prop.Get()[5], 
								 mat3Prop.Get()[6], mat3Prop.Get()[7], mat3Prop.Get()[8]);
		}
		else if(matType == 4)
		{
			for(unsigned int i = 0; i < 4*4; i++)
			{
				if(i == 1)
					init_mat4values[i] = (float)value;
				else
					init_mat4values[i] = mat4Prop.Get()[i];
			}
			mat4Prop = CL_Mat4f(init_mat4values);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QMatrixWrapper::slot3ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(matType == 3)
		{
			mat3Prop = CL_Mat3f(mat3Prop.Get()[0], mat3Prop.Get()[1], (float)value,
								 mat3Prop.Get()[3], mat3Prop.Get()[4], mat3Prop.Get()[5], 
								 mat3Prop.Get()[6], mat3Prop.Get()[7], mat3Prop.Get()[8]);
		}
		else if(matType == 4)
		{
			for(unsigned int i = 0; i < 4*4; i++)
			{
				if(i == 2)
					init_mat4values[i] = (float)value;
				else
					init_mat4values[i] = mat4Prop.Get()[i];
			}
			mat4Prop = CL_Mat4f(init_mat4values);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QMatrixWrapper::slot4ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(matType == 3)
		{
			mat3Prop = CL_Mat3f(mat3Prop.Get()[0], mat3Prop.Get()[1], mat3Prop.Get()[2], 
								 (float)value, mat3Prop.Get()[4], mat3Prop.Get()[5], 
								 mat3Prop.Get()[6], mat3Prop.Get()[7], mat3Prop.Get()[8]);
		}
		else if(matType == 4)
		{
			for(unsigned int i = 0; i < 4*4; i++)
			{
				if(i == 3)
					init_mat4values[i] = (float)value;
				else
					init_mat4values[i] = mat4Prop.Get()[i];
			}
			mat4Prop = CL_Mat4f(init_mat4values);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QMatrixWrapper::slot5ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(matType == 3)
		{
			mat3Prop = CL_Mat3f(mat3Prop.Get()[0], mat3Prop.Get()[1], mat3Prop.Get()[2], 
								 mat3Prop.Get()[3], (float)value, mat3Prop.Get()[5], 
								 mat3Prop.Get()[6], mat3Prop.Get()[7], mat3Prop.Get()[8]);
		}
		else if(matType == 4)
		{
			for(unsigned int i = 0; i < 4*4; i++)
			{
				if(i == 4)
					init_mat4values[i] = (float)value;
				else
					init_mat4values[i] = mat4Prop.Get()[i];
			}
			mat4Prop = CL_Mat4f(init_mat4values);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QMatrixWrapper::slot6ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(matType == 3)
		{
			mat3Prop = CL_Mat3f(mat3Prop.Get()[0], mat3Prop.Get()[1], mat3Prop.Get()[2], 
								 mat3Prop.Get()[3], mat3Prop.Get()[4], (float)value, 
								 mat3Prop.Get()[6], mat3Prop.Get()[7], mat3Prop.Get()[8]);
		}
		else if(matType == 4)
		{
			for(unsigned int i = 0; i < 4*4; i++)
			{
				if(i == 5)
					init_mat4values[i] = (float)value;
				else
					init_mat4values[i] = mat4Prop.Get()[i];
			}
			mat4Prop = CL_Mat4f(init_mat4values);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QMatrixWrapper::slot7ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(matType == 3)
		{
			mat3Prop = CL_Mat3f(mat3Prop.Get()[0], mat3Prop.Get()[1], mat3Prop.Get()[2], 
								 mat3Prop.Get()[3], mat3Prop.Get()[4], mat3Prop.Get()[5], 
								 (float)value, mat3Prop.Get()[7], mat3Prop.Get()[8]);
		}
		else if(matType == 4)
		{
			for(unsigned int i = 0; i < 4*4; i++)
			{
				if(i == 6)
					init_mat4values[i] = (float)value;
				else
					init_mat4values[i] = mat4Prop.Get()[i];
			}
			mat4Prop = CL_Mat4f(init_mat4values);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QMatrixWrapper::slot8ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(matType == 3)
		{
			mat3Prop = CL_Mat3f(mat3Prop.Get()[0], mat3Prop.Get()[1], mat3Prop.Get()[2], 
								 mat3Prop.Get()[3], mat3Prop.Get()[4], mat3Prop.Get()[5], 
								 mat3Prop.Get()[6], (float)value, mat3Prop.Get()[8]);
		}
		else if(matType == 4)
		{
			for(unsigned int i = 0; i < 4*4; i++)
			{
				if(i == 7)
					init_mat4values[i] = (float)value;
				else
					init_mat4values[i] = mat4Prop.Get()[i];
			}
			mat4Prop = CL_Mat4f(init_mat4values);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QMatrixWrapper::slot9ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(matType == 3)
		{
			mat3Prop = CL_Mat3f(mat3Prop.Get()[0], mat3Prop.Get()[1], mat3Prop.Get()[2], 
								 mat3Prop.Get()[3], mat3Prop.Get()[4], mat3Prop.Get()[5], 
								 mat3Prop.Get()[6], mat3Prop.Get()[7], (float)value);
		}
		else if(matType == 4)
		{
			for(unsigned int i = 0; i < 4*4; i++)
			{
				if(i == 8)
					init_mat4values[i] = (float)value;
				else
					init_mat4values[i] = mat4Prop.Get()[i];
			}
			mat4Prop = CL_Mat4f(init_mat4values);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QMatrixWrapper::slot10ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(matType == 4)
		{
			for(unsigned int i = 0; i < 4*4; i++)
			{
				if(i == 9)
					init_mat4values[i] = (float)value;
				else
					init_mat4values[i] = mat4Prop.Get()[i];
			}
			mat4Prop = CL_Mat4f(init_mat4values);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QMatrixWrapper::slot11ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(matType == 4)
		{
			for(unsigned int i = 0; i < 4*4; i++)
			{
				if(i == 10)
					init_mat4values[i] = (float)value;
				else
					init_mat4values[i] = mat4Prop.Get()[i];
			}
			mat4Prop = CL_Mat4f(init_mat4values);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QMatrixWrapper::slot12ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(matType == 4)
		{
			for(unsigned int i = 0; i < 4*4; i++)
			{
				if(i == 11)
					init_mat4values[i] = (float)value;
				else
					init_mat4values[i] = mat4Prop.Get()[i];
			}
			mat4Prop = CL_Mat4f(init_mat4values);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QMatrixWrapper::slot13ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(matType == 4)
		{
			for(unsigned int i = 0; i < 4*4; i++)
			{
				if(i == 12)
					init_mat4values[i] = (float)value;
				else
					init_mat4values[i] = mat4Prop.Get()[i];
			}
			mat4Prop = CL_Mat4f(init_mat4values);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QMatrixWrapper::slot14ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(matType == 4)
		{
			for(unsigned int i = 0; i < 4*4; i++)
			{
				if(i == 13)
					init_mat4values[i] = (float)value;
				else
					init_mat4values[i] = mat4Prop.Get()[i];
			}
			mat4Prop = CL_Mat4f(init_mat4values);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QMatrixWrapper::slot15ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(matType == 4)
		{
			for(unsigned int i = 0; i < 4*4; i++)
			{
				if(i == 14)
					init_mat4values[i] = (float)value;
				else
					init_mat4values[i] = mat4Prop.Get()[i];
			}
			mat4Prop = CL_Mat4f(init_mat4values);
		}
	}
	else
	{
		propChanged = false;
	}
}

void QMatrixWrapper::slot16ValueChanged(double value)
{
	if(!propChanged)
	{
		slotChanged = true;
		if(matType == 4)
		{
			for(unsigned int i = 0; i < 4*4; i++)
			{
				if(i == 15)
					init_mat4values[i] = (float)value;
				else
					init_mat4values[i] = mat4Prop.Get()[i];
			}
			mat4Prop = CL_Mat4f(init_mat4values);
		}
	}
	else
	{
		propChanged = false;
	}
}
