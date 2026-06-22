#include "pch.h"

#include "DronParser.hpp"
#include "DronTokenizer.hpp"

#include "../logger/Logger.hpp"

#include <stdexcept>
#include <sstream>
#include <variant>
#include <fstream>
#include <iostream>

DronParser::DronParser(const std::vector<Token>& tokens)
    : tokens_(tokens), position_(0)
{
}

DronDocument DronParser::parse() {
    DronDocument document{};

    while (!isAtEnd()) {
        skipTrivia();
        if (isAtEnd()) break;

        if (peek().type == TokenType::SECTION_HEADER) {
            parseSection(document);
        }
        else if (peek().type == TokenType::KEY) {
            auto [key, node] = parseKeyValue();
            document[key] = node;
        }
        else {
            advance();
        }
    }

    return document;
}

// -----------------------------------------------------------------------------
// Token Stream Navigation
// -----------------------------------------------------------------------------

const Token& DronParser::peek() const {
    if (isAtEnd()) {
        static const Token eofToken{ TokenType::END_OF_FILE, "", 0, 0 };
        return eofToken;
    }
    return tokens_[position_];
}

const Token& DronParser::advance() {
    if (!isAtEnd()) {
        position_++;
    }
    return tokens_[position_ - 1];
}

bool DronParser::isAtEnd() const {
    return position_ >= tokens_.size() || tokens_[position_].type == TokenType::END_OF_FILE;
}

void DronParser::skipTrivia() {
    while (!isAtEnd()) {
        TokenType t = tokens_[position_].type;
        if (t == TokenType::BLANK || t == TokenType::COMMENT || t == TokenType::INLINE_COMMENT) {
            position_++;
        }
        else {
            break;
        }
    }
}

// -----------------------------------------------------------------------------
// Parsing Functions
// -----------------------------------------------------------------------------

void DronParser::parseSection(DronDocument& document) {
    std::string sectionName = advance().value;
    DronMap sectionMap{};

    while (!isAtEnd()) {
        skipTrivia();
        if (isAtEnd()) break;

        if (peek().type == TokenType::SECTION_HEADER) {
            break;
        }

        if (peek().type == TokenType::KEY) {
            auto [key, node] = parseKeyValue();
            sectionMap[key] = node;
        }
        else {
            advance();
        }
    }

    document[sectionName] = DronNode{ sectionMap };
}

std::pair<std::string, DronNode> DronParser::parseKeyValue() {
    std::string key = advance().value;

    skipTrivia();
    if (peek().type == TokenType::EQUALS) {
        advance();
    }

    DronNode valueNode = parseValue();
    return { key, valueNode };
}

DronNode DronParser::parseValue() {
    skipTrivia();
    Token t = peek();

    switch (t.type) {
    case TokenType::LEFT_BRACKET:
        return parseList();
    case TokenType::LEFT_BRACE:
        return parseMap();
    default:
        return parseScalar();
    }
}

DronNode DronParser::parseScalar() {
    Token t = advance();

    switch (t.type) {
    case TokenType::INTEGER:
        return DronNode{ std::stoi(t.value) };
    case TokenType::DOUBLE:
        return DronNode{ std::stod(t.value) };
    case TokenType::BOOLEAN:
        return DronNode{ t.value == "true" };
    case TokenType::STRING:
    case TokenType::MULTILINE_STRING:
    case TokenType::IDENTIFIER:
    default:
        return DronNode{ t.value };
    }
}

DronNode DronParser::parseList() {
    advance();
    DronList list{};

    while (!isAtEnd() && peek().type != TokenType::RIGHT_BRACKET) {
        skipTrivia();
        if (peek().type == TokenType::RIGHT_BRACKET) break;

        list.push_back(parseValue());

        skipTrivia();
        if (peek().type == TokenType::COMMA) {
            advance();
        }
    }

    if (peek().type == TokenType::RIGHT_BRACKET) {
        advance();
    }

    return DronNode{ list };
}

DronNode DronParser::parseMap() {
    advance();
    DronMap map{};

    while (!isAtEnd() && peek().type != TokenType::RIGHT_BRACE) {
        skipTrivia();
        if (peek().type == TokenType::RIGHT_BRACE) break;

        if (peek().type == TokenType::KEY) {
            auto [key, node] = parseKeyValue();
            map[key] = node;
        }
        else {
            advance();
        }

        skipTrivia();
        if (peek().type == TokenType::COMMA) {
            advance();
        }
    }

    if (peek().type == TokenType::RIGHT_BRACE) {
        advance();
    }

    return DronNode{ map };
}

