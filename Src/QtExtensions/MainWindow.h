#pragma once

#include <GL/GLee.h>

#include <QtGui/QMainWindow>
#include <QtTemp/moc/moc_UiMainWindow.h>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
Q_OBJECT
public:
    MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    virtual ~MainWindow();

	void init();
	void update();
	void mouseMoveEventP(QMouseEvent *event, int cx, int cy);

private:
	virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);

private slots:
    void slotClose();
};