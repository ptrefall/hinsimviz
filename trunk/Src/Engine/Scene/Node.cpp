#include "Node.h"
#include "SceneManager.h"
#include "Object.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Shader/ShaderUniform.h>
#include <Engine/Events/EngineEvent.h>
#include <Engine/Events/IEngineEventManager.h>
#include <Engine/Resource/ResManager.h>
#include <Engine/Resource/IResource.h>

using namespace Engine;
using namespace Scene;

Node::Node(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Core::CoreManager *coreMgr)
: Engine::Entity::PropertyContainer(coreMgr), Engine::Shader::ShaderObject(coreMgr), parent(NULL), 
  renderable(false), opaque(true), initialized(false), isAnObject(false), 
  vao(0), ibo(0), vbo(0)
{
	this->id = id;
	this->colorId = colorId;
	this->type = type;
	if(name == CL_String())
		this->name = type;
	else
		this->name = name;
	this->coreMgr = coreMgr;

	shaderName = CL_String();

	scale_property = this->AddProperty<float>("Scale", 1.0f);
	position_property = this->AddProperty<CL_Vec3f>("Position", CL_Vec3f(0.0f));
	rotation_property = this->AddProperty<CL_Mat4f>("Rotation", CL_Mat4f::identity());
	localTranslate_property = this->AddProperty<CL_Mat4f>("LocalTranslate", CL_Mat4f::identity());
	globalTranslate_property = this->AddProperty<CL_Mat4f>("GlobalTranslate", CL_Mat4f::identity());

	slotPositionChanged = position_property.ValueChanged().connect(this, &Node::OnPositionChanged);
	slotLocalTranslateChanged = localTranslate_property.ValueChanged().connect(this, &Node::OnLocalTranslateChanged);
	slotGlobalTranslateChanged = globalTranslate_property.ValueChanged().connect(this, &Node::OnGlobalTranslateChanged);

	this->addShaderUniform(new Shader::ShaderUniform("Transform1.projectionMatrix", Shader::UNI_MAT4, (void*)&CL_Mat4f::identity()[0]));
	this->addShaderUniform(new Shader::ShaderUniform("Transform1.viewMatrix", Shader::UNI_MAT4, (void*)&CL_Mat4f::identity()[0]));
	this->addShaderUniform(new Shader::ShaderUniform("Transform1.modelMatrix", Shader::UNI_MAT4, (void*)&CL_Mat4f::identity()[0]));
	this->addShaderUniform(new Shader::ShaderUniform("uni_colorId", Shader::UNI_FLOAT4, (void*)&colorId.r));
}

Node::~Node()
{
	if(isAnObject)
	{
		CL_String msg = cl_format("Object %1 is being destroied!", name);
		coreMgr->getLogMgr()->log("Node::Destruct", msg, Log::L_INFO);
		coreMgr->getEngineEventMgr()->SendEvent(Engine::Events::EngineEvent("ObjectRemoved", Engine::Events::EngineEventValue(getAsObject())));
	}
	else
	{
		CL_String msg = cl_format("Node %1 is being destroied!", name);
		coreMgr->getLogMgr()->log("Node::Destruct", msg, Log::L_INFO);
		coreMgr->getEngineEventMgr()->SendEvent(Engine::Events::EngineEvent("NodeRemoved", Engine::Events::EngineEventValue(this)));
	}

	if(parent)
	{
		parent->remove(this, false);
	}

	for(unsigned int i = 0; i < nonrenderable_children.size(); i++)
	{
		Node *node = nonrenderable_children[i];
		delete node;
		node = NULL;
	}
	nonrenderable_children.clear();

	for(unsigned int i = 0; i < opaque_children.size(); i++)
	{
		Node *node = opaque_children[i];
		delete node;
		node = NULL;
	}
	opaque_children.clear();

	for(unsigned int i = 0; i < transparent_children.size(); i++)
	{
		Node *node = transparent_children[i];
		delete node;
		node = NULL;
	}
	transparent_children.clear();

	for(unsigned int i = 0; i < texIds.size(); i++)
	{
		TexStorageStruct *tex = texIds[i];
		delete tex;
		tex = NULL;
	}
	texIds.clear();
}

int Node::init()
{
	return 0;
}

