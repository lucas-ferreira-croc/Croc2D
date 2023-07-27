#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

const char* const ANSI_COLOR_GREEN = "\x1b[32m";
const char* const ANSI_COLOR_RED = "\x1b[31m";
const char* const ANSI_COLOR_YELLOW = "\x1b[33m";

enum class LogType
{
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

struct LogEntry
{
	LogType type;
	std::string message;
};

class Logger
{
public:
	static std::vector<LogEntry> messages;
	static void log(const std::string& message);
	static void warn(const std::string& warn);
	static void error(const std::string& error);
private:
	static std::string get_datetime();
};

#endif 