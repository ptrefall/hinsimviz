#pragma once

#include <ClanLib/core.h>

namespace Engine
{
namespace Scene { class Object; }
namespace Events
{

enum TypeId
{
	TYPE_BOOL = 0,
	TYPE_INT,
	TYPE_UINT,
	TYPE_FLOAT,
	TYPE_DOUBLE,
	TYPE_STRING,
	TYPE_VEC2,
	TYPE_VEC3,
	TYPE_VEC4,
	TYPE_MAT3,
	TYPE_MAT4,
	TYPE_OBJECT,
	TYPE_VOID
};

class Type
{
public:
	static int getType(bool arg) { return TYPE_BOOL; }
	static int getType(unsigned int arg) { return TYPE_UINT; }
	static int getType(int arg) { return TYPE_INT; }
	static int getType(float arg) { return TYPE_FLOAT; }
	static int getType(double arg) { return TYPE_DOUBLE; }
	static int getType(const CL_String &arg) { return TYPE_STRING; }
	static int getType(const CL_Vec2f &arg) { return TYPE_VEC2; }
	static int getType(const CL_Vec3f &arg) { return TYPE_VEC3; }
	static int getType(const CL_Vec4f &arg) { return TYPE_VEC4; }
	static int getType(const CL_Mat3f &arg) { return TYPE_MAT3; }
	static int getType(const CL_Mat4f &arg) { return TYPE_MAT4; }
	static int getType(Scene::Object *arg);
	static int getType(void *arg) { return TYPE_VOID; }
};

}}
