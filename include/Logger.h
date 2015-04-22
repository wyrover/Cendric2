#pragma once

#include "global.h"

// the importance of the log output. 0 is the highest (None) 
// and should never be used to log messages (would have no effect)
enum class LogLevel
{
	None,
	Error,
	Warning,
	Info,
	Debug,
	Verbose
};

class Logger
{
public:
	void logError(const std::string& source, const std::string& message);
	void logWarning(const std::string& source, const std::string& message);
	void logInfo(const std::string& source, const std::string& message);
	// logs the message with importance level. The source should describe the calling class and/or method.
	void log(LogLevel level, const std::string& source, const std::string& message);
	// sets the log level to 'level'. The log will only output messages, that have importance 'level' or higher.
	// if the log level is set to 'None', the log won't output anything.
	void setLogLevel(LogLevel level);

private:
	LogLevel m_logLevel = LogLevel::Debug;
};