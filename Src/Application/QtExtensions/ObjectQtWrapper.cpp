#include "ObjectQtWrapper.h"
#include "MainWindow.h"
#include <ClanLib/core.h>
#include <Engine/Scene/Object.h>
#include <Depends/Entity/IProperty.h>
#include <Depends/Entity/Property.h>
#include <QtGui/QSpinBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include "QCheckBoxWrapper.h"
#include "QDoubleSpinBoxWrapper.h"
#include "QSpinBoxWrapper.h"
#include "QVectorWrapper.h"
#include "QMatrixWrapper.h"
#include "QLineEditWrapper.h"

ObjectQtWrapper::ObjectQtWrapper(Engine::Scene::Object *obj, MainWindow *wnd)
: QObject(), selected(false)
{
	this->obj = obj;
	this->wnd = wnd;
	action = new QAction(this);
	action->setObjectName(cl_format("Action%1", obj->getName()).c_str());
	action->setText(obj->getName().c_str());
	connect(action, SIGNAL( triggered() ), this, SLOT( slotObjectClicked() ));
}
ObjectQtWrapper::~ObjectQtWrapper()
{
	for(unsigned int i = 0; i < wrappers.size(); i++)
	{
		delete wrappers[i];
		wrappers[i] = NULL;
	}
}

