// DronTokenizer.cpp

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "DronTokenizer.hpp"
#include "../logger/Logger.hpp"

namespace {
    std::string trim(const std::string& line) {
        const std::string whiteSpace = " \t\r\n\v\f";

        size_t start = line.find_first_not_of(whiteSpace);
        if (start == std::string::npos) return "";

        size_t end = line.find_last_not_of(whiteSpace);

        size_t length = end - start + 1;
        return line.substr(start, length);
    }

    bool is_alpha(const char& c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    bool is_numeric(const char& c) {
        return (c >= '0' && c <= '9');
    }

    bool is_upper(const char& c) {
        return (c >= 'A' && c <= 'Z');
    }

    size_t get_decimal_position(const std::string& line) {
        return line.find('.');
    }
}

void DronTokenizer::handleMultiline(const std::string& trimmed) {
    if (trimmed == "\"\"\"") {
        emitToken(TokenType::MULTILINE_STRING, std::move(currentMultilineContent_), lineNumber_);
        inMultiline_ = false;
        currentMultilineContent_.clear();
    }
    else {
        if (!currentMultilineContent_.empty()) currentMultilineContent_ += '\n';
        currentMultilineContent_ += trimmed;
    }
}

void DronTokenizer::handleSectionHeader(const std::string& trimmed) {
    std::string name = trim(trimmed.substr(1, trimmed.size() - 2));
    emitToken(TokenType::SECTION_HEADER, std::move(name), lineNumber_);
}

void DronTokenizer::handleKeyValue(const std::string& trimmed) {
    if (!is_alpha(trimmed[0])) {
        emitToken(TokenType::UNKNOWN, trimmed, lineNumber_);
        return;
    }

    size_t equalPos = trimmed.find('=');
    if (equalPos == std::string::npos) {
        emitToken(TokenType::UNKNOWN, trimmed, lineNumber_);
        return;
    }

    std::string key = trim(trimmed.substr(0, equalPos));
    std::string value = trim(trimmed.substr(equalPos + 1));

    emitToken(TokenType::KEY, std::move(key), lineNumber_);
    emitToken(TokenType::EQUALS, "=", lineNumber_);

    if (value.empty()) {
        // No value on this line - next line may be """ fence opener
        return;
    }

    tokenizeValue(value);
}

void DronTokenizer::tokenizeValue(const std::string& value) {
    for (size_t position = 0; position < value.size(); ++position) {

        // skip whitespace
        if (value[position] == ' ') {
            continue;
        }

        // inline comment - grab it and emit a token so it will be it's own line now
        if (value[position] == '#') {
            std::string tokenString = value.substr(position);
            emitToken(TokenType::COMMENT, std::move(tokenString), lineNumber_);
            return;
        }

        // strings
        if (value[position] == '"') {
            if (value.substr(position, 3) == "\"\"\"") {
                inMultiline_ = true;
                return;
            }
            ++position; // skip opening quote
            std::string tokenString{};
            while (position < value.size() && value[position] != '"') {
                tokenString += value[position];
                ++position;
            }
            // position is now on closing " (or past end if malformed)
            if (position >= value.size()) {
                emitToken(TokenType::UNKNOWN_VALUE, std::move(tokenString), lineNumber_);
            }
            else {
                emitToken(TokenType::STRING, std::move(tokenString), lineNumber_);
            }
            continue;
        }

        // structural tokens
        if (value[position] == '[') {
            emitToken(TokenType::LEFT_BRACKET, "[", lineNumber_);
            ++containerDepth_;
            continue;
        }
        else if (value[position] == ']') {
            emitToken(TokenType::RIGHT_BRACKET, "]", lineNumber_);
            --containerDepth_;
            continue;
        }
        else if (value[position] == '{') {
            emitToken(TokenType::LEFT_BRACE, "{", lineNumber_);
            ++containerDepth_;
            continue;
        }
        else if (value[position] == '}') {
            emitToken(TokenType::RIGHT_BRACE, "}", lineNumber_);
            --containerDepth_;
            continue;
        }
        else if (value[position] == ',') {
            emitToken(TokenType::COMMA, ",", lineNumber_);
            continue;
        }
        else if (value[position] == '=') {
            emitToken(TokenType::EQUALS, "=", lineNumber_);
            continue;
        }

        // alpha - identifiers, booleans, or keys
        else if (is_alpha(value[position])) {
            if (is_upper(value[position])) {
                std::string tokenString{};
                while (position < value.size() && (is_upper(value[position]) || value[position] == '_' || is_numeric(value[position]))) {
                    tokenString += value[position];
                    ++position;
                }
                --position; // correct for for-loop increment
                emitToken(TokenType::IDENTIFIER, std::move(tokenString), lineNumber_);
            }
            else {
                std::string word{};
                while (position < value.size() && (is_alpha(value[position]) || value[position] == '_' || is_numeric(value[position]))) {
                    word += value[position];
                    ++position;
                }
                --position; // correct for for-loop increment
                if (word == "true" || word == "false") {
                    emitToken(TokenType::BOOLEAN, std::move(word), lineNumber_);
                }
                else {
                    emitToken(TokenType::KEY, std::move(word), lineNumber_);
                }
            }
            continue;
        }

        // numbers
        else if (is_numeric(value[position]) || value[position] == '-' || value[position] == '.') {
            std::string tokenNumeric{};
            while (position < value.size() && (is_numeric(value[position]) || value[position] == '-' || value[position] == '.')) {
                tokenNumeric += value[position];
                ++position;
            }
            --position; // correct for for-loop increment

            // validate
            if (tokenNumeric == "-" || tokenNumeric == ".") {
                LOG_WARN("DronTokenizer", "Line {}: malformed number '{}' - emitting UNKNOWN_VALUE", lineNumber_, tokenNumeric);
                emitToken(TokenType::UNKNOWN_VALUE, std::move(tokenNumeric), lineNumber_);
                continue;
            }

            // normalize -.4 to -0.4
            if (tokenNumeric.size() >= 2 && tokenNumeric[0] == '-' && tokenNumeric[1] == '.') {
                LOG_WARN("DronTokenizer", "Line {}: normalizing '{}' to '-0{}'", lineNumber_, tokenNumeric, tokenNumeric.substr(1));
                tokenNumeric = "-0" + tokenNumeric.substr(1);
            }

            // normalize .4 to 0.4
            if (tokenNumeric[0] == '.') {
                LOG_WARN("DronTokenizer", "Line {}: normalizing '{}' to '0{}'", lineNumber_, tokenNumeric, tokenNumeric);
                tokenNumeric = "0" + tokenNumeric;
            }

            if (get_decimal_position(tokenNumeric) == std::string::npos) {
                emitToken(TokenType::INTEGER, std::move(tokenNumeric), lineNumber_);
            }
            else {
                emitToken(TokenType::DOUBLE, std::move(tokenNumeric), lineNumber_);
            }
            continue;
        }

        // truly unknown
        else {
            emitToken(TokenType::UNKNOWN_VALUE, std::string(1, value[position]), lineNumber_);
        }
    }
}

std::vector<Token> DronTokenizer::tokenize(std::string rawText) {
    tokens_.clear();
    lineNumber_ = 0;
    inMultiline_ = false;
    currentMultilineContent_.clear();
    containerDepth_ = 0;

    std::stringstream ss(rawText);
    std::string line{};

    while (std::getline(ss, line)) {
        ++lineNumber_;
        std::string trimmed = trim(line);

        if (inMultiline_) {
            handleMultiline(trimmed);
            continue;
        }

        if (trimmed.empty()) {
            emitToken(TokenType::BLANK, line, lineNumber_);
            continue;
        }

        if (trimmed[0] == '#') {
            emitToken(TokenType::COMMENT, line, lineNumber_);
            continue;
        }

        if (trimmed[0] == '[') {
            handleSectionHeader(trimmed);
            continue;
        }

        // multiline fence opener on its own line
        if (trimmed == "\"\"\"") {
            inMultiline_ = true;
            currentMultilineContent_.clear();
            continue;
        }

        if (containerDepth_ > 0) {
            tokenizeValue(trimmed);
            continue;
        }

        handleKeyValue(trimmed);
    }

    emitToken(TokenType::END_OF_FILE, "", lineNumber_);
    return tokens_;
}

void DronTokenizer::emitToken(TokenType type, std::string value, uint32_t lineNumber) {
    tokens_.push_back({ type, std::move(value), lineNumber, static_cast<uint32_t>(tokens_.size()) });
}

std::string DronTokenizer::untokenize() {
    untokenized_text_.clear();
    int depth = 0;

    for (size_t i = 0; i < tokens_.size(); ++i) {
        const auto& token = tokens_[i];

        if (token.type == TokenType::LEFT_BRACKET || token.type == TokenType::LEFT_BRACE)  ++depth;
        if (token.type == TokenType::RIGHT_BRACKET || token.type == TokenType::RIGHT_BRACE) --depth;

        untokenized_text_ += untokenizeValue(token);

        // spacing
        if (token.type == TokenType::EQUALS) {
            untokenized_text_ += " ";
        }
        else if (token.type == TokenType::KEY && i + 1 < tokens_.size() && tokens_[i + 1].type == TokenType::EQUALS) {
            untokenized_text_ += " ";
        }
        else if (token.type == TokenType::COMMA) {
            untokenized_text_ += " ";
        }

        // newline after value or closing container at depth 0
        bool isValue = (token.type == TokenType::STRING ||
            token.type == TokenType::MULTILINE_STRING ||
            token.type == TokenType::INTEGER ||
            token.type == TokenType::DOUBLE ||
            token.type == TokenType::BOOLEAN ||
            token.type == TokenType::IDENTIFIER ||
            token.type == TokenType::UNKNOWN ||
            token.type == TokenType::UNKNOWN_VALUE);

        bool isClosing = (token.type == TokenType::RIGHT_BRACKET ||
            token.type == TokenType::RIGHT_BRACE);

        if ((isValue || isClosing) && depth == 0) {
            untokenized_text_ += "\n";
        }

        // trivia always newlines
        if (token.type == TokenType::BLANK) {
            untokenized_text_ += token.value + "\n";
        }
        else if (token.type == TokenType::SECTION_HEADER || token.type == TokenType::COMMENT) {
            if (untokenized_text_.empty() || untokenized_text_.back() != '\n')
                untokenized_text_ += "\n";
        }
    }

    return untokenized_text_;
}

std::string DronTokenizer::untokenizeValue(Token token) {
    switch (token.type) {
    case TokenType::STRING:
        return "\"" + token.value + "\"";

    case TokenType::LEFT_BRACKET:  return "[";
    case TokenType::RIGHT_BRACKET: return "]";
    case TokenType::LEFT_BRACE:    return "{";
    case TokenType::RIGHT_BRACE:   return "}";
    case TokenType::COMMA:         return ",";
    case TokenType::EQUALS:        return "=";

    case TokenType::MULTILINE_STRING:
        return "\"\"\"\n" + token.value + "\n\"\"\"";

    case TokenType::SECTION_HEADER:
        return "[" + token.value + "]";

    // Identifiers, Keys, Booleans, Numbers, and Errors print their raw value
    case TokenType::IDENTIFIER:
    case TokenType::KEY:
    case TokenType::BOOLEAN:
    case TokenType::INTEGER:
    case TokenType::DOUBLE:
    case TokenType::UNKNOWN:
    case TokenType::UNKNOWN_VALUE:
    case TokenType::COMMENT:
    case TokenType::BLANK:
        return token.value;

    case TokenType::END_OF_FILE:
    default:
        return "";
    }
}

// ************************* TESTING ****************

void DronTokenizer::test(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        LOG_ERROR("DronTokenizer", "Failed to open: {}", filepath);
        return;
    }
    std::string content((std::istreambuf_iterator<char>(file)), {});
    file.close();

