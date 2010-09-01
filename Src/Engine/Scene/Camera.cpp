#include "Camera.h"
#include "SceneManager.h"
#include "PostProcess.h"
#include <Core/CoreManager.h>
#include <GL/glew.h>

using namespace Engine;
using namespace Scene;

Camera::Camera(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Engine::Core::CoreManager *coreMgr)
: Engine::Scene::Node(id, colorId, type, name, coreMgr)
{
	renderable = false;
	opaque = true;
	drawMode = 0;

	projMatrix = CL_Mat4f::identity();
	viewMatrix = CL_Mat4f::identity();

	cameraState_property = this->AddProperty<int>("CameraState", Scene::CAM_STATIC);
	slotCameraStateChanged = cameraState_property.ValueChanged().connect(this, &Camera::OnCameraStateChanged);
}

Camera::~Camera()
{
}

int Camera::init()
{
	initialized = true;
	perspective(40.0f, 1.0f, 1.0f, 1000.0f);
	lookAt(CL_Vec3f(0.0f, 20.0f, 0.0f), 
		   CL_Vec3f(0.0f, 0.0f, -1000.0f), 
		   CL_Vec3f(0.0f, 1.0f, 0.0f));
	return 0;
}

int Camera::update(double dt)
{
	if(cameraState_property.Get() == Scene::CAM_STATIC)
	{
	}
	else if(cameraState_property.Get() == Scene::CAM_FIRST_PERSON)
	{
		if(parent)
		{
			const CL_Mat4f &model = parent->getModelMatrix();
			viewMatrix = CL_Mat4f::inverse(model);
		}
	}
	else if(cameraState_property.Get() == Scene::CAM_CHASE)
	{
		//Camera's object encapsulation
		if(parent)
		{
			//Parent of camera object
			if(parent->getParent())
			{
				CL_Mat4f model = getModelMatrix();
				CL_Mat4f parentModel = parent->getParent()->getModelMatrix();
				
				CL_Vec3f eye = CL_Vec3f(model[12], model[13], model[14]);
				CL_Vec3f target = CL_Vec3f(parentModel[12], parentModel[13], parentModel[14]);
				CL_Vec3f up = CL_Vec3f(0.0f, 1.0f, 0.0f);
				lookAt(eye, target, up);

				/*const CL_Mat4f &model = parent->getParent()->getModelMatrix();
				viewMatrix = CL_Mat4f::inverse(model);
				viewMatrix = viewMatrix.multiply(parent->getLocalTranslate());*/

			}
		}
	}
	return 0;
}

void Camera::OnCameraStateChanged(const int &oldValue, const int &newValue)
{
	std::cout << "Changed camera state to " << newValue << std::endl;
}

const CL_Mat4f &Camera::lookAt(const CL_Vec3f &eye, const CL_Vec3f &target, const CL_Vec3f &up)
{
	/*viewMatrix = viewMatrix.look_at(eye.x, eye.y, eye.z,
									target.x, target.y, target.z,
									up.x, up.y, up.z);*/

	// Builds a look-at style view matrix.
	// This is essentially the same matrix used by gluLookAt().
	
	CL_Vec3f zAxis = eye - target;
	zAxis.normalize();

	CL_Vec3f xAxis = CL_Vec3f::cross(up, zAxis);
	xAxis.normalize();

	CL_Vec3f yAxis = CL_Vec3f::cross(zAxis, xAxis);
	yAxis.normalize();

	viewMatrix[0] = xAxis.x;
	viewMatrix[4] = xAxis.y;
	viewMatrix[8] = xAxis.z;
	viewMatrix[12] = -CL_Vec3f::dot(xAxis, eye);

	viewMatrix[1] = yAxis.x;
	viewMatrix[5] = yAxis.y;
	viewMatrix[9] = yAxis.z;
	viewMatrix[13] = -CL_Vec3f::dot(yAxis, eye);

	viewMatrix[2] = zAxis.x;
	viewMatrix[6] = zAxis.y;
	viewMatrix[10] = zAxis.z;
	viewMatrix[14] = -CL_Vec3f::dot(zAxis, eye);

	viewMatrix[3] = 0.0f;
	viewMatrix[7] = 0.0f;
	viewMatrix[11] = 0.0f;
	viewMatrix[15] = 1.0f;

	return viewMatrix;
}

const CL_Mat4f &Camera::perspective(float fovY, float aspect, float zNear, float zFar)
{
	projMatrix = CL_Mat4f::perspective(fovY, aspect, zNear, zFar);
	return projMatrix;
}

int Camera::unProject(const CL_Vec3f &win, 
			  const CL_Mat4f &modelview, 
			  const CL_Mat4f &proj, 
			  const int viewport[4], 
			  CL_Vec3f &obj)
{
	CL_Mat4f matrix = proj * modelview;  
	matrix = matrix.inverse();

	CL_Vec4f v( 2*(win.x - (float)viewport[0])/(float)viewport[2] - 1,
						 2*(win.y - (float)(viewport[1]))/(float)viewport[3] - 1,
						 2*win.z - 1,
						 1.0f); 

     CL_Vec4f result = matrix * v; 

	 obj = CL_Vec3f(result.x/result.w, result.y/result.w, result.z/result.w);
	 return 0;
}

int Camera::unProject(const CL_Vec3i &abs, CL_Vec3f &obj)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	float z;
	glReadPixels(abs.x, viewport[3] - abs.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);

	return unProject(CL_Vec3f((float)abs.x, (float)(viewport[3] - abs.y), z), viewMatrix, projMatrix, viewport, obj);
}

int Camera::colorPick(const CL_Vec3i &abs, CL_Vec4f &color)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	unsigned char pixel[4];
	coreMgr->getSceneMgr()->getScenePostProcess()->bind();
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glReadPixels(abs.x, viewport[3] - abs.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	coreMgr->getSceneMgr()->getScenePostProcess()->unbind();

	for(unsigned int i = 0; i < 4; i++)
		color[i] = pixel[i]/255.0f;

	return GL_TRUE;
}
