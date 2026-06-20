// DronTokenizer.hpp

#pragma once
#include <vector>
#include <string>

#include "DronToken.hpp"

class DronTokenizer {
public:
	std::vector<Token> tokenize(std::string rawText);
	static void test(const std::string& filepath);

private:
	void handleMultiline(const std::string& trimmed);
	void handleSectionHeader(const std::string& trimmed);
	void handleKeyValue(const std::string& trimmed);
	void tokenizeValue(const std::string& value);

	std::vector<std::string> rawLines_{};
	
	uint32_t lineNumber_{ 0 };

	// for multiline strings
	bool inMultiline_{ false };
	std::string currentMultilineContent_{};

	// track multiline [ and {
	uint32_t containerDepth_{ 0 };

	std::vector<Token> tokens_{};
};