// DronTokenizer.hpp

#pragma once
#include <vector>
#include <string>

#include "DronToken.hpp"

class DronTokenizer {
public:
	std::vector<Token> tokenize(std::string rawText);

private:
	std::vector<std::string> rawLines_{};
	
	uint32_t lineNumber_{ 0 };
	bool inMultiline_{ false };
	std::string currentMultilineContent_{};

	std::vector<Token> tokens_{};
};