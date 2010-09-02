#include "SceneManager.h"
#include "NodeFactory.h"
#include "NodeRegistrator.h"
#include "Camera.h"
#include "PostProcess.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Events/EngineEvent.h>
#include <Engine/Events/IEngineEventManager.h>

#include <GL/glew.h>
#ifdef WIN32
	#include <GL/wglew.h>
#endif
#include <GL/gl.h>

using namespace Engine;
using namespace Scene;

SceneManager::SceneManager(Core::CoreManager *coreMgr)
: factory(NULL), nodeIds(0), root(NULL), camera(NULL), gameStateGuiRoot(NULL), gameStateRoot(NULL)
{
	this->coreMgr = coreMgr;
	colorIds = CL_Vec4f(255.0f, 255.0f, 255.0f, 255.0f);
}

SceneManager::~SceneManager()
{
	if(factory)
	{
		delete factory;
		factory = NULL;
	}
	if(scenePostProcess)
	{
		delete scenePostProcess;
		scenePostProcess = NULL;
	}
}

int SceneManager::init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_LIGHTING);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	factory = new NodeFactory(coreMgr);
	NodeRegistrator::Register(factory);

	root = factory->create("Root", CL_String()); //Placeholder

	Node *test = this->create("Flow3D");
	test->GetProperty<CL_String>("File") = "flow.dat";
	test->init();

	camera = static_cast<Camera*>(factory->create("Camera", CL_String())); //Placeholder
	camera->perspective(40.0f, 1.0f, 1.0f, 1000.0f);
	camera->lookAt(CL_Vec3f(0.0f, 20.0f, 0.0f), 
				   CL_Vec3f(0.0f, 0.0f, -1000.0f), 
				   CL_Vec3f(0.0f, 1.0f, 0.0f));


	//Set up scene post process
	std::vector<unsigned int> sceneMaps;
	sceneMaps.push_back(GL_COLOR_ATTACHMENT0);
	sceneMaps.push_back(GL_COLOR_ATTACHMENT1);
	scenePostProcess = new PostProcess("scenePostProcess", sceneMaps, coreMgr);
	return 0;
}

void SceneManager::reseize(int w, int h)
{
	if(h == 0)
		h = 1;
	if(w == 0)
		w = 1;

	float ratio = (float)(1.0 * ((double)w/(double)h));
	glViewport(0, 0, w, h);
	camera->perspective(40.0f, ratio, 1.0f, 1000.0f);
}

Node *SceneManager::create(const CL_String &type, const CL_String &name)
{
	if(factory == NULL)
		return NULL;
	
	CL_String msg = cl_format("Adding %1 node of type %2 to Scene", name, type);
	coreMgr->getLogMgr()->log("SceneManager::create", msg, Log::L_INFO);
	Node *node = factory->create(type, name);
	if(node == NULL)
		return NULL;

	root->add(node); //Placeholder
	coreMgr->getEngineEventMgr()->SendEvent(Engine::Events::EngineEvent("NodeCreated", Engine::Events::EngineEventValue(node)));
	
	msg = cl_format("Finished adding %1 node of type %2 to Scene", name, type);
	coreMgr->getLogMgr()->log("SceneManager::create", msg, Log::L_INFO);
	return node;
}

void SceneManager::registerForDelete(Node *node)
{
	pendingDelete.push_back(node);
}

void SceneManager::checkForPendingDelete()
{
	for(unsigned int i = 0; i < pendingDelete.size(); i++)
	{
		Node *node = pendingDelete[i];
		delete node;
		node = NULL;
	}
	pendingDelete.clear();
}

unsigned int SceneManager::genNodeId()
{
	nodeIds++;
	return nodeIds;
}

CL_Vec4f SceneManager::genNodeColorId()
{
	float r,g,b,a;
	r = colorIds.r;
	g = colorIds.g;
	b = colorIds.b;
	a = colorIds.a;

	b--;
	if(b==0)
	{
		b = 255;
		g--;
		if(g==0)
		{
			g = 255;
			r--;
			if(r==0)
			{
				//Quite impressive to manage ++16million cells in the grid!
				r = 255;
				a--;
				if(a==0)
				{
					a = 255;
				}
			}
		}
	}

	colorIds.r = r;
	colorIds.g = g;
	colorIds.b = b;
	colorIds.a = a;

	return CL_Vec4f(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
}

int SceneManager::update(double dt)
{
	if(gameStateGuiRoot && gameStateRoot)
	{
		int fail = gameStateGuiRoot->update(dt);
		if(fail)
			return fail;

		fail = gameStateRoot->update(dt);
		if(fail)
			return fail;
	}
	else
	{
		return root->update(dt);
	}
	return 0;
}

int SceneManager::interpolate(double dt)
{
	if(gameStateGuiRoot && gameStateRoot)
	{
		int fail = gameStateGuiRoot->interpolate(dt);
		if(fail)
			return fail;

		fail = gameStateRoot->interpolate(dt);
		if(fail)
			return fail;
	}
	else
	{
		return root->interpolate(dt);
	}
	return 0;
}

int SceneManager::display()
{
	scenePostProcess->bind();
	{
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glCullFace(GL_BACK);

		if(gameStateGuiRoot && gameStateRoot)
		{
			int fail = gameStateGuiRoot->display();
			if(fail)
				return fail;

			fail = gameStateRoot->display();
			if(fail)
				return fail;
		}
		else
		{
			return root->display();
		}
	} scenePostProcess->unbind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int fail = scenePostProcess->getNode()->display();
	if(fail)
		return fail;

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		std::cout << "OpenGL prog error: " << gluErrorString(err) << std::endl;
	std::cout.flush();

	return 0;
}
