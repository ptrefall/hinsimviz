#include "ShaderUniformContainer.h"
#include "ShaderUniform.h"

using namespace Engine;
using namespace Shader;

ShaderUniformContainer::~ShaderUniformContainer()
{
	for(unsigned int i = 0; i < uniforms.size(); i++)
	{
		ShaderUniform *uniform = uniforms[i];
		delete uniform;
		uniform = NULL;
	}
	uniforms.clear();
}

void ShaderUniformContainer::addShaderUniform(ShaderUniform *uni)
{
	uniforms.push_back(uni);
}

void ShaderUniformContainer::updateShaderUniform(const CL_String &name, void *pointer)
{
	for(unsigned int i = 0; i < uniforms.size(); i++)
	{
		if(uniforms[i]->getName() == name)
		{
			uniforms[i]->setPointer(pointer);
			break;
		}
	}
}

bool ShaderUniformContainer::initShaderUniforms(unsigned int prog)
{
	if(initialized)
		return false;

	for(unsigned int i = 0; i < uniforms.size(); i++)
	{
		uniforms[i]->init(prog);
	}
	initialized = true;
	return true;
}

bool ShaderUniformContainer::bindUniforms(unsigned int prog)
{
	for(unsigned int i = 0; i < uniforms.size(); i++)
	{
		uniforms[i]->bind();
	}
	return true;
}

ShaderUniform *ShaderUniformContainer::getShaderUniform(unsigned int i)
{
	if(i < uniforms.size())
		return uniforms[i];
	else
		return 0;
}
