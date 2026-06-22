// DronParser.hpp
#pragma once

#include <string>
#include <vector>
#include "DronToken.hpp"
#include "DronNode.hpp"

class DronParser {
public:
    explicit DronParser(const std::vector<Token>& tokens);

    DronDocument parse();

    // Layout-preserving blueprint exporter
    void writeDocument(const DronDocument& doc, const std::string& outputPath);

    // Testing & Diagnostics Engine
    static void test(const std::string& filepath);
    static void verifyFullRoundTrip(const std::string& originalText);
    static void testEntityRoundTrip(const std::string& filepath);

private:
    // token stream navigation
    const Token& peek() const;
    const Token& advance();
    bool isAtEnd() const;
    void skipTrivia();

    // parse functions
    void parseSection(DronDocument& document);
    std::pair<std::string, DronNode> parseKeyValue();
    DronNode parseValue();
    DronNode parseList();
    DronNode parseMap();
    DronNode parseScalar();

    const std::vector<Token>& tokens_;
    size_t position_{ 0 };
};