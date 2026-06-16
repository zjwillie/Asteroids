#pragma once

#include <fstream>
#include <string>

#include "LogSink.hpp"

class FileSink : public LogSink {
public:
    FileSink(const std::string& filepath);
    ~FileSink() override;

    void write(const LogEntry& entry) override;

private:
    std::ofstream stream_;
};