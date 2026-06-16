#pragma once

#include <string_view>

enum class LogLevel {
	Debug = 0,
	Info = 1,
	Warn = 2,
	Error = 3,
	Fatal = 4
};

inline std::string_view levelToString(LogLevel level) {
	switch (level) {
	case LogLevel::Debug:	return "DEBUG";
	case LogLevel::Info:	return "INFO ";
	case LogLevel::Warn:	return "WARN ";
	case LogLevel::Error:	return "ERROR";
	case LogLevel::Fatal:	return "FATAL";
	}
	return "LogLevel::levelToString called with unknown level.";
}