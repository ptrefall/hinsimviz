#pragma once

#include <GL/GLee.h>
#include <Engine/GUI/IMainGuiManager.h>

#include <QtGui/QMainWindow>
#include <QtTemp/moc/moc_UiMainWindow.h>

namespace Engine { 
	namespace Core { class CoreManager; }
	namespace Scene { class Object; }
}

class MainWindow : public QMainWindow, public Ui::MainWindow, public Engine::GUI::IMainGuiManager
{
Q_OBJECT
public:
    MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    virtual ~MainWindow();

	void init(int argc, char *argv[]);
	void update();
	void mouseMoveEventP(QMouseEvent *event, int cx, int cy);

	virtual void setCaptionText(const char *text);
	virtual void addObject(const char *name, Engine::Scene::Object *obj);

	QGridLayout *propertiesLayout;

private:
	virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);

	Engine::Core::CoreManager *coreMgr;

private slots:
    void slotClose();
};