#include "PostProcess.h"
#include "SceneManager.h"
#include "Node.h"
#include "Camera.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/GUI/IGuiManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Resource/ResManager.h>
#include <GL/glew.h>

using namespace Engine;
using namespace Scene;

PostProcess::PostProcess(const CL_String &shaderName, const std::vector<unsigned int> &maps, Core::CoreManager *coreMgr)
{
	this->shaderName = shaderName;
	this->maps = maps;
	this->attachments = maps;
	this->coreMgr = coreMgr;
	
	this->node = NULL;
	this->fbo = 0;

	init();
}

PostProcess::~PostProcess()
{
}

void PostProcess::init()
{
	node = coreMgr->getSceneMgr()->create("Plane");
	node->setShader(cl_format("%1/Shader/%2", coreMgr->getResMgr()->getRootPath(), shaderName));
	node->GetProperty<CL_Vec3f>("Size") = CL_Vec3f(10.0f, 10.0f, 0.0f);
	
	bool hasColor0 = false, hasColor1 = false, hasDepth = false;
	int iColor0 = -1, iColor1 = -1, iDepth = -1;
	glGenTextures(maps.size(), &maps[0]);
	for(unsigned int i = 0; i < maps.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, maps[i]);
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			int w,h;
			w = coreMgr->getGuiMgr()->getWidth();
			h = coreMgr->getGuiMgr()->getHeight();

			if(attachments[i] == GL_COLOR_ATTACHMENT0)
			{
				hasColor0 = true;
				iColor0 = i;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
				node->setTexture("uni_Color0Tex", maps[i], 0);
			}
			else if(attachments[i] == GL_COLOR_ATTACHMENT1)
			{
				hasColor1 = true;
				iColor1 = i;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
				node->setTexture("uni_Color1Tex", maps[i], 1);
			}
			else if(attachments[i] == GL_DEPTH_ATTACHMENT)
			{
				hasDepth = true;
				iDepth = i;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
				node->setTexture("uni_DepthTex", maps[i], 2);
			}

		} glBindTexture(GL_TEXTURE_2D, 0); //Done setting up the texture
	}
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	{
		GLenum fboStatus;

		if(hasColor0 && hasColor1)
		{
			GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
			glDrawBuffers(2, buffers);
		}
		else if(hasColor0)
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
		}
		else if(hasColor1)
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT1);
		}
		else if(hasDepth)
		{
			glDrawBuffer(GL_NONE);
		}

		if(hasColor0)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, maps[iColor0], 0);

		if(hasColor1)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, maps[iColor1], 0);

		if(hasDepth)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, maps[iDepth], 0);

		// check FBO status
		fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(fboStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("GL_FRAMEBUFFER_COMPLETE failed, CANNOT use SCENE FBO\n");
			coreMgr->getLogMgr()->log("BasePostProcess::init", "GL_FrameBuffer_Complete failed, cannot use fbo", Log::L_ERROR);
		}
	} glBindFramebuffer(GL_FRAMEBUFFER, 0);

	node->init();
}

void PostProcess::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void PostProcess::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int PostProcess::getMap(unsigned int i)
{
	if(i >= maps.size())
		return 0;

	return maps[i];
}

unsigned int PostProcess::getAttachment(unsigned int i)
{
	if(i >= attachments.size())
		return 0;

	return attachments[i];
}

Node *PostProcess::getNode() const 
{ 
	return node; 
}

unsigned int PostProcess::getFBO() const
{
	return fbo;
}