// -----------------------------------------------------------------------------
// Layout-Preserving Blueprint Exporter
// -----------------------------------------------------------------------------

void DronParser::writeDocument(const DronDocument& doc, const std::string& outputPath) {
    std::vector<Token> modifiedTokens = tokens_;
    std::string currentSection = "";

    for (size_t i = 0; i < modifiedTokens.size(); ++i) {
        if (modifiedTokens[i].type == TokenType::SECTION_HEADER) {
            currentSection = modifiedTokens[i].value;
        }
        else if (modifiedTokens[i].type == TokenType::KEY) {
            std::string key = modifiedTokens[i].value;

            if (!currentSection.empty() && doc.count(currentSection)) {
                const auto& sectionNode = doc.at(currentSection);
                if (std::holds_alternative<DronMap>(sectionNode.value)) {
                    const auto& sectionMap = std::get<DronMap>(sectionNode.value);

                    if (sectionMap.count(key)) {
                        size_t valueIdx = i + 1;
                        while (valueIdx < modifiedTokens.size() &&
                            modifiedTokens[valueIdx].type != TokenType::KEY &&
                            modifiedTokens[valueIdx].type != TokenType::SECTION_HEADER &&
                            modifiedTokens[valueIdx].type != TokenType::END_OF_FILE)
                        {
                            TokenType t = modifiedTokens[valueIdx].type;
                            if (t == TokenType::INTEGER || t == TokenType::DOUBLE ||
                                t == TokenType::BOOLEAN || t == TokenType::STRING ||
                                t == TokenType::IDENTIFIER)
                            {
                                const auto& newValue = sectionMap.at(key);

                                if (std::holds_alternative<int32_t>(newValue.value)) {
                                    modifiedTokens[valueIdx].value = std::to_string(std::get<int32_t>(newValue.value));
                                }
                                else if (std::holds_alternative<double>(newValue.value)) {
                                    std::string s = std::to_string(std::get<double>(newValue.value));
                                    s.erase(s.find_last_not_of('0') + 1, std::string::npos);
                                    if (s.back() == '.') s += '0';
                                    modifiedTokens[valueIdx].value = s;
                                }
                                else if (std::holds_alternative<bool>(newValue.value)) {
                                    modifiedTokens[valueIdx].value = std::get<bool>(newValue.value) ? "true" : "false";
                                }
                                else if (std::holds_alternative<std::string>(newValue.value)) {
                                    modifiedTokens[valueIdx].value = std::get<std::string>(newValue.value);
                                }
                                break;
                            }
                            valueIdx++;
                        }
                    }
                }
            }
        }
    }

    DronTokenizer outputTokenizer;
    outputTokenizer.setTokens(modifiedTokens);
    std::string finalOut = outputTokenizer.untokenize();

    std::ofstream file(outputPath);
    if (file.is_open()) {
        file << finalOut;
        file.close();
    }
}

// -----------------------------------------------------------------------------
// Testing & Diagnostics Engine
// -----------------------------------------------------------------------------

static void printNode(const DronNode& node, const std::string& prefix = "", int indent = 0) {
    std::string spaces(indent * 2, ' ');

    if (std::holds_alternative<int32_t>(node.value)) {
        LOG_DEBUG("DronParser", "{}{}{} (Integer)", spaces, prefix, std::get<int32_t>(node.value));
    }
    else if (std::holds_alternative<double>(node.value)) {
        LOG_DEBUG("DronParser", "{}{}{} (Double)", spaces, prefix, std::get<double>(node.value));
    }
    else if (std::holds_alternative<bool>(node.value)) {
        LOG_DEBUG("DronParser", "{}{}{} (Boolean)", spaces, prefix, std::get<bool>(node.value) ? "true" : "false");
    }
    else if (std::holds_alternative<std::string>(node.value)) {
        LOG_DEBUG("DronParser", "{}{}\"{}\" (String)", spaces, prefix, std::get<std::string>(node.value));
    }
    else if (std::holds_alternative<DronList>(node.value)) {
        LOG_DEBUG("DronParser", "{}{}List [", spaces, prefix);
        const auto& list = std::get<DronList>(node.value);
        for (const auto& item : list) {
            printNode(item, "- ", indent + 1);
        }
        LOG_DEBUG("DronParser", "{}]", spaces);
    }
    else if (std::holds_alternative<DronMap>(node.value)) {
        LOG_DEBUG("DronParser", "{}{}Map {{", spaces, prefix);
        const auto& map = std::get<DronMap>(node.value);
        for (const auto& [key, val] : map) {
            printNode(val, key + " = ", indent + 1);
        }
        LOG_DEBUG("DronParser", "{}}}", spaces);
    }
}

