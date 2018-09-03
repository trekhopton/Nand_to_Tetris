//a1696375

// a skeleton of the tokens class implementation

#include "tokensf.h"

using namespace std ;

// create a new tokeniser - use readchar function to read the input
// readchar function returns -1 on error or end of input
tokensf::tokensf(int(*_readchar)(void))
{
	// save pointer to the function
	readchar = _readchar ;

	// token value initially ""
	value = "" ;

	// read first character to initialise the tokeniser
	ch = readchar() ;
}

// read next character in the input
void tokensf::nextch()
{
	ch = readchar() ;
}

// return the current token value
string tokensf::token_value()
{
	return value ;
}

// return the next token - "?" on error or end of input
string tokensf::next_token()
{
	while(ch != -1){
		if(ch == '/'){
			nextch();
			//look for // comments and ignore them
			if(ch == '/'){
				while(ch != '\n'){
					nextch();
				}
			//look for /* */ comments and ignore them
			} else if (ch == '*'){
				nextch();
				if(ch == '/'){
					nextch();
				} else {
					bool inComment = 1;
					while(inComment){
						if(ch == '*'){
							nextch();
							if(ch == '/'){
								nextch();
								inComment = 0;
							} else {
								nextch();
							}
						} else {
							nextch();
						}
					}
				}
			}
		}

		// skip any white space
		while(ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t'){
			nextch();
		}

		// if the character is a digit, letter or operator then while it is, get what's next
		// compare that against all the valid commands and if a match is found, return the appropriate
		// type after setting the value
		if(toupper(isalpha(ch)) || ch == '$' || ch == '_' || ch == ':' || ch == '.' ||
		 isdigit(ch) || ch == '-' || ch == '+' || ch == '!' || ch == '&' || ch == '|')
		{
			stringstream buffer;
			while (toupper(isalpha(ch)) || ch == '$' || ch == '_' || ch == ':' ||
			 ch == '.' || isdigit(ch) || ch == '-' || ch == '+' || ch == '!' ||
			  ch == '&' || ch == '|')
			{
				if(isalpha(ch)){
					toupper(ch);
				}
				if(toupper(ch) == 'J'){
					stringstream buffer;
					while(isalpha(ch)){
						buffer << (char)toupper(ch);
						nextch();
					}
					if(buffer.str() == "JMP" || buffer.str() == "JLT" ||
					buffer.str() == "JLE" || buffer.str() == "JGT" ||
					buffer.str() == "JGE" || buffer.str() == "JEQ" ||
					buffer.str() == "JNE"){
						value = buffer.str();
						return "jump";
					}
				} else if (toupper(ch) == 'N'){
					stringstream buffer;
					while(isalpha(ch)){
						buffer << (char)toupper(ch);
						nextch();
					}
					if(buffer.str() == "NULL"){
						value = buffer.str();
						return "null";
					}
				}
				// dest, dest-comp? and comp
				stringstream destCompBuffer;
				while(toupper(ch) == 'A' || toupper(ch) == 'M' || toupper(ch) == 'D' ||
				toupper(ch) == '1' || toupper(ch) == '0' || ch == '-' || ch == '+' ||
				ch == '!' || ch == '&' || ch == '|'){
					destCompBuffer << (char)toupper(ch);
					nextch();
				}
				if(destCompBuffer.str() == "MD" || destCompBuffer.str() == "AM" ||
				destCompBuffer.str() == "AD" || destCompBuffer.str() == "AMD"){
					value = destCompBuffer.str();
					return "dest";
				}
				if(destCompBuffer.str() == "A" || destCompBuffer.str() == "M" || destCompBuffer.str() == "D"){
					value = destCompBuffer.str();
					return "dest-comp?";
				}
				if(destCompBuffer.str() == "0" || destCompBuffer.str() == "1" ||
					destCompBuffer.str() == "-1" || destCompBuffer.str() == "!D" ||
					destCompBuffer.str() == "!A" || destCompBuffer.str() == "-D" ||
					destCompBuffer.str() == "-A" || destCompBuffer.str() == "D+1" ||
					destCompBuffer.str() == "A+1" || destCompBuffer.str() == "D-1" ||
					destCompBuffer.str() == "A-1" || destCompBuffer.str() == "D+A" ||
					destCompBuffer.str() == "D-A" || destCompBuffer.str() == "A-D" ||
					destCompBuffer.str() == "D&A" || destCompBuffer.str() == "D|A" ||
					destCompBuffer.str() == "!M" || destCompBuffer.str() == "-M" ||
					destCompBuffer.str() == "M+1" || destCompBuffer.str() == "M-1" ||
					destCompBuffer.str() == "D+M" || destCompBuffer.str() == "D-M" ||
					destCompBuffer.str() == "M-D" || destCompBuffer.str() == "D&M" ||
					destCompBuffer.str() == "D|M"){
					value = destCompBuffer.str();
					return "comp";
				}
			}
		}

		//check the character to see if it's a valid symbol and if it is, set the
		//value to the following string and return the appropriate type
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
			case ';':{
				value = ";";
				nextch();
				return "semi";
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
					if(ch == ')'){
						value = buffer.str();
						nextch();
						return "label";
					}
				}
				break;
			}
		}


	}
	// if the character is not vaild
	nextch();
	value = "?";
	return "?" ;
}