int Node::kill()
{
	if(isAnObject)
		return 1;

	coreMgr->getSceneMgr()->registerForDelete(this);
	return 0;
}

Object *Node::getAsObject()
{
	if(!isAnObject)
		return NULL;

	return static_cast<Object*>(this);
}

int Node::update(double dt)
{
	localUpdate(dt);

	for(unsigned int i = 0; i < nonrenderable_children.size(); i++)
	{
		nonrenderable_children[i]->update(dt);
	}
	for(unsigned int i = 0; i < opaque_children.size(); i++)
	{
		opaque_children[i]->update(dt);
	}
	for(unsigned int i = 0; i < transparent_children.size(); i++)
	{
		transparent_children[i]->update(dt);
	}

	return 0;
}

int Node::interpolate(double dt)
{
	baseInterpolate(dt);
	localInterpolate(dt);

	for(unsigned int i = 0; i < nonrenderable_children.size(); i++)
	{
		nonrenderable_children[i]->interpolate(dt);
	}
	for(unsigned int i = 0; i < opaque_children.size(); i++)
	{
		opaque_children[i]->interpolate(dt);
	}
	for(unsigned int i = 0; i < transparent_children.size(); i++)
	{
		transparent_children[i]->interpolate(dt);
	}

	return 0;
}

void Node::baseInterpolate(double dt)
{

}

int Node::display()
{
	localDisplay();

	for(unsigned int i = 0; i < nonrenderable_children.size(); i++)
	{
		nonrenderable_children[i]->display();
	}
	for(unsigned int i = 0; i < opaque_children.size(); i++)
	{
		opaque_children[i]->display();
	}
	for(unsigned int i = 0; i < transparent_children.size(); i++)
	{
		transparent_children[i]->display();
	}

	return 0;
}

int Node::add(Node *node)
{
	if(node == NULL)
		return 1;

	node->setParent(this);
	if(node->isNotRenderable())
		nonrenderable_children.push_back(node);
	else if(node->isOpaque())
		opaque_children.push_back(node);
	else
		transparent_children.push_back(node);

	return 0;
}

int Node::remove(Node *node, bool propagate)
{
	if(node == NULL)
		return 1;

	if(node->isNotRenderable())
	{
		for(std::vector<Node *>::iterator it = nonrenderable_children.begin(); it != nonrenderable_children.end(); ++it)
		{
			if(*it == node)
			{
				nonrenderable_children.erase(it);
				return 0;
			}
			if(propagate)
			{
				if((*it)->remove(node, propagate))
				{
					return 0;
				}
			}
		}
	}
	else if(node->isOpaque())
	{
		for(std::vector<Node *>::iterator it = opaque_children.begin(); it != opaque_children.end(); ++it)
		{
			if(*it == node)
			{
				opaque_children.erase(it);
				return 0;
			}
			if(propagate)
			{
				if((*it)->remove(node, propagate))
				{
					return 0;
				}
			}
		}
	}
	else
	{
		for(std::vector<Node *>::iterator it = transparent_children.begin(); it != transparent_children.end(); ++it)
		{
			if(*it == node)
			{
				transparent_children.erase(it);
				return 0;
			}
			if(propagate)
			{
				if((*it)->remove(node, propagate))
				{
					return 0;
				}
			}
		}
	}
	return 1; //failed to find node among children
}

void Node::setParent(Node *parent) 
{ 
	this->parent = parent;
	parentsGlobalTranslate_property = parent->AddProperty<CL_Mat4f>("GlobalTranslate", CL_Mat4f::identity());
	parentsRotate_property = parent->AddProperty<CL_Mat4f>("Rotate", CL_Mat4f::identity());
	parentsScale_property = parent->AddProperty<float>("Scale", 1.0f);
	
	slotParentsGlobalTranslateChanged = parentsGlobalTranslate_property.ValueChanged().connect(this, &Node::OnParentsGlobalTranslateChanged);
	slotParentsRotateChanged = parentsRotate_property.ValueChanged().connect(this, &Node::OnParentsRotateChanged);
	slotParentsScaleChanged = parentsScale_property.ValueChanged().connect(this, &Node::OnParentsScaleChanged);
	
	globalTranslate_property = localTranslate_property.Get() * parentsGlobalTranslate_property.Get();
	position_property = CL_Vec3f(localTranslate_property.Get()[12], localTranslate_property.Get()[13], localTranslate_property.Get()[14]);
}

