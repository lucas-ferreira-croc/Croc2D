#include "logger.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

std::vector<LogEntry> Logger::messages;

void Logger::log(const std::string& message)
{
	LogEntry logEntry = { 
		LogType::LOG_INFO,
		std::string(ANSI_COLOR_GREEN) + "LOG[" + get_datetime() + "] " + message
	};

	std::cout << logEntry.message <<  std::endl;
	messages.push_back(logEntry);
}

void Logger::warn(const std::string& warn)
{
	LogEntry logEntry = {
		LogType::LOG_WARNING,
		std::string(ANSI_COLOR_YELLOW) + "ERROR[" + get_datetime() + "] " + warn
	};

	std::cerr << logEntry.message << std::endl;
	messages.push_back(logEntry);
}


void Logger::error(const std::string& error)
{
	LogEntry logEntry = {
		LogType::LOG_ERROR,
		std::string(ANSI_COLOR_RED) + "ERROR[" + get_datetime() + "] " + error
	};

	std::cerr << logEntry.message << std::endl;
	messages.push_back(logEntry);
}


std::string Logger::get_datetime()
{
	std::chrono::time_point now = std::chrono::system_clock::now();
	std::time_t now_datetime = std::chrono::system_clock::to_time_t(now);
	std::stringstream ss;
	ss << std::put_time(std::localtime(&now_datetime), "%F %T");
	return ss.str();
}