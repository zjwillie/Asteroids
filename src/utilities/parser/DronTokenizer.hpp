// DronTokenizer.hpp
#pragma once
#include <vector>
#include <string>
#include "DronToken.hpp"

class DronTokenizer {
public:
    // Core tokenization variants
    std::vector<Token> tokenize(std::string rawText);
    std::vector<Token> tokenizeFile(const std::string& filepath);

    // Manual token override for layout-preserving serialization
    void setTokens(const std::vector<Token>& tokens) { tokens_ = tokens; }
    const std::vector<Token>& getTokens() const { return tokens_; }

    // Untokenizer operations
    std::string untokenize();
    std::string untokenizeValue(Token token);

    static void test(const std::string& filepath);

private:
    void handleMultiline(const std::string& trimmed);
    void handleSectionHeader(const std::string& trimmed);
    void handleKeyValue(const std::string& trimmed);
    void tokenizeValue(const std::string& value);
    void emitToken(TokenType type, std::string value, uint32_t lineNumber);

    uint32_t lineNumber_{ 0 };
    bool inMultiline_{ false };
    std::string currentMultilineContent_{};
    uint32_t containerDepth_{ 0 };

    std::vector<Token> tokens_{};
    std::string untokenized_text_{};
};