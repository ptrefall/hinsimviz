#include "ShaderAttribContainer.h"
#include "ShaderAttrib.h"
#include <GL/glew.h>

using namespace Engine;
using namespace Shader;

ShaderAttribContainer::~ShaderAttribContainer()
{
	for(unsigned int i = 0; i < attribs.size(); i++)
	{
		ShaderAttrib *attrib = attribs[i];
		delete attrib;
		attrib = NULL;
	}
	attribs.clear();
}

void ShaderAttribContainer::addShaderAttrib(ShaderAttrib *attrib)
{
	attribs.push_back(attrib);
}

bool ShaderAttribContainer::initShaderAttribs(unsigned int prog)
{
	for(unsigned int i = 0; i < attribs.size(); i++)
	{
		const char *attribName = attribs[i]->getName().c_str();
		int id = glGetAttribLocation(prog, attribName);
		if(id >= 0)
		{
			glVertexAttribPointer(id, 
				attribs[i]->getSize(), 
				attribs[i]->getType(), 
				attribs[i]->getNormalized(), 
				attribs[i]->getStride(), 
				attribs[i]->getPointer());
			glEnableVertexAttribArray(id);
		}
		else
		{
			continue;
		}
	}
	return true;
}

bool ShaderAttribContainer::bindAttribs(unsigned int prog)
{
	for(unsigned int i = 0; i < attribs.size(); i++)
	{
		glBindAttribLocation(prog, i, attribs[i]->getName().c_str());
	}
	return true;
}

ShaderAttrib *ShaderAttribContainer::getShaderAttrib(unsigned int i)
{
	if(i < attribs.size())
		return attribs[i];
	else
		return 0;
}
