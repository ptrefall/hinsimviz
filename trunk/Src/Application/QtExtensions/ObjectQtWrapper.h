#pragma once

#include <GL/GLee.h>
#include <QtGui/QMainWindow>
#include <QtGui/QAction>

namespace Engine { namespace Scene { class Object; }}

class ObjectQtWrapper : public QObject
{
Q_OBJECT
public:
    ObjectQtWrapper(Engine::Scene::Object *obj);
    virtual ~ObjectQtWrapper();

	QAction *getAction() const { return action; }

private:
	Engine::Scene::Object *obj;
	QAction *action;

public slots:
	void slotObjectClicked();
};