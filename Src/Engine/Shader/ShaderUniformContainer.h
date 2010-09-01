#pragma once

#include <Depends/Entity/Property.h>
#include <vector>

namespace Engine
{
namespace Shader
{
	class ShaderUniform;
	class ShaderUniformContainer
	{
	public:
		ShaderUniformContainer() : initialized(false) {}
		virtual ~ShaderUniformContainer();

		virtual void addShaderUniform(ShaderUniform *uni);
		virtual void updateShaderUniform(const CL_String &name, void *pointer);

		virtual bool bindUniforms(unsigned int prog);
		virtual bool initShaderUniforms(unsigned int prog);

		virtual unsigned int getShaderUniformCount() const { return uniforms.size(); }
		virtual ShaderUniform *getShaderUniform(unsigned int i);

	protected:
		std::vector<ShaderUniform*> uniforms;
		bool initialized;
	};

}}