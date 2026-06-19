// DronTokenizer.cpp

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "DronTokenizer.hpp"

namespace {
    std::string trim(std::string line) {
        const std::string whiteSpace = "\t\r\n\v\f";

        size_t start = line.find_first_not_of(whiteSpace);
        if (start == std::string::npos) return "";

        size_t end = line.find_last_not_of(whiteSpace);

        size_t length = end - start + 1;
        return line.substr(start, length);
    }
}

std::vector<Token> DronTokenizer::tokenize(std::string rawText) {
    std::stringstream ss(rawText);
    std::string line{};

    // let's 
    while (std::getline(ss, line)) {
        // first strip all "white space" including the trailing \r if it exists (fuck you microsoft windows)
        std::string strippedLine = trim(line);

    }

    return tokens_;
}