#pragma once

#include <Depends/Entity/Property.h>
#include <GL/glew.h>

namespace Engine
{
namespace Shader
{
	enum eUniformType
	{
		UNI_SAMPLER,
		UNI_FLOAT,
		UNI_FLOAT2,
		UNI_FLOAT3,
		UNI_FLOAT4,
		UNI_MAT3,
		UNI_MAT4,
		UNI_COUNT
	};

	class ShaderUniform
	{
	public:
		ShaderUniform(const CL_String &name, const unsigned int &type, void* pointer)
			: name(name), type(type), loc(0)
		{
			unsigned int size = 0;
			switch(type)
			{
			case Shader::UNI_SAMPLER:
				{
					size = sizeof(int);
					this->pointer = new int[1];
				} break;
			case Shader::UNI_FLOAT:
				{
					size = sizeof(float);
					this->pointer = new float[1];
				} break;
			case Shader::UNI_FLOAT2:
				{
					size = sizeof(float) * 2;
					this->pointer = new float[2];
				} break;
			case Shader::UNI_FLOAT3:
				{
					size = sizeof(float) * 3;
					this->pointer = new float[3];
				} break;
			case Shader::UNI_FLOAT4:
				{
					size = sizeof(float) * 4;
					this->pointer = new float[4];
				} break;
			case Shader::UNI_MAT3:
				{
					size = sizeof(float) * 9;
					this->pointer = new float[9];
				} break;
			case Shader::UNI_MAT4:
				{
					size = sizeof(float) * 16;
					this->pointer = new float[16];
				} break;
			};
			memcpy(this->pointer, pointer, size);
		}

		virtual void init(const unsigned int &prog)
		{
			loc = glGetUniformLocation(prog, name.c_str());
		}

		virtual void bind()
		{
			switch(type)
			{
			case Shader::UNI_SAMPLER:
				{
					glUniform1i(loc, *(static_cast<int*>(pointer)));
				} break;
			case Shader::UNI_FLOAT:
				{
					glUniform1f(loc, *(static_cast<float*>(pointer)));
				} break;
			case Shader::UNI_FLOAT2:
				{
					glUniform2fv(loc, 1, (static_cast<float*>(pointer)));
				} break;
			case Shader::UNI_FLOAT3:
				{
					glUniform3fv(loc, 1, (static_cast<float*>(pointer)));
				} break;
			case Shader::UNI_FLOAT4:
				{
					glUniform4fv(loc, 1, (static_cast<float*>(pointer)));
				} break;
			case Shader::UNI_MAT3:
				{
					glUniformMatrix3fv(loc, 1, GL_FALSE, (static_cast<float*>(pointer)));
				} break;
			case Shader::UNI_MAT4:
				{
					glUniformMatrix4fv(loc, 1, GL_FALSE, (static_cast<float*>(pointer)));
				} break;
			};
		}

		virtual const CL_String &getName() const { return name; }
		virtual unsigned int getLocation() const { return loc; }
		virtual unsigned int getType() const { return type; }
		virtual void *getPointer() const { return pointer; }

		virtual void setPointer(void *pointer)
		{
			unsigned int size = 0;
			switch(type)
			{
			case Shader::UNI_SAMPLER:
				{
					size = sizeof(int);
				} break;
			case Shader::UNI_FLOAT:
				{
					size = sizeof(float);
				} break;
			case Shader::UNI_FLOAT2:
				{
					size = sizeof(float) * 2;
				} break;
			case Shader::UNI_FLOAT3:
				{
					size = sizeof(float) * 3;
				} break;
			case Shader::UNI_FLOAT4:
				{
					size = sizeof(float) * 4;
				} break;
			case Shader::UNI_MAT3:
				{
					size = sizeof(float) * 9;
				} break;
			case Shader::UNI_MAT4:
				{
					size = sizeof(float) * 16;
				} break;
			};
			memcpy(this->pointer, pointer, size);
		}

	protected:
		CL_String name;
		unsigned int loc;
		unsigned int prog;
		unsigned int type;
		void* pointer;
	};
}}