//a1696375

#ifndef TOKENS_F_H
#define TOKENS_F_H

// tokeniser for the tokeniser-f program

#include <string>
#include <iostream>
#include <cstdlib>
#include <sstream>

// the tokens class reads tokens using a given input function
class tokensf
{
public:
        tokensf(int(*)()) ;			// create a new tokeniser to read input using the supplied function
        std::string next_token() ;		// read the next token from the input and return its kind
        std::string token_value() ;		// return the current token as a string
private:
	int (*readchar)() ;			// pointer to function to read next character
	void nextch() ;				// read the next character
	int ch ;				// next character to read
	std::string value ;			// value of last token read
};
#endif //TOKENS_F_H
