#include "MainWindow.h"
#include <Engine/Core/CoreManager.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags), coreMgr(NULL)
{
    setupUi(this);
	connect(action_close, SIGNAL( triggered() ), this, SLOT( slotClose() ));
}
MainWindow::~MainWindow()
{
	if(coreMgr)
		delete coreMgr;
}

void MainWindow::init(int argc, char *argv[])
{
	coreMgr = new Engine::Core::CoreManager(argc, argv, this, scene);
	scene->init(coreMgr);
}

void MainWindow::update()
{
}

void MainWindow::slotClose()
{
    // Ask for confirmation on clear
    QMessageBox::StandardButton choice = QMessageBox::question( this, "Quit?", "Sure you want to quit?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes );
    switch( choice )
    {
        case QMessageBox::No:
        {
            return;
        }break;
        case QMessageBox::Yes:
        default:
        {
            // Remove all Selections
            close();
        }break;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	//if(!gameEngine->keyEvent(event->key(), true))
	//	event->ignore();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
	//if(!gameEngine->keyEvent(event->key(), false))
	//	event->ignore();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
}

void MainWindow::mouseMoveEventP(QMouseEvent *event, int cx, int cy)
{
	//if(!gameEngine->mouseMoveEvent(event->x(), event->y(), cx,cy))
	//	event->ignore();
}

void MainWindow::setCaptionText(const char *text)
{
	
}

void MainWindow::addObject(const char *name)
{
	//QMenu *menu = new QMenu(name);
	QAction *action_object = new QAction(this);
	QString objName = "Action";
	objName += QString(name);
	action_object->setObjectName(objName);
	action_object->setText(name);
	connect(action_object, SIGNAL( clicked() ), this, SLOT( slotObjectClicked() ));
	this->menuObjects->addAction(action_object);
}

void MainWindow::slotObjectClicked()
{
	std::cout << "Object clicked!" << std::endl;
}
