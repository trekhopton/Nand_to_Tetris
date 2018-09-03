#ifndef JACK_TOKENS_H
#define JACK_TOKENS_H
#include <string>
#include "jacksymbols.h"

// the jacktokens class reads Jack tokens from cin
// next_token() returns integers, strings and identifiers as their class
// next_token() returns all other tokens as their value
class jacktokens
{
public:
	jacktokens() ;				// initialise the tokeniser
        std::string next_token() ;		// read the next token from the input and return its kind
        std::string token_class() ;		// return the current token class as a string
        std::string token_value() ;		// return the current token as a string
	void deleteme() ;			// delete this object
private:
	void nextch() ;				// read next character
	int ch ;				// one character lookahead
	std::string tokenclass ;		// the current token class
	std::string token ;			// the current token value
	symbols_string *symbols ;		// internal symbol table - maps token values to token class
};

#endif //JACK_TOKENS_H

