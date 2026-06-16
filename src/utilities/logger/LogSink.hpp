#pragma once

#include "LogEntry.hpp"

class LogSink {
public:
	virtual void write(const LogEntry& logEntry) = 0;
	virtual ~LogSink() = default;
};