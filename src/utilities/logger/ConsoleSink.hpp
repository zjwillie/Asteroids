#pragma once

#include "LogSink.hpp"

class ConsoleSink : public LogSink {
public:
    ConsoleSink();
    void write(const LogEntry& entry) override;
private:
    void printColorTest();
};