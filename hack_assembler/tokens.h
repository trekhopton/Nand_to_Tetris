//a1696375
#ifndef TOKENS_H
#define TOKENS_H
#include <string>

// the tokens class reads Hack assembly language tokens
class tokens
{
public:
        tokens(int(*)()) ;		// create a new tokeniser to read input using the supplied function
        std::string next_token() ;	// read the next token from the input and return its kind
        std::string token_value() ;	// return the current token as a string
private:
	void *data ;			// private data
};
#endif //TOKENS_H
