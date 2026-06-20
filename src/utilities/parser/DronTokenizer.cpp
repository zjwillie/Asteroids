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
        tokens_.push_back({ TokenType::STRING, currentMultilineContent_, lineNumber_ });
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
    tokens_.push_back({ TokenType::SECTION_HEADER, name, lineNumber_ });
}

void DronTokenizer::handleKeyValue(const std::string& trimmed) {
    if (!is_alpha(trimmed[0])) {
        tokens_.push_back({ TokenType::UNKNOWN, trimmed, lineNumber_ });
        return;
    }

    size_t equalPos = trimmed.find('=');
    if (equalPos == std::string::npos) {
        tokens_.push_back({ TokenType::UNKNOWN, trimmed, lineNumber_ });
        return;
    }

    std::string key = trim(trimmed.substr(0, equalPos));
    std::string value = trim(trimmed.substr(equalPos + 1));

    tokens_.push_back({ TokenType::KEY, key, lineNumber_ });
    tokens_.push_back({ TokenType::EQUALS, "=", lineNumber_ });

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

        // inline comment - stop processing rest of line
        if (value[position] == '#') {
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
                tokens_.push_back({ TokenType::UNKNOWN_VALUE, tokenString, lineNumber_ });
            }
            else {
                tokens_.push_back({ TokenType::STRING, tokenString, lineNumber_ });
            }
            continue;
        }

        // structural tokens
        if (value[position] == '[') {
            tokens_.push_back({ TokenType::LEFT_BRACKET, "[", lineNumber_ });
            ++containerDepth_;
            continue;
        }
        else if (value[position] == ']') {
            tokens_.push_back({ TokenType::RIGHT_BRACKET, "]", lineNumber_ });
            --containerDepth_;
            continue;
        }
        else if (value[position] == '{') {
            tokens_.push_back({ TokenType::LEFT_BRACE, "{", lineNumber_ });
            ++containerDepth_;
            continue;
        }
        else if (value[position] == '}') {
            tokens_.push_back({ TokenType::RIGHT_BRACE, "}", lineNumber_ });
            --containerDepth_;
            continue;
        }
        else if (value[position] == ',') {
            tokens_.push_back({ TokenType::COMMA, ",", lineNumber_ });
            continue;
        }
        else if (value[position] == '=') {
            tokens_.push_back({ TokenType::EQUALS, "=", lineNumber_ });
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
                tokens_.push_back({ TokenType::IDENTIFIER, tokenString, lineNumber_ });
            }
            else {
                std::string word{};
                while (position < value.size() && (is_alpha(value[position]) || value[position] == '_' || is_numeric(value[position]))) {
                    word += value[position];
                    ++position;
                }
                --position; // correct for for-loop increment
                if (word == "true" || word == "false") {
                    tokens_.push_back({ TokenType::BOOLEAN, word, lineNumber_ });
                }
                else {
                    tokens_.push_back({ TokenType::KEY, word, lineNumber_ });
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
                tokens_.push_back({ TokenType::UNKNOWN_VALUE, tokenNumeric, lineNumber_ });
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
                tokens_.push_back({ TokenType::INTEGER, tokenNumeric, lineNumber_ });
            }
            else {
                tokens_.push_back({ TokenType::DOUBLE, tokenNumeric, lineNumber_ });
            }
            continue;
        }

        // truly unknown
        else {
            tokens_.push_back({ TokenType::UNKNOWN_VALUE, std::string(1, value[position]), lineNumber_ });
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
            tokens_.push_back({ TokenType::BLANK, line, lineNumber_ });
            continue;
        }

        if (trimmed[0] == '#') {
            tokens_.push_back({ TokenType::COMMENT, line, lineNumber_ });
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

    tokens_.push_back({ TokenType::END_OF_FILE, "", lineNumber_ });
    return tokens_;
}

void DronTokenizer::test(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        LOG_ERROR("DronTokenizer", "Failed to open: {}", filepath);
        return;
    }

    std::string content((std::istreambuf_iterator<char>(file)), {});
    file.close();

    DronTokenizer tokenizer;
    auto tokens = tokenizer.tokenize(content);

    LOG_DEBUG("DronTokenizer", "=== Tokenizing: {} ===", filepath);
    for (const auto& token : tokens) {
        LOG_DEBUG("DronTokenizer", "Line {:3} | Type {:2} | Value: '{}'",
            token.lineNumber,
            static_cast<int>(token.type),
            token.value
        );
    }
    LOG_DEBUG("DronTokenizer", "=== {} tokens total ===", tokens.size());
}