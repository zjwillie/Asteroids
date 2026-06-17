#pragma once

#include <string>

#include "LogLevel.hpp"

struct LogEntry {
	double timestamp;
	LogLevel level;
	std::string channel;
	std::string file;
	int line;
	std::string function;
	std::string message;
};