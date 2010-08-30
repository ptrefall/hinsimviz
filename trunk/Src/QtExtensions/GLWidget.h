#pragma once

#include <QtOpenGL>
#include <Engine/GUI/IGuiManager.h>

namespace Core { class CoreManager; }

class GLWidget : public QGLWidget, public GUI::IGuiManager
{
public:
	GLWidget( QWidget *parent = 0 );
	virtual ~GLWidget();

	void init();

	virtual void center(int &x, int &y);

protected:
	virtual void glDraw();
	virtual void initializeGL();
	virtual void paintEvent( QPaintEvent* e );
	virtual void paintGL();
	virtual void paintOverlayGL();
	virtual void resizeGL( int w, int h );
	virtual void timerEvent( QTimerEvent *qte );

	//virtual void swapBuffers();

private:
	virtual void mouseMoveEvent(QMouseEvent *);

	Core::CoreManager *coreManager;
};