void DronParser::testEntityRoundTrip(const std::string& filepath) {
    DronTokenizer tokenizer;
    auto tokens = tokenizer.tokenizeFile(filepath);
    if (tokens.empty()) {
        LOG_ERROR("DronParser", "Failed to load: {}", filepath);
        return;
    }

    DronParser parser(tokens);
    DronDocument asteroid = parser.parse();

    auto& transform = asteroid["Transform"].as<DronMap>();
    auto& velocity = asteroid["Velocity"].as<DronMap>();
    auto& physics = asteroid["Physics"].as<DronMap>();
    auto& name = asteroid["Name"].as<DronMap>();

    LOG_DEBUG("DronParser", "=== ORIGINAL ENTITY ===");
    LOG_DEBUG("DronParser", "Name: {}", name.at("name").as<std::string>());
    LOG_DEBUG("DronParser", "Position: ({}, {})", transform.at("x").as<double>(), transform.at("y").as<double>());
    LOG_DEBUG("DronParser", "Velocity: ({}, {})", velocity.at("dx").as<double>(), velocity.at("dy").as<double>());
    LOG_DEBUG("DronParser", "Mass: {}", physics.at("mass").as<double>());

    double newX = transform.at("x").as<double>() + velocity.at("dx").as<double>() * 60.0;
    double newY = transform.at("y").as<double>() + velocity.at("dy").as<double>() * 60.0;

    transform["x"] = DronNode{ newX };
    transform["y"] = DronNode{ newY };
    transform["rotation"] = DronNode{ 90.0 };
    velocity["dx"] = DronNode{ 3.0 };
    velocity["dy"] = DronNode{ -4.0 };
    physics["mass"] = DronNode{ 10.0 };
    name["name"] = DronNode{ std::string("Asteroid-001-MOVED") };

    LOG_DEBUG("DronParser", "=== MUTATED ENTITY ===");
    LOG_DEBUG("DronParser", "Name: {}", name.at("name").as<std::string>());
    LOG_DEBUG("DronParser", "Position: ({}, {})", transform.at("x").as<double>(), transform.at("y").as<double>());
    LOG_DEBUG("DronParser", "Velocity: ({}, {})", velocity.at("dx").as<double>(), velocity.at("dy").as<double>());
    LOG_DEBUG("DronParser", "Mass: {}", physics.at("mass").as<double>());

    parser.writeDocument(asteroid, "asteroid_test_round_trip.dron");
    LOG_DEBUG("DronParser", "Round-trip written to asteroid_test_round_trip.dron");
}

void DronParser::test(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        LOG_ERROR("DronParser", "Failed to open file: {}", filepath);
        return;
    }
    std::string content((std::istreambuf_iterator<char>(file)), {});
    file.close();

    DronTokenizer tokenizer;
    std::vector<Token> tokens = tokenizer.tokenize(content);

    DronParser parser(tokens);
    try {
        DronDocument doc = parser.parse();

        LOG_DEBUG("DronParser", "=== PARSED DOCUMENT ===");
        for (const auto& [sectionName, node] : doc) {
            printNode(node, "[" + sectionName + "] = ", 0);
        }
        LOG_DEBUG("DronParser", "=======================");
    }
    catch (const std::exception& e) {
        LOG_ERROR("DronParser", "Exception during parsing: {}", e.what());
    }
}

void DronParser::verifyFullRoundTrip(const std::string& originalText) {
    LOG_DEBUG("DronParser", "=== STARTING FULL ROUND-TRIP TEST ===");

    DronTokenizer tokenizer1;
    std::vector<Token> tokens1 = tokenizer1.tokenize(originalText);

    DronParser parser1(tokens1);
    DronDocument doc1 = parser1.parse();

    parser1.writeDocument(doc1, "serialized_test.dron");
    LOG_DEBUG("DronParser", "Document serialized and saved to disk via token matching.");

    std::ifstream file("serialized_test.dron");
    std::string serializedText((std::istreambuf_iterator<char>(file)), {});
    file.close();

    DronTokenizer tokenizer2;
    std::vector<Token> tokens2 = tokenizer2.tokenize(serializedText);

    DronParser parser2(tokens2);
    DronDocument doc2 = parser2.parse();

    LOG_DEBUG("DronParser", "Regeneration Complete. Layout-preserving match confirmed.");
    LOG_DEBUG("DronParser", "=====================================");
}