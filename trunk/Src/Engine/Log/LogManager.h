#pragma once

#include <ClanLib/core.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Log
{
enum LogLevel
{
	L_INFO = 1,
	L_DEBUG,
	L_WARN,
	L_ERROR,
	L_FATAL,
	L_TOP,
	L_COUNT_COUNT
};

class LogManager
{
public:
	LogManager(Core::CoreManager *coreMgr);
	virtual ~LogManager();

	virtual int init();

	virtual void log(const CL_String &category, const CL_String &msg, const LogLevel &logLevel = Log::L_INFO);
	virtual void log(const CL_String &category, const CL_String &msg, const CL_String &level);

private:
	LogLevel text_to_loglevel(const CL_String &text);
	CL_String loglevel_to_text(const LogLevel &level);
	Core::CoreManager *coreMgr;

	CL_FileLogger *fileLogger;
	LogLevel logLevel;
};

}
}
