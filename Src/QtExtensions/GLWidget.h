#pragma once

#include <QtOpenGL>

class GLWidget : public QGLWidget
{
public:
	GLWidget( QWidget *parent = 0 );
	virtual ~GLWidget();

	void init();

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
};