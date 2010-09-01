#pragma once

#include "Node.h"
#include <Depends/Entity/Property.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Scene
{
	enum CameraState
	{
		CAM_STATIC = 0,
		CAM_FIRST_PERSON = 1,
		CAM_CHASE = 2,
		CAM_COUNT
	};

	class Camera : public Node
	{
	public:
		virtual ~Camera();

		static const char *getSType() { return "Camera"; }
		static Node *create(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Engine::Core::CoreManager *coreMgr) { return new Camera(id, colorId, type, name, coreMgr); }
		virtual CL_String getNodeType() { return getSType(); }

		virtual int init();
		virtual int update(double dt);

		const CL_Mat4f &lookAt(const CL_Vec3f &eye, const CL_Vec3f &target, const CL_Vec3f &up);
		const CL_Mat4f &perspective(float fovx, float aspect, float znear, float zfar);
		int unProject(const CL_Vec3f &win, const CL_Mat4f &modelview, const CL_Mat4f &proj, const int viewport[4], CL_Vec3f &obj);
		int unProject(const CL_Vec3i &abs, CL_Vec3f &obj);

		int colorPick(const CL_Vec3i &abs, CL_Vec4f &color);

		const CL_Mat4f &getViewMatrix() const { return viewMatrix; }
		const CL_Mat4f &getProjectionMatrix() const { return projMatrix; }

	protected:
		Camera(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Engine::Core::CoreManager *coreMgr);

		CL_Mat4f viewMatrix;
		CL_Mat4f projMatrix;

		Entity::Property<int> cameraState_property;
		CL_Slot slotCameraStateChanged;
		void OnCameraStateChanged(const int &oldValue, const int &newValue);
	};
}}