const CL_Vec3f &Node::getGlobalPos()
{
	globalPos = CL_Vec3f(globalTranslate_property.Get()[12], globalTranslate_property.Get()[13], globalTranslate_property.Get()[14]);
	return globalPos;
}

void Node::translate(const CL_Vec3f &pos)
{
	position_property += pos;
}

void Node::translate(float x, float y, float z)
{
	position_property += CL_Vec3f(x,y,z);
}


void Node::setLocalPos(const CL_Vec3f &pos)
{
	if(position_property == pos)
		return;

	position_property = pos;
}

void Node::rotate(float value, const CL_Vec3f &axis)
{
	/*CL_Vec3f rot = rotation_property.Get();
	rot += axis*value;
	rotation_property = rot;*/
	rotation_property = rotation_property.Get() * CL_Mat4f::rotate(CL_Angle(value, CL_AngleUnit()), axis.x, axis.y, axis.z);
}

void Node::scale(float scale_val)
{
	scale_property = scale_val;
}

void Node::OnParentsGlobalTranslateChanged(const CL_Mat4f &oldValue, const CL_Mat4f &newValue)
{
	//Update the translation matrix here
	globalTranslate_property = localTranslate_property.Get() * parent->getModelMatrix();
	position_property = CL_Vec3f(localTranslate_property.Get()[12], localTranslate_property.Get()[13], localTranslate_property.Get()[14]);
}

void Node::OnParentsRotateChanged(const CL_Mat4f &oldValue, const CL_Mat4f &newValue)
{
	//Update the translation matrix here
	globalTranslate_property = localTranslate_property.Get() * parent->getModelMatrix();
	position_property = CL_Vec3f(localTranslate_property.Get()[12], localTranslate_property.Get()[13], localTranslate_property.Get()[14]);
}

void Node::OnParentsScaleChanged(const float &oldValue, const float &newValue)
{
	//Update the translation matrix here
	globalTranslate_property = localTranslate_property.Get() * parent->getModelMatrix();
	position_property = CL_Vec3f(localTranslate_property.Get()[12], localTranslate_property.Get()[13], localTranslate_property.Get()[14]);
}

void Node::OnGlobalTranslateChanged(const CL_Mat4f &oldValue, const CL_Mat4f &newValue)
{
	//modelChanged = true;
}

void Node::OnLocalTranslateChanged(const CL_Mat4f &oldValue, const CL_Mat4f &newValue)
{
	//Update the transformation matrix here
	if(parent)
		globalTranslate_property = newValue * parent->getModelMatrix();
	else
		globalTranslate_property = newValue;
	position_property = CL_Vec3f(newValue[12], newValue[13], newValue[14]);
}

void Node::OnPositionChanged(const CL_Vec3f &oldValue, const CL_Vec3f &newValue)
{
	if(CL_Vec3f(localTranslate_property.Get()[12], localTranslate_property.Get()[13], localTranslate_property.Get()[14]) == newValue)
		return;

	CL_Vec3f pos = newValue - oldValue;
	
	//Update the transformation matrix here
	localTranslate_property = localTranslate_property.Get() * CL_Mat4f::translate(pos.x, pos.y, pos.z);
}

CL_Mat4f Node::getModelMatrix()
{
	CL_Mat4f modelMatrix = CL_Mat4f::identity();
	modelMatrix[0] *= getScale();
	modelMatrix[5] *= getScale();
	modelMatrix[10] *= getScale();
	modelMatrix = modelMatrix * getRotation();
	modelMatrix = modelMatrix * getLocalTranslate();

	if(parent)
		modelMatrix = modelMatrix * parent->getModelMatrix();
	return modelMatrix;
}

Node *Node::findNode(const CL_Vec3f &pos)
{
	//For now this only checks exact positions. Will need a collision manager that supports 
	//AABB, Circular, Rectangular and Spherical bounds to effectively and robustly check whether
	//a node occupies a position. Could also improve this check using a quadtree/octree
	if(this->getGlobalPos() == pos)
	{
		return this;
	}

	for(unsigned int i = 0; i < nonrenderable_children.size(); i++)
	{
		Node *node = nonrenderable_children[i]->findNode(pos);
		if(node)
			return node;
	}
	for(unsigned int i = 0; i < opaque_children.size(); i++)
	{
		Node *node = opaque_children[i]->findNode(pos);
		if(node)
			return node;
	}
	for(unsigned int i = 0; i < transparent_children.size(); i++)
	{
		Node *node = transparent_children[i]->findNode(pos);
		if(node)
			return node;
	}

	return NULL;
}