    // === PASS 1 ===
    DronTokenizer t1;
    auto tokens1 = t1.tokenize(content);
    LOG_DEBUG("DronTokenizer", "=== PASS 1: {} tokens ===", tokens1.size());
    for (const auto& tok : tokens1)
        LOG_DEBUG("DronTokenizer", "Token {:4} | Line {:3} | Type {:<16} | Value: '{}'",
            tok.index, tok.lineNumber, tokenTypeToString(tok.type), tok.value);

    // === UNTOKENIZE ===
    std::string rebuilt = t1.untokenize();
    LOG_DEBUG("DronTokenizer", "=== UNTOKENIZED TEXT ===");
    std::stringstream ss(rebuilt);
    std::string line;
    uint32_t lineNum = 0;
    while (std::getline(ss, line))
        LOG_DEBUG("DronTokenizer", "{:4} | {}", ++lineNum, line);

    // === PASS 2 ===
    DronTokenizer t2;
    auto tokens2 = t2.tokenize(rebuilt);
    LOG_DEBUG("DronTokenizer", "=== PASS 2: {} tokens ===", tokens2.size());
    for (const auto& tok : tokens2)
        LOG_DEBUG("DronTokenizer", "Token {:4} | Line {:3} | Type {:<16} | Value: '{}'",
            tok.index, tok.lineNumber, tokenTypeToString(tok.type), tok.value);

    // === COMPARE ===
    LOG_DEBUG("DronTokenizer", "=== PASS 1: {} tokens | PASS 2: {} tokens | {} ===",
        tokens1.size(), tokens2.size(),
        tokens1.size() == tokens2.size() ? "MATCH" : "MISMATCH");
}