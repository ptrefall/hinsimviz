#include "ObjectQtWrapper.h"
#include "MainWindow.h"
#include <ClanLib/core.h>
#include <Engine/Scene/Object.h>

ObjectQtWrapper::ObjectQtWrapper(Engine::Scene::Object *obj, MainWindow *wnd)
: QObject(), text(NULL)
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
	if(wnd->propertiesLayout->children().empty() == false)
	{
		QWidget *textLabel = wnd->propertiesLayout->findChild<QWidget*>("TextLabel");
		if(textLabel)
			wnd->propertiesLayout->removeWidget(textLabel);
	}

	if(text == NULL)
	{
		text = new QLabel(cl_format("Name: %1", obj->getName()).c_str());
		text->setObjectName("TextLabel");
	}
	wnd->propertiesLayout->addWidget(text, 1, 1, 1, 1);

	wnd->properties->repaint();
}