Node *Node::findNode(const CL_Vec4f &color)
{
	if(this->getColorId() == color)
	{
		return this;
	}

	for(unsigned int i = 0; i < nonrenderable_children.size(); i++)
	{
		Node *node = nonrenderable_children[i]->findNode(color);
		if(node)
			return node;
	}
	for(unsigned int i = 0; i < opaque_children.size(); i++)
	{
		Node *node = opaque_children[i]->findNode(color);
		if(node)
			return node;
	}
	for(unsigned int i = 0; i < transparent_children.size(); i++)
	{
		Node *node = transparent_children[i]->findNode(color);
		if(node)
			return node;
	}

	return NULL;
}

Node *Node::findNode(const CL_String &type)
{
	if(this->getType() == type)
		return this;

	for(unsigned int i = 0; i < nonrenderable_children.size(); i++)
	{
		Node *node = nonrenderable_children[i]->findNode(type);
		if(node)
			return node;
	}
	for(unsigned int i = 0; i < opaque_children.size(); i++)
	{
		Node *node = opaque_children[i]->findNode(type);
		if(node)
			return node;
	}
	for(unsigned int i = 0; i < transparent_children.size(); i++)
	{
		Node *node = transparent_children[i]->findNode(type);
		if(node)
			return node;
	}
	return NULL;
}

void Node::bindTextures()
{
	for(unsigned int i = 0; i < texIds.size(); i++)
	{
		unsigned int numId = texIds[i]->texNumId;
		unsigned int glTexId = texIds[i]->glId;
		unsigned int texType = texIds[i]->texType;
		
		glActiveTexture(GL_TEXTURE0+numId);
		if(texType == Scene::TEXCOORD_2D)
			glBindTexture(GL_TEXTURE_2D, glTexId);
		else if(texType == Scene::TEXCOORD_CUBE)
		{
			glEnable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, glTexId);
		}
		else
			glBindTexture(GL_TEXTURE_2D, glTexId);
	}
}

void Node::unbindTextures()
{
	for(unsigned int i = 0; i < texIds.size(); i++)
	{
		unsigned int numId = texIds[i]->texNumId;
		unsigned int texType = texIds[i]->texType;
		
		glActiveTexture(GL_TEXTURE0+numId);
		if(texType == Scene::TEXCOORD_2D)
			glBindTexture(GL_TEXTURE_2D, 0);
		else if(texType == Scene::TEXCOORD_CUBE)
		{
			glEnable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glDisable(GL_TEXTURE_CUBE_MAP);
		}
		else
			glBindTexture(GL_TEXTURE_2D, 0);
	}
}


void Node::setTexture(const CL_String &uniformName, const CL_String &filename, unsigned int i, unsigned int texType)
{
	Resource::IResource *tex = coreMgr->getResMgr()->create(filename.c_str(), "TEX");
	TexStorageStruct *storage = new TexStorageStruct(i, tex->getGLId(), 0, texType, tex->getTexBits());

	texIds.push_back(storage);
	this->addShaderUniform(new Shader::ShaderUniform(uniformName, Shader::UNI_SAMPLER, &i));
}

void Node::setTexture(const CL_String &uniformName, unsigned int glTexId, unsigned int i, unsigned int texType)
{
	TexStorageStruct *storage = new TexStorageStruct(i, glTexId, 0, texType, 0);
	texIds.push_back(storage);
	this->addShaderUniform(new Shader::ShaderUniform(uniformName, Shader::UNI_SAMPLER, &i));
}

unsigned int Node::getTexId(unsigned int id, bool glType)
{
	for(unsigned int i = 0; i < texIds.size(); i++)
	{
		if(id == texIds[i]->texNumId)
		{
			if(glType)
			{
				return texIds[i]->glId;
			}
			else
			{
				return texIds[i]->id;
			}
			break;
		}
	}
	return 0;
}
