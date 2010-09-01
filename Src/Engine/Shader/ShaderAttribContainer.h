#pragma once

#include <Depends/Entity/Property.h>
#include <vector>

namespace Engine
{
namespace Shader
{
	class ShaderAttrib;
	class ShaderAttribContainer
	{
	public:
		ShaderAttribContainer() : size(0) {}
		virtual ~ShaderAttribContainer();

		virtual void addShaderAttrib(ShaderAttrib *attrib);

		virtual void setShaderAttribTotalSize(unsigned int size) { this->size = size; }
		virtual unsigned int getShaderAttribTotalSize() const { return size; }

		virtual bool bindAttribs(unsigned int prog);
		virtual bool initShaderAttribs(unsigned int prog);

		virtual unsigned int getShaderAttribCount() const { return attribs.size(); }
		virtual ShaderAttrib *getShaderAttrib(unsigned int i);

	protected:
		std::vector<ShaderAttrib*> attribs;
		unsigned int size;
	};

}}