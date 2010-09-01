#pragma once

#include <Depends/Entity/Property.h>
#include <GL/glew.h>

namespace Engine
{
namespace Shader
{
	class ShaderAttrib
	{
	public:
		ShaderAttrib(const CL_String &name, int size, GLenum type, bool normalized, int stride, void* pointer)
		: name(name), size(size), type(type), normalized(normalized), stride(stride), pointer(pointer)
		{}

		virtual const CL_String &getName() const { return name; }
		virtual int getSize() const { return size; }
		virtual GLenum getType() const { return type; }
		virtual bool getNormalized() const { return normalized; }
		virtual int getStride() const { return stride; }
		virtual void *getPointer() const { return pointer; }

	protected:
		CL_String name;
		int size;
		GLenum type;
		bool normalized;
		int stride;
		void* pointer;
	};
}}