#include "ObjectQtWrapper.h"
#include <ClanLib/core.h>
#include <Engine/Scene/Object.h>

ObjectQtWrapper::ObjectQtWrapper(Engine::Scene::Object *obj)
: QObject()
{
	this->obj = obj;
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
}
