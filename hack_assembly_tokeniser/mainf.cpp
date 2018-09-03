//a1696375

// convert text into XML
#include <iostream>
#include "tokensf.h"

// to make the code shorter
using namespace std ;

// tokensier to use
static tokensf *tokeniser ;

// next token
static string next_token()
{
	return tokeniser->next_token() ;
}

// token value
static string token_value()
{
	return tokeniser->token_value() ;
}

// the function using the tokeniser is part of the supplied library
void text_to_tokens_in_xml(string(*)(),string(*)()) ;

// read each token from standard input and output it in XML format
int main()
{
	// create a new tokeniser to read standard input using getchar
	tokeniser = new tokensf(getchar) ;

	// call our display program to read the tokens and display as xml
	// we pass it the next_token and token_value functions it must use
	text_to_tokens_in_xml(next_token,token_value) ;

	return 0 ;
}
