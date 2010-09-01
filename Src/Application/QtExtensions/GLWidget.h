#pragma once

#include <QtOpenGL>
#include <Engine/GUI/IGuiManager.h>

namespace Engine { namespace Core { class CoreManager; }}

class GLWidget : public QGLWidget, public Engine::GUI::IGuiManager
{
public:
	GLWidget( QWidget *parent = 0 );
	virtual ~GLWidget();

	void init(int argc, char *argv[]);

	virtual void center(int &x, int &y);
	virtual int getWidth() const;
	virtual int getHeight() const;
	virtual int getDesktopWidth() const;
	virtual int getDesktopHeight() const;
	virtual void swapBuffers();

protected:
	virtual void glDraw();
	virtual void initializeGL();
	virtual void paintEvent( QPaintEvent* e );
	virtual void paintGL();
	virtual void paintOverlayGL();
	virtual void resizeGL( int w, int h );
	virtual void timerEvent( QTimerEvent *qte );

private:
	virtual void mouseMoveEvent(QMouseEvent *);

	Engine::Core::CoreManager *coreManager;
};