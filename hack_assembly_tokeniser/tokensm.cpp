//a1696375

// a skeleton of the tokens class implementation

#include "tokensm.h"

using namespace std ;

// create a new tokeniser - use readchar function to read the input
// readchar function returns -1 on error or end of input
tokensm::tokensm(int(*_readchar)(void))
{
	// save pointer to the function
	readchar = _readchar ;

	// token value initially ""
	value = "" ;

	// read first character to initialise the tokeniser
	ch = readchar() ;
}

// read next character in the input
void tokensm::nextch()
{
	ch = readchar() ;
}

// return the current token value
string tokensm::token_value()
{
	return value ;
}

// return the next token - "?" on error or end of input
string tokensm::next_token()
{
	while(ch == ' ' || ch == '\n'){
		nextch();
	}

	switch (ch) {
		case '@':{
			nextch();
			stringstream buffer;
			if(isdigit(ch)){
				while(isdigit(ch)){
					buffer << (char)ch;
					nextch();
				}
			} else if (isalpha(ch) || ch == '$' || ch == '_' || ch == ':' || ch == '.'){
				while(isalpha(ch) || ch == '$' || ch == '_' || ch == ':' || ch == '.' || isdigit(ch)){
					buffer << (char)ch;
					nextch();
				}
			}
			value = buffer.str();
			return "address";
			break;
		}
		case '=': {
			value = "=";
			nextch();
			return "equals";
			break;
		}
		case '(':{
			nextch();
			stringstream buffer;
			if (isalpha(ch) || ch == '$' || ch == '_' || ch == ':' || ch == '.'){
				while(isalpha(ch) || ch == '$' || ch == '_' || ch == ':' || ch == '.' || isdigit(ch)){
					buffer << (char)ch;
					nextch();
				}
			}
			if(ch == ')'){
				value = buffer.str();
				nextch();
				return "label";
			}
			break;
		}
		case ';':{
			value = ";";
			nextch();
			return "semi";
			break;
		}
	}


	if(isalpha(ch) || ch == '$' || ch == '_' || ch == ':' || ch == '.' || isdigit(ch) || ch == '-' || ch == '+' || ch == '!' || ch == '&' || ch == '|'){
		stringstream buffer;
		while (isalpha(ch) || ch == '$' || ch == '_' || ch == ':' || ch == '.' || isdigit(ch) || ch == '-' || ch == '+' || ch == '!' || ch == '&' || ch == '|') {
			buffer << (char)ch;
			nextch();
		}
		value = buffer.str();
		return "C-component";
	}


	nextch();
	value = "?";
	return "?" ;
}
