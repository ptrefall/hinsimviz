#include "LogManager.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Resource/ResManager.h>
#include <Engine/Resource/IResource.h>

using namespace Engine;
using namespace Log;

LogManager::LogManager(Core::CoreManager *coreMgr)
: fileLogger(NULL), logLevel((Log::LogLevel)0)
{
	this->coreMgr = coreMgr;
}

LogManager::~LogManager()
{
	if(fileLogger)
	{
		log("LogManager::Destruct", "-=END SESSION=-", Log::L_TOP);
		fileLogger->disable();
		delete fileLogger;
		fileLogger = NULL;
	}
}

int LogManager::init()
{
	Resource::IResource *config = coreMgr->getResMgr()->create("log_config.xml", "XML");
	CL_String fileName = config->getString("Config/Log/Filename");
	CL_String level = config->getString("Config/Log/Level");
	logLevel = text_to_loglevel(level);

	fileLogger = new CL_FileLogger(fileName);

	log("LogManager::Init", "-=START SESSION=-", Log::L_TOP);
	return 0;
}

LogLevel LogManager::text_to_loglevel(const CL_String &text)
{
	CL_String level = CL_StringHelp::text_to_lower(text);
	if(level == "info")
	{
		return Log::L_INFO;
	}
	else if(level == "debug")
	{
		return Log::L_DEBUG;
	}
	else if(level == "warn")
	{
		return Log::L_WARN;
	}
	else if(level == "error")
	{
		return Log::L_ERROR;
	}
	else if(level == "fatal")
	{
		return Log::L_FATAL;
	}
	else if(level == "top")
	{
		return Log::L_TOP;
	}
	else
		return (Log::LogLevel)0;
}

CL_String LogManager::loglevel_to_text(const LogLevel &level)
{
	if(level == Log::L_INFO)
	{
		return "Info";
	}
	else if(level == Log::L_DEBUG)
	{
		return "Debug";
	}
	else if(level == Log::L_WARN)
	{
		return "Warn";
	}
	else if(level == Log::L_ERROR)
	{
		return "Error";
	}
	else if(level == Log::L_FATAL)
	{
		return "Fatal";
	}
	else if(level == Log::L_TOP)
	{
		return "Top";
	}
	else
		return CL_String();
}

void LogManager::log(const CL_String &category, const CL_String &msg, const LogLevel &logLevel)
{
	//Check if we should log this message
	if(logLevel < this->logLevel)
	{
		return;
	}

	CL_String type = cl_format("%1][%2", loglevel_to_text(logLevel), category);
	cl_log_event(type, msg);
	std::cout << "LOG: [" << type.c_str() << "] - " << msg.c_str() << std::endl;
}

void LogManager::log(const CL_String &category, const CL_String &msg, const CL_String &level)
{
	LogLevel ll = text_to_loglevel(level);
	log(category, msg, ll);
}
