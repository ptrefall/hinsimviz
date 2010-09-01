#include "Plane.h"

#include <GL/glew.h>
#ifdef WIN32
	#include <GL/wglew.h>
#endif

#include <Engine/Core/CoreManager.h>
#include <Engine/Scene/SceneManager.h>
#include <Engine/Scene/Camera.h>
#include <Engine/Shader/ShaderAttrib.h>
#include <Engine/Resource/ResManager.h>

using namespace Engine;
using namespace Nodes;

#ifndef BUFFER_OFFSET
	#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + bytes)
#endif

Plane::Plane(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Engine::Core::CoreManager *coreMgr)
: Engine::Scene::Node(id, colorId, type, name, coreMgr)
{
	renderable = true;
	opaque = true;
	drawMode = GL_TRIANGLE_STRIP;

	size_property = this->AddProperty<CL_Vec3f>("Size", CL_Vec3f(1.0f, 1.0f, 1.0f));
}

Plane::~Plane()
{
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

int Plane::init()
{
	if(initialized)
		return 1;

	glEnable(GL_TEXTURE_2D);

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

	initialized = true;
	return 0;
}

void Plane::localDisplay()
{
	if(!initialized)
		return;

	glEnable(GL_TEXTURE_2D);

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
	
	} disableShader();
	return;
}

void Plane::initIndices()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);
}

void Plane::initVertices()
{
	float width = size_property.Get().x*0.5f;
	float height = size_property.Get().y*0.5f;
	float depth = size_property.Get().z*0.5f;

	vertices.push_back(-width); vertices.push_back(-height); vertices.push_back(-depth);		// 0 - right/top/back
	vertices.push_back(-width); vertices.push_back(height); vertices.push_back(depth);	// 1 - left/top/back
	vertices.push_back(width); vertices.push_back(-height); vertices.push_back(-depth);		// 2 - right/top/front
	vertices.push_back(width); vertices.push_back(height); vertices.push_back(depth);		// 3 - left/top/front
}

void Plane::initTexCoords()
{
	texCoords.push_back(0.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);
}
