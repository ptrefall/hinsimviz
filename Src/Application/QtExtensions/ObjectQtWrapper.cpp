#include "ObjectQtWrapper.h"
#include "MainWindow.h"
#include <ClanLib/core.h>
#include <Engine/Scene/Object.h>

ObjectQtWrapper::ObjectQtWrapper(Engine::Scene::Object *obj, MainWindow *wnd)
: QObject()
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
}

void ObjectQtWrapper::slotObjectClicked()
{
	std::cout << "Object " << obj->getName().c_str() << " clicked!" << std::endl;
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

	wnd->propertiesContents = new QWidget();
    wnd->propertiesContents->setObjectName(QString::fromUtf8("propertiesContents"));
	
	wnd->gridLayout_3 = new QGridLayout(wnd->propertiesContents);
    wnd->gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
    
	wnd->properties->setWidget(wnd->propertiesContents);

	wnd->propertiesLayout = new QGridLayout();
    wnd->propertiesLayout->setContentsMargins(2, 2, 2, 2);
    wnd->propertiesLayout->setObjectName(QString::fromUtf8("propertiesLayout"));
	wnd->gridLayout_3->addLayout(wnd->propertiesLayout, 0, 0, 1, 1);

	QLabel *text = new QLabel(cl_format("Name: %1", obj->getName()).c_str());
	text->setObjectName("TextLabel");
	wnd->propertiesLayout->addWidget(text, 1, 1, 1, 1);
}
