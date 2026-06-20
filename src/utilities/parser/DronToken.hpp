#pragma once

// DronToken.hpp

#include <string>

enum class TokenType {
	// structural
	SECTION_HEADER,
	EQUALS,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	LEFT_BRACE,
	RIGHT_BRACE,
	TRIPLE_QUOTE,
	COMMA,
	COMMENT,
	BLANK,

	// errors
	UNKNOWN,
	UNKNOWN_VALUE,

	// values
	INTEGER,
	DOUBLE,
	BOOLEAN,
	STRING,
	IDENTIFIER,

	// keys
	KEY,
	
	// housekeeping
	END_OF_FILE
};

struct Token {
	TokenType type;
	std::string value;
	uint32_t lineNumber;
};