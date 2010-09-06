#pragma once

#include <GL/GLee.h>
#include <QtGui/QMainWindow>
#include <QtGui/QAction>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>

namespace Engine { namespace Scene { class Object; }}
class MainWindow;

class ObjectQtWrapper : public QObject
{
Q_OBJECT
public:
    ObjectQtWrapper(Engine::Scene::Object *obj, MainWindow *wnd);
    virtual ~ObjectQtWrapper();

	QAction *getAction() const { return action; }

private:
	Engine::Scene::Object *obj;
	QAction *action;
	MainWindow *wnd;

public slots:
	void slotObjectClicked();
};