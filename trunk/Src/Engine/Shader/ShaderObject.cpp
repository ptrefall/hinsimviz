#include "ShaderObject.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Resource/ResManager.h>
#include <GL/glew.h>

#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#pragma warning(push)
#pragma warning(disable:4049)

using namespace Engine;
using namespace Shader;

ShaderObject::ShaderObject(Engine::Core::CoreManager *coreMgr)
: isSet(false), initialized(false)
{
	this->coreMgr = coreMgr;
}

bool ShaderObject::setShader(const CL_String &fileName)
{
	if(isSet)
		return false;

	int vSize, fSize;

	// Allocate memory to hold the source of our shaders.
	vSize = shaderSize(fileName, V_SHADER);
	fSize = shaderSize(fileName, F_SHADER);

	if((vSize == -1) || (fSize == -1))
	{
		CL_String msg = cl_format("Cannot determine size of the shader %1", fileName);
		coreMgr->getLogMgr()->log("ShaderObject::setShader", msg, Log::L_ERROR);
		return false;
	}

	vertexShaderSource = (GLchar *) malloc(vSize);
	fragmentShaderSource = (GLchar *) malloc(fSize);

	// Read the source code
	if(!readShader(fileName, V_SHADER, vertexShaderSource, vSize))
	{
		CL_String msg = cl_format("Cannot read the file %1.vs", fileName);
		coreMgr->getLogMgr()->log("ShaderObject::setShader", msg, Log::L_ERROR);
		return false;
	}

	if(!readShader(fileName, F_SHADER, fragmentShaderSource, fSize))
	{
		CL_String msg = cl_format("Cannot read the file %1.fs", fileName);
		coreMgr->getLogMgr()->log("ShaderObject::setShader", msg, Log::L_ERROR);
		return false;
	}

	isSet = true;
	return true;
}

bool ShaderObject::initShader()
{
	if(initialized)
		return false;

	if(!isShaderSet())
	{
		setShader(cl_format("%1/%2", coreMgr->getResMgr()->getRootPath(), "Shader/minimal"));
	}

	GLint vertCompiled, fragCompiled;    // status values
	GLint linked;
	int infologLength = 0;
	int charsWritten  = 0;
	GLchar *infoLog;

	// Create a vertex shader object and a fragment shader object
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);

	const char *vertS = vertexShaderSource;
	const char *fragS = fragmentShaderSource;
	// Load source code strings into shaders
	glShaderSource(vs, 1, &vertS, NULL);
	glShaderSource(fs, 1, &fragS, NULL);

	// Compile the vertex shader, and print out
	// the compiler log file.
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &vertCompiled);
	glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 0)
	{
		infoLog = (GLchar *)malloc(infologLength);
		if (infoLog == NULL)
		{
			CL_String msg = cl_format("Could not allocate InfoLog buffer");
			coreMgr->getLogMgr()->log("ShaderObject::initShader", msg, Log::L_ERROR);
			return false;
		}
		glGetShaderInfoLog(vs, infologLength, &charsWritten, infoLog);
		if(charsWritten > 0)
		{
			CL_String msg = cl_format("VertexShader InfoLog: %1", infoLog);
			coreMgr->getLogMgr()->log("ShaderObject::initShader", msg, Log::L_WARN);
		}
		free(infoLog);
	}

	// Compile the fragment shader, and print out
	// the compiler log file.

	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &fragCompiled);
	glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 0)
	{
		infoLog = (GLchar *)malloc(infologLength);
		if (infoLog == NULL)
		{
			CL_String msg = cl_format("Could not allocate InfoLog buffer");
			coreMgr->getLogMgr()->log("ShaderObject::initShader", msg, Log::L_ERROR);
			return false;
		}
		glGetShaderInfoLog(fs, infologLength, &charsWritten, infoLog);
		if(charsWritten > 0)
		{
			CL_String msg = cl_format("FragmentShader InfoLog: %1", infoLog);
			coreMgr->getLogMgr()->log("ShaderObject::initShader", msg, Log::L_WARN);
		}
		free(infoLog);
	}

	if (!vertCompiled || !fragCompiled)
		return false;

	// Create a program object and attach the two compiled shaders
	prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);

	// Link the program object and print out the info log
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 0)
	{
		infoLog = (GLchar *)malloc(infologLength);
		if (infoLog == NULL)
		{
			CL_String msg = cl_format("Could not allocate InfoLog buffer");
			coreMgr->getLogMgr()->log("ShaderObject::initShader", msg, Log::L_ERROR);
			return false;
		}
		glGetProgramInfoLog(prog, infologLength, &charsWritten, infoLog);
		if(charsWritten > 0)
		{
			CL_String msg = cl_format("Program InfoLog: %1", infoLog);
			coreMgr->getLogMgr()->log("ShaderObject::initShader", msg, Log::L_WARN);
		}
		free(infoLog);
	}
	if (!linked)
		return false;

	initialized = true;
	return true;
}

void ShaderObject::enableShader()
{
	glUseProgram(prog);
}

void ShaderObject::disableShader()
{
	glUseProgram(0);
}

int ShaderObject::shaderSize(const CL_String &fileName, ShaderType shaderType)
{
  // Returns the size in bytes of the shader fileName.
  // If an error occurred, it returns -1.
  //
  // File name convention:
  //
  // <fileName>.vert
  // <fileName>.frag
  //
  int fd;
  char name[100];
  int count = -1;

  strcpy(name, fileName.c_str());

  switch(shaderType)
  {
    case V_SHADER:
		strcat(name, ".vs");
		break;
    case F_SHADER:
		strcat(name, ".fs");
		break;
    default:
		CL_String msg = cl_format("Unknown shader file type");
		coreMgr->getLogMgr()->log("ShaderObject::shaderSize", msg, Log::L_ERROR);
		return -1;
  }

  // Open the file, seek to the end to find its length (WIN32)
  fd = _open(name, _O_RDONLY);
  if (fd != -1)
  {
    count = _lseek(fd, 0, SEEK_END) + 1;
    _close(fd);
  }
  return count;
}

int ShaderObject::readShader(const CL_String &fileName, ShaderType shaderType, char *shaderText, int size)
{
  // Reads a shader from the supplied file and returns the shader in the
  // arrays passed in. Returns 1 if successful, 0 if an error occurred.
  // The parameter size is an upper limit of the amount of bytes to read.
  // It is ok for it to be too big.
  FILE *fh;
  char name[100];
  int count;

  strcpy(name, fileName.c_str());

  switch (shaderType) 
  {
    case V_SHADER:
      strcat(name, ".vs");
      break;
    case F_SHADER:
      strcat(name, ".fs");
      break;
    default:
      CL_String msg = cl_format("Unknown shader file type");
	  coreMgr->getLogMgr()->log("ShaderObject::readShader", msg, Log::L_ERROR);
      exit(1);
      break;
  }

  // Open the file
  fh = fopen(name, "r");
  if (!fh)
    return -1;

  // Get the shader from a file.
  fseek(fh, 0, SEEK_SET);
  count = (int) fread(shaderText, 1, size, fh);
  shaderText[count] = '\0';

  if (ferror(fh))
      count = 0;

  fclose(fh);
  return count;
}
