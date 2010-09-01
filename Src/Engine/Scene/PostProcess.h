#pragma once

#include <ClanLib/core.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Scene
{
	class Node;
	class PostProcess
	{
	public:
		PostProcess(const CL_String &shaderName, const std::vector<unsigned int> &maps, Core::CoreManager *coreMgr);
		virtual ~PostProcess();

		virtual void bind();
		virtual void unbind();

		virtual Node *getNode() const;
		virtual unsigned int getMap(unsigned int i);
		virtual unsigned int getAttachment(unsigned int i);
		virtual unsigned int getFBO() const;
		
	protected:
		void init();

		Core::CoreManager *coreMgr;
		Node *node;
		std::vector<unsigned int> maps;
		std::vector<unsigned int> attachments;
		unsigned int fbo;
		CL_String shaderName;

	};
}}
