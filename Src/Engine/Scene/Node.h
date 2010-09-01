#pragma once

#include "NodeFactory.h"
#include <Engine/Shader/ShaderObject.h>
#include <Engine/Shader/ShaderAttribContainer.h>
#include <Engine/Shader/ShaderUniformContainer.h>
#include <Depends/Entity/PropertyContainer.h>
#include <Depends/Entity/Property.h>

namespace Engine
{
namespace Scene
{
	struct TexStorageStruct
	{
		unsigned int glId;
		unsigned int id;
		unsigned int texNumId;
		unsigned int texType;
		BYTE *bits;
		TexStorageStruct(unsigned int texNumId, unsigned int glId, unsigned int id, unsigned int texType, BYTE *bits) : texNumId(texNumId), glId(glId), id(id), texType(texType), bits(bits) {}
	};

	enum eTexCoordType
	{
		TEXCOORD_2D = 2,
		TEXCOORD_CUBE = 3
	};

	class Object;
	class Node : public Entity::PropertyContainer, public Engine::Shader::ShaderObject, public Engine::Shader::ShaderAttribContainer, public Engine::Shader::ShaderUniformContainer
	{
	protected:
		friend class NodeFactory;
	public:
		virtual ~Node();

		virtual unsigned int getId() const { return id; }
		virtual CL_Vec4f getColorId() const { return colorId; }
		virtual const CL_String &getType() const { return type; }
		virtual const CL_String &getName() const { return name; }

		virtual int init();
		virtual int kill();
		virtual int update(double dt);
		virtual int interpolate(double dt);
		virtual int display();

		virtual int add(Node *node);
		virtual int remove(Node *node, bool propagate);

		virtual void setParent(Node *parent);
		virtual Node *getParent() const { return parent; }

		virtual bool isRenderable() { return renderable; }
		virtual bool isNotRenderable() { return !renderable; }
		virtual bool isOpaque() { return opaque; }
		virtual bool isTransparent() { return !opaque; }
		
		virtual bool isObject() { return isAnObject; }
		virtual void setAsObject() { isAnObject = true; }
		virtual Object *getAsObject();

		virtual float getScale() const { return scale_property.Get(); }
		virtual const CL_Mat4f &getRotation() const { return rotation_property.Get(); }
		virtual const CL_Mat4f &getGlobalTranslate() const { return globalTranslate_property.Get(); }
		virtual const CL_Mat4f &getParentsGlobalTranslate() const { return parentsGlobalTranslate_property.Get(); }
		virtual const CL_Mat4f &getLocalTranslate() const { return localTranslate_property.Get(); }
		virtual const CL_Vec3f &getLocalPos() const { return position_property.Get(); }
		virtual const CL_Vec3f &getGlobalPos();
		
		virtual void scale(float scale_val);
		virtual void rotate(float value, const CL_Vec3f &axis);
		virtual void translate(const CL_Vec3f &pos);
		virtual void translate(float x, float y, float z);
		virtual void setLocalPos(const CL_Vec3f &pos);

		virtual CL_Mat4f getModelMatrix();

		virtual Node *findNode(const CL_Vec3f &pos);
		virtual Node *findNode(const CL_Vec4f &color);
		virtual Node *findNode(const CL_String &type);

		virtual void bindTextures();
		virtual void unbindTextures();

		virtual void setTexture(const CL_String &uniformName, const CL_String &filename, unsigned int i = 0, unsigned int texType = 2);
		virtual void setTexture(const CL_String &uniformName, unsigned int glTexId, unsigned int i = 0, unsigned int texType = 2);
		virtual unsigned int getTexId(unsigned int id, bool glType);

		CL_String getShaderName() const { return shaderName; }
		virtual void setShaderName(const CL_String &shaderName) { this->shaderName = shaderName; }

	protected:
		Node(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Core::CoreManager *coreMgr);
		void baseInterpolate(double dt);

		virtual void localUpdate(double dt) {}
		virtual void localInterpolate(double dt) {}
		virtual void localDisplay() {}

		unsigned int id;
		CL_Vec4f colorId;
		CL_String type;
		CL_String name;

		Core::CoreManager *coreMgr;
		Node *parent;
		
		std::vector<Node*> nonrenderable_children;
		std::vector<Node*> opaque_children;
		std::vector<Node*> transparent_children;

		unsigned int vao;
		unsigned int ibo;
		unsigned int vbo;

		bool renderable;
		bool opaque;
		bool initialized;
		bool isAnObject;

		unsigned int drawMode;

		CL_String shaderName;

		Entity::Property<float> scale_property;
		Entity::Property<CL_Vec3f> position_property;
		Entity::Property<CL_Mat4f> rotation_property;
		Entity::Property<CL_Mat4f> localTranslate_property;
		Entity::Property<CL_Mat4f> globalTranslate_property;
		Entity::Property<CL_Mat4f> parentsGlobalTranslate_property;
		Entity::Property<CL_Mat4f> parentsRotate_property;
		Entity::Property<float> parentsScale_property;

		CL_Slot slotPositionChanged;
		CL_Slot slotGlobalTranslateChanged;
		CL_Slot slotLocalTranslateChanged;
		CL_Slot slotParentsGlobalTranslateChanged;
		CL_Slot slotParentsRotateChanged;
		CL_Slot slotParentsScaleChanged;
		
		void OnPositionChanged(const CL_Vec3f &oldValue, const CL_Vec3f &newValue);
		void OnGlobalTranslateChanged(const CL_Mat4f &oldValue, const CL_Mat4f &newValue);
		void OnLocalTranslateChanged(const CL_Mat4f &oldValue, const CL_Mat4f &newValue);
		void OnParentsGlobalTranslateChanged(const CL_Mat4f &oldValue, const CL_Mat4f &newValue);
		void OnParentsRotateChanged(const CL_Mat4f &oldValue, const CL_Mat4f &newValue);
		void OnParentsScaleChanged(const float &oldValue, const float &newValue);

		CL_Vec3f globalPos;

		std::vector<TexStorageStruct*> texIds;
	};
}}
