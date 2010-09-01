#pragma once

#include <Depends/Entity/Property.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Shader
{
	enum ShaderType 
	{
		V_SHADER, 
		F_SHADER
	};

	class ShaderObject
	{
	public:
		ShaderObject(Engine::Core::CoreManager *coreMgr);
		virtual ~ShaderObject(){}

		virtual bool isShaderSet() const { return isSet; }
		virtual bool isShaderInitialized() const { return initialized; }

		virtual bool setShader(const CL_String &filename);
		virtual bool initShader();

		virtual void enableShader();
		virtual void disableShader();
		
		virtual unsigned int getShaderProg() const { return prog; }

	protected:
		int shaderSize(const CL_String &fileName, ShaderType shaderType);
		int readShader(const CL_String &fileName, ShaderType shaderType, char *shaderText, int size);
		
		Engine::Core::CoreManager *coreMgr;
		bool isSet, initialized;
		unsigned int vs, fs, prog;
		char *vertexShaderSource, *fragmentShaderSource;
	};

}}