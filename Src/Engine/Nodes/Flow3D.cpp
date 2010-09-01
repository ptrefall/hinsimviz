#include "Flow3D.h"

#include <GL/glew.h>
#ifdef WIN32
	#include <GL/wglew.h>
#endif

#include <Engine/Core/CoreManager.h>
#include <Engine/Scene/SceneManager.h>
#include <Engine/Scene/Camera.h>
#include <Engine/Shader/ShaderAttrib.h>
#include <Engine/Resource/ResManager.h>
#include <Engine/Resource/IResource.h>
#include <Engine/Log/LogManager.h>

using namespace Engine;
using namespace Nodes;

#ifndef BUFFER_OFFSET
	#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + bytes)
#endif

Flow3D::Flow3D(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Engine::Core::CoreManager *coreMgr)
: Engine::Scene::Node(id, colorId, type, name, coreMgr), bin(NULL)
{
	renderable = true;
	opaque = true;
	drawMode = GL_TRIANGLE_STRIP;

	file_property = this->AddProperty<CL_String>("File", CL_String());
	slotFileChanged = file_property.ValueChanged().connect(this, &Flow3D::OnFileChanged);
}

Flow3D::~Flow3D()
{
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

int Flow3D::init()
{
	if(initialized)
		return 1;

	/*glEnable(GL_TEXTURE_2D);

	initIndices();
	initVertices();
	initTexCoords();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size()+sizeof(float)*texCoords.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*vertices.size(), &vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), sizeof(float)*texCoords.size(), &texCoords[0]);

	this->addShaderAttrib(new Engine::Shader::ShaderAttrib("in_Position", 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)));
	this->addShaderAttrib(new Engine::Shader::ShaderAttrib("in_TexCoord", 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float)*vertices.size())));
	
	initShader();		
	initShaderAttribs(getShaderProg());

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	initialized = true;*/
	return 0;
}

void Flow3D::localDisplay()
{
	if(!initialized)
		return;

	/*glEnable(GL_TEXTURE_2D);

	CL_Mat4f modelMatrix = getModelMatrix();
	const CL_Mat4f &projMatrix = coreMgr->getSceneMgr()->getCamera()->getProjectionMatrix();
	const CL_Mat4f &viewMatrix = coreMgr->getSceneMgr()->getCamera()->getViewMatrix();

	enableShader();
	{
		initShaderUniforms(getShaderProg());
		updateShaderUniform("Transform1.projectionMatrix", (void*)&projMatrix[0]);
		updateShaderUniform("Transform1.viewMatrix", (void*)&viewMatrix[0]);
		updateShaderUniform("Transform1.modelMatrix", (void*)&modelMatrix[0]);

		bindUniforms(getShaderProg());
		bindTextures();

		glBindVertexArray(vao);
		glDrawElements(drawMode, indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		glBindVertexArray(0);

		unbindTextures();
	
	} disableShader();*/
	return;
}

void Flow3D::initIndices()
{
}

void Flow3D::initVertices()
{
}

void Flow3D::initTexCoords()
{
}

void Flow3D::OnFileChanged(const CL_String &oldValue, const CL_String &newValue)
{
	bin = coreMgr->getResMgr()->create(newValue.c_str(), "BIN");
}
