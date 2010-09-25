#pragma once

#include <GL/GLee.h>
#include <QtGui/QMainWindow>
#include <QtGui/QAction>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <vector>

namespace Engine { namespace Scene { class Object; }}
class MainWindow;
class IWrapper;

class ObjectQtWrapper : public QObject
{
Q_OBJECT
public:
    ObjectQtWrapper(Engine::Scene::Object *obj, MainWindow *wnd);
    virtual ~ObjectQtWrapper();

	QAction *getAction() const { return action; }
	void deselect() { selected = false; }
	void select() { selected = true; }
	bool isSelected() const { return selected; }

private:
	Engine::Scene::Object *obj;
	QAction *action;
	MainWindow *wnd;
	bool selected;
	std::vector<IWrapper*> wrappers;

public slots:
	void slotObjectClicked();
};