void ObjectQtWrapper::slotObjectClicked()
{
	std::cout << "Object " << obj->getName().c_str() << " clicked!" << std::endl;

	wnd->setSelectedObj(this);

	if(wnd->propertiesLayout)
	{
		
		wnd->gridLayout_3->removeItem(wnd->propertiesLayout);
		wnd->propertiesLayout->setParent(NULL);
		delete wnd->propertiesLayout;
		wnd->propertiesLayout = NULL;
	}

	if(wnd->propertiesContents)
	{
		wnd->properties->setWidget(NULL);
		wnd->propertiesContents->setParent(NULL);
		delete wnd->propertiesContents;
		wnd->propertiesContents = NULL;
	}

	for(unsigned int i = 0; i < wrappers.size(); i++)
	{
		delete wrappers[i];
		wrappers[i] = NULL;
	}
	wrappers.clear();

	wnd->propertiesContents = new QWidget();
    wnd->propertiesContents->setObjectName(QString::fromUtf8("propertiesContents"));
	
	wnd->gridLayout_3 = new QGridLayout(wnd->propertiesContents);
    wnd->gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
    
	wnd->properties->setWidget(wnd->propertiesContents);

	wnd->propertiesLayout = new QGridLayout();
    wnd->propertiesLayout->setContentsMargins(2, 2, 2, 2);
    wnd->propertiesLayout->setObjectName(QString::fromUtf8("propertiesLayout"));
	wnd->gridLayout_3->addLayout(wnd->propertiesLayout, 0, 0, 1, 1);

	QVBoxLayout *vLayout = new QVBoxLayout();
	
	{
		QHBoxLayout *hLayout = new QHBoxLayout();
		hLayout->addWidget(new QLabel("Name:"));
		hLayout->addWidget(new QLabel(obj->getName().c_str()));
		vLayout->addLayout(hLayout);
	}
	{
		QGroupBox *group = new QGroupBox();
		QScrollArea *scrollArea = new QScrollArea();
		QVBoxLayout *groupLayout = new QVBoxLayout();
		group->setLayout(groupLayout);
		group->setTitle("Properties");
		groupLayout->addWidget(scrollArea);
		QVBoxLayout *gLayout = new QVBoxLayout();
		scrollArea->setLayout(gLayout);

		std::map<CL_String, Engine::Entity::IProperty*> &properties = obj->GetProperties();
		std::map<CL_String, Engine::Entity::IProperty*>::iterator it = properties.begin();
		while(it != properties.end())
		{
			Engine::Entity::IProperty *property = it->second;
			QHBoxLayout *hLayout = new QHBoxLayout();
			hLayout->addWidget(new QLabel(property->GetName().c_str()));

			if(property->GetTypeId() == Engine::Entity::TypeSerializer::TYPE_BOOL)
			{
				Engine::Entity::Property<bool> *prop = static_cast<Engine::Entity::Property<bool>*>(property);
				QCheckBoxWrapper *checkbox = new QCheckBoxWrapper(this, *prop);
				wrappers.push_back(checkbox);
				hLayout->addWidget(checkbox->getQCheckBox());
			}
			else if(property->GetTypeId() == Engine::Entity::TypeSerializer::TYPE_DOUBLE)
			{
				Engine::Entity::Property<double> *prop = static_cast<Engine::Entity::Property<double>*>(property);
				QDoubleSpinBoxWrapper *spinbox = new QDoubleSpinBoxWrapper(this, *prop);
				wrappers.push_back(spinbox);
				hLayout->addWidget(spinbox->getQDoubleSpinBox());
			}
			else if(property->GetTypeId() == Engine::Entity::TypeSerializer::TYPE_FLOAT)
			{
				Engine::Entity::Property<float> *prop = static_cast<Engine::Entity::Property<float>*>(property);
				QDoubleSpinBoxWrapper *spinbox = new QDoubleSpinBoxWrapper(this, *prop);
				wrappers.push_back(spinbox);
				hLayout->addWidget(spinbox->getQDoubleSpinBox());
			}
			else if(property->GetTypeId() == Engine::Entity::TypeSerializer::TYPE_INT)
			{
				Engine::Entity::Property<int> *prop = static_cast<Engine::Entity::Property<int>*>(property);
				QSpinBoxWrapper* spinbox = new QSpinBoxWrapper(this, *prop);
				wrappers.push_back(spinbox);
				hLayout->addWidget(spinbox->getQSpinBox());
			}
			else if(property->GetTypeId() == Engine::Entity::TypeSerializer::TYPE_MAT3)
			{
				Engine::Entity::Property<CL_Mat3f> *prop = static_cast<Engine::Entity::Property<CL_Mat3f>*>(property);
				QMatrixWrapper *qMat = new QMatrixWrapper(this, *prop);
				wrappers.push_back(qMat);

				QVBoxLayout *vLayoutMat = new QVBoxLayout();
				{
					QHBoxLayout *hLayoutMat1 = new QHBoxLayout();
					{
						hLayoutMat1->addWidget(qMat->getQDoubleSpinBox(0));
						hLayoutMat1->addWidget(qMat->getQDoubleSpinBox(1));
						hLayoutMat1->addWidget(qMat->getQDoubleSpinBox(2));
					}
					QHBoxLayout *hLayoutMat2 = new QHBoxLayout();
					{
						hLayoutMat2->addWidget(qMat->getQDoubleSpinBox(3));
						hLayoutMat2->addWidget(qMat->getQDoubleSpinBox(4));
						hLayoutMat2->addWidget(qMat->getQDoubleSpinBox(5));
					}
					QHBoxLayout *hLayoutMat3 = new QHBoxLayout();
					{
						hLayoutMat3->addWidget(qMat->getQDoubleSpinBox(6));
						hLayoutMat3->addWidget(qMat->getQDoubleSpinBox(7));
						hLayoutMat3->addWidget(qMat->getQDoubleSpinBox(8));
					}
					vLayoutMat->addLayout(hLayoutMat1);
					vLayoutMat->addLayout(hLayoutMat2);
					vLayoutMat->addLayout(hLayoutMat3);
				}
				hLayout->addLayout(vLayoutMat);
			}
			else if(property->GetTypeId() == Engine::Entity::TypeSerializer::TYPE_MAT4)
			{
				Engine::Entity::Property<CL_Mat4f> *prop = static_cast<Engine::Entity::Property<CL_Mat4f>*>(property);
				QMatrixWrapper *qMat = new QMatrixWrapper(this, *prop);
				wrappers.push_back(qMat);

				QVBoxLayout *vLayoutMat = new QVBoxLayout();
				{
					QHBoxLayout *hLayoutMat1 = new QHBoxLayout();
					{
						hLayoutMat1->addWidget(qMat->getQDoubleSpinBox(0));
						hLayoutMat1->addWidget(qMat->getQDoubleSpinBox(1));
						hLayoutMat1->addWidget(qMat->getQDoubleSpinBox(2));
						hLayoutMat1->addWidget(qMat->getQDoubleSpinBox(3));
					}
					QHBoxLayout *hLayoutMat2 = new QHBoxLayout();
					{
						hLayoutMat2->addWidget(qMat->getQDoubleSpinBox(4));
						hLayoutMat2->addWidget(qMat->getQDoubleSpinBox(5));
						hLayoutMat2->addWidget(qMat->getQDoubleSpinBox(6));
						hLayoutMat2->addWidget(qMat->getQDoubleSpinBox(7));
					}
					QHBoxLayout *hLayoutMat3 = new QHBoxLayout();
					{
						hLayoutMat3->addWidget(qMat->getQDoubleSpinBox(8));
						hLayoutMat3->addWidget(qMat->getQDoubleSpinBox(9));
						hLayoutMat3->addWidget(qMat->getQDoubleSpinBox(10));
						hLayoutMat3->addWidget(qMat->getQDoubleSpinBox(11));
					}
					QHBoxLayout *hLayoutMat4 = new QHBoxLayout();
					{
						hLayoutMat4->addWidget(qMat->getQDoubleSpinBox(12));
						hLayoutMat4->addWidget(qMat->getQDoubleSpinBox(13));
						hLayoutMat4->addWidget(qMat->getQDoubleSpinBox(14));
						hLayoutMat4->addWidget(qMat->getQDoubleSpinBox(15));
					}
					
					vLayoutMat->addLayout(hLayoutMat1);
					vLayoutMat->addLayout(hLayoutMat2);
					vLayoutMat->addLayout(hLayoutMat3);
					vLayoutMat->addLayout(hLayoutMat4);
				}
				hLayout->addLayout(vLayoutMat);
			}
			else if(property->GetTypeId() == Engine::Entity::TypeSerializer::TYPE_STRING)
			{
				Engine::Entity::Property<CL_String> *prop = static_cast<Engine::Entity::Property<CL_String>*>(property);
				QLineEditWrapper* lineedit = new QLineEditWrapper(this, *prop);
				wrappers.push_back(lineedit);
				hLayout->addWidget(lineedit->getQLineEdit());
			}
			else if(property->GetTypeId() == Engine::Entity::TypeSerializer::TYPE_UNSIGNED_INT)
			{
				Engine::Entity::Property<unsigned int> *prop = static_cast<Engine::Entity::Property<unsigned int>*>(property);
				QSpinBoxWrapper* spinbox = new QSpinBoxWrapper(this, *prop);
				wrappers.push_back(spinbox);
				hLayout->addWidget(spinbox->getQSpinBox());
			}
			else if(property->GetTypeId() == Engine::Entity::TypeSerializer::TYPE_VEC2)
			{
				Engine::Entity::Property<CL_Vec2f> *prop = static_cast<Engine::Entity::Property<CL_Vec2f>*>(property);
				QVectorWrapper *qVec = new QVectorWrapper(this, *prop);
				wrappers.push_back(qVec);
				hLayout->addWidget(qVec->getQDoubleSpinBox(0));
				hLayout->addWidget(qVec->getQDoubleSpinBox(1));
			}
			else if(property->GetTypeId() == Engine::Entity::TypeSerializer::TYPE_VEC3)
			{
				Engine::Entity::Property<CL_Vec3f> *prop = static_cast<Engine::Entity::Property<CL_Vec3f>*>(property);
				QVectorWrapper *qVec = new QVectorWrapper(this, *prop);
				wrappers.push_back(qVec);
				hLayout->addWidget(qVec->getQDoubleSpinBox(0));
				hLayout->addWidget(qVec->getQDoubleSpinBox(1));
				hLayout->addWidget(qVec->getQDoubleSpinBox(2));
			}
			else if(property->GetTypeId() == Engine::Entity::TypeSerializer::TYPE_VEC4)
			{
				Engine::Entity::Property<CL_Vec4f> *prop = static_cast<Engine::Entity::Property<CL_Vec4f>*>(property);
				QVectorWrapper *qVec = new QVectorWrapper(this, *prop);
				wrappers.push_back(qVec);
				hLayout->addWidget(qVec->getQDoubleSpinBox(0));
				hLayout->addWidget(qVec->getQDoubleSpinBox(1));
				hLayout->addWidget(qVec->getQDoubleSpinBox(2));
				hLayout->addWidget(qVec->getQDoubleSpinBox(3));
			}
			gLayout->addLayout(hLayout);
			++it;
		}
		vLayout->addWidget(group);
	}
	wnd->propertiesLayout->addLayout(vLayout, 1, 1);
}
