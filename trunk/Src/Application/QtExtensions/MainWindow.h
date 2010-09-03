#pragma once

#include <GL/GLee.h>
#include <Engine/GUI/IMainGuiManager.h>

#include <QtGui/QMainWindow>
#include <QtTemp/moc/moc_UiMainWindow.h>

namespace Engine { namespace Core { class CoreManager; }}

class MainWindow : public QMainWindow, private Ui::MainWindow, public Engine::GUI::IMainGuiManager
{
Q_OBJECT
public:
    MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    virtual ~MainWindow();

	void init(int argc, char *argv[]);
	void update();
	void mouseMoveEventP(QMouseEvent *event, int cx, int cy);

	virtual void setCaptionText(const char *text);
	virtual void addObject(const char *name);

private:
	virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);

	Engine::Core::CoreManager *coreMgr;

private slots:
    void slotClose();
	void slotObjectClicked();
};