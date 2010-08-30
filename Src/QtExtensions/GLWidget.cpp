#include "GLWidget.h"
#include <Engine/Core/CoreManager.h>

GLWidget::GLWidget( QWidget *parent )
: QGLWidget( QGLFormat( QGL::AlphaChannel | QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::SampleBuffers | QGL::StereoBuffers), parent)
{
	//this->setMouseTracking(true);
	//this->setCursor( QCursor( Qt::BlankCursor ) );
	//this->showFullScreen();
	//this->grabMouse();

	coreManager = new Core::CoreManager(this);
}


GLWidget::~GLWidget() 
{
	//this->releaseMouse();
}

void GLWidget::init()
{
}

void GLWidget::center(int &x, int &y)
{
	QRect rect = QApplication::desktop()->availableGeometry();
	x = rect.center().x();
	y = rect.center().y();
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
	init();
	qglClearColor(QColor(127,127,127));
	initializeOverlayGL();
}


void GLWidget::paintEvent(QPaintEvent *e) 
{
	updateGL();
}


void GLWidget::paintGL() 
{
}

void GLWidget::paintOverlayGL()
{
}

void GLWidget::resizeGL(int w, int h) 
{
}


/*void GLWidget::swapBuffers() 
{
	QGLWidget::swapBuffers();
}*/


void GLWidget::timerEvent(QTimerEvent *e) 
{
	updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{	
}
