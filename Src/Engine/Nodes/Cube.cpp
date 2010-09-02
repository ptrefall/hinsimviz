#include "Cube.h"

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

Cube::Cube(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Engine::Core::CoreManager *coreMgr)
: Engine::Scene::Node(id, colorId, type, name, coreMgr)
{
	renderable = true;
	opaque = true;
	//drawMode = GL_TRIANGLE_STRIP;
	drawMode = GL_TRIANGLES;
}

Cube::~Cube()
{
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

int Cube::init()
{
	if(initialized)
		return 1;

	initIndices();
	initVertices();
	initTexCoords();

	unsigned int iSize = sizeof(unsigned int)*indices.size();
	unsigned int vSize = sizeof(float)*vertices.size();
	unsigned int tSize = sizeof(float)*texCoords.size();

	unsigned int vOffset = 0;
	unsigned int tOffset = vOffset + vSize;

	glEnable(GL_TEXTURE_2D);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, &indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vSize+tSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, vOffset, vSize, &vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, tOffset, tSize, &texCoords[0]);

	this->addShaderAttrib(new Engine::Shader::ShaderAttrib("in_Position", 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vOffset)));
	this->addShaderAttrib(new Engine::Shader::ShaderAttrib("in_TexCoord", 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(tOffset)));

	initShader();		
	initShaderAttribs(getShaderProg());

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	initialized = true;
	return 0;
}

void Cube::localDisplay()
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
		bindAttribs(getShaderProg());
		bindTextures();

		glBindVertexArray(vao);
		glDrawElements(drawMode, indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		glBindVertexArray(0);

		unbindTextures();
	
	} disableShader();
}

/*void Cube::initIndices()
{
	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(6);
	indices.push_back(5);
	indices.push_back(4);
	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(4);
	indices.push_back(2);
	indices.push_back(0);
}*/

void Cube::initIndices()
{
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(2); indices.push_back(3); indices.push_back(0);

	indices.push_back(4); indices.push_back(5); indices.push_back(6);
	indices.push_back(6); indices.push_back(7); indices.push_back(4);

	indices.push_back(8); indices.push_back(9); indices.push_back(10);
	indices.push_back(10); indices.push_back(11); indices.push_back(8);

	indices.push_back(12); indices.push_back(13); indices.push_back(14);
	indices.push_back(14); indices.push_back(15); indices.push_back(12);

	indices.push_back(16); indices.push_back(17); indices.push_back(18);
	indices.push_back(18); indices.push_back(19); indices.push_back(16);

	indices.push_back(20); indices.push_back(21); indices.push_back(22);
	indices.push_back(22); indices.push_back(23); indices.push_back(20);
}

/*void Cube::initVertices()
{
	float size = 4.0f; //TODO: This should be set through a property
	vertices.push_back(size); vertices.push_back(size*2.0f); vertices.push_back(-size);		// 0 - right/top/back
	vertices.push_back(-size); vertices.push_back(size*2.0f); vertices.push_back(-size);	// 1 - left/top/back
	vertices.push_back(size); vertices.push_back(size*2.0f); vertices.push_back(size);		// 2 - right/top/front
	vertices.push_back(-size); vertices.push_back(size*2.0f); vertices.push_back(size);		// 3 - left/top/front
	vertices.push_back(size); vertices.push_back(0.0f); vertices.push_back(-size);			// 4 - right/bottom/back
	vertices.push_back(-size); vertices.push_back(0.0f); vertices.push_back(-size);			// 5 - left/bottom/back
	vertices.push_back(-size); vertices.push_back(0.0f); vertices.push_back(size);			// 6 - left/bottom/front
	vertices.push_back(size); vertices.push_back(0.0f); vertices.push_back(size);			// 7 - right/bottom/front

}*/

void Cube::initVertices()
{
	float size = 4.0f;

	//TOP
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(-size);
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(-size);

	//BOTTOM
	vertices.push_back(-size); vertices.push_back(size); vertices.push_back(-size);
	vertices.push_back(size); vertices.push_back(size); vertices.push_back(-size);
	vertices.push_back(size); vertices.push_back(size); vertices.push_back(size);
	vertices.push_back(-size); vertices.push_back(size); vertices.push_back(size);

	//FRONT
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(-size);
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(-size);
	vertices.push_back(size); vertices.push_back(size); vertices.push_back(-size);
	vertices.push_back(-size); vertices.push_back(size); vertices.push_back(-size);

	//BACK
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(size);

	//LEFT
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(-size);
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(size); vertices.push_back(-size);

	//RIGHT
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(-size);
	vertices.push_back(-size); vertices.push_back(size); vertices.push_back(-size);
	vertices.push_back(-size); vertices.push_back(size); vertices.push_back(size);
}

/*void Cube::initTexCoords()
{
	//This will be wrong no matter how we look at it I think, since we're 
	//running with an optimized cube vertex list, we're nothing thinking 
	//about seperate faces, which is necessary to properly lay out texcoords
	//for each face of the cube. Oh well :)
	texCoords.push_back(0.0f); texCoords.push_back(0.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(0.0f); texCoords.push_back(0.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
}*/

void Cube::initTexCoords()
{
	//TOP
	texCoords.push_back(0.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);

	//BOTTOM
	texCoords.push_back(0.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);

	//FRONT
	texCoords.push_back(0.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);

	//BACK
	texCoords.push_back(0.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);

	//LEFT
	texCoords.push_back(0.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);

	//RIGHT
	texCoords.push_back(0.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);
}
