#include "GLWidget.h"
#include <Engine/Core/CoreManager.h>

GLWidget::GLWidget( QWidget *parent )
: QGLWidget( QGLFormat( QGL::AlphaChannel | QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::SampleBuffers | QGL::StereoBuffers), parent)
{
	timerId = -1;
	//this->setMouseTracking(true);
	//this->setCursor( QCursor( Qt::BlankCursor ) );
	//this->showFullScreen();
	//this->grabMouse();
	coreMgr = NULL;
}


GLWidget::~GLWidget() 
{
	//this->releaseMouse();
	if(timerId != -1)
		killTimer(timerId);
}

void GLWidget::init(Engine::Core::CoreManager *coreMgr)
{
	this->coreMgr = coreMgr;
	timerId = startTimer(10);
}

void GLWidget::center(int &x, int &y)
{
	QRect rect = QApplication::desktop()->availableGeometry();
	x = rect.center().x();
	y = rect.center().y();
}

int GLWidget::getWidth() const
{
	return this->width();
}

int GLWidget::getHeight() const
{
	return this->height();
}

int GLWidget::getDesktopWidth() const
{
	return QApplication::desktop()->availableGeometry().width();
}

int GLWidget::getDesktopHeight() const
{
	return QApplication::desktop()->availableGeometry().height();
}

void GLWidget::glDraw() 
{
	makeCurrent();
	makeOverlayCurrent();
	paintGL();
	paintOverlayGL();
}

void GLWidget::initializeGL() 
{
	qglClearColor(QColor(127,127,127));
	initializeOverlayGL();
}


void GLWidget::paintEvent(QPaintEvent *e) 
{
	updateGL();
}


void GLWidget::paintGL() 
{
	if(coreMgr)
		coreMgr->frame();
}

void GLWidget::paintOverlayGL()
{
}

void GLWidget::resizeGL(int w, int h) 
{
	if(coreMgr)
		coreMgr->resize(w,h);
}


void GLWidget::swapBuffers() 
{
	QGLWidget::swapBuffers();
}


void GLWidget::timerEvent(QTimerEvent *e) 
{
	updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{	
}
