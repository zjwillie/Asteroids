#pragma once

#include <string>

#include "LogLevel.hpp"

struct LogEntry {
	float timestamp;
	LogLevel level;
	std::string channel;
	std::string file;
	int line;
	std::string function;
	std::string message;
};