#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
{
    setupUi(this);
	connect(action_close, SIGNAL( triggered() ), this, SLOT( slotClose() ));
}
MainWindow::~MainWindow()
{
}

void MainWindow::init()
{
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
