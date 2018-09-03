#ifndef JACK_SYMBOLS_H
#define JACK_SYMBOLS_H

#include <string>

// the symbol tables below can be used to store and lookup values associated with a given symbol
// insert records a new association between a symbol and a value but, only if the symbol is not already in the table
// lookup returns the value associated with a symbol or an error value if it is not in the table

// symbol int pair used by symbols_int
struct string_int { std::string symbol ; int value ; } ;

class symbols_int        // string symbol to integer value lookup table
{
public:
        symbols_int() ;						// create a new empty symbol table
        bool insert(std::string symbol,int value) ;		// returns false for duplicate symbol
        int lookup(std::string symbol) ;			// returns -1 for undefined symbol
private:
       int nsymbols ;
       int size ;
       string_int *symbols ;

} ;

// symbol string pair used by symbols_string
struct string_string { std::string symbol ; std::string value ; } ;

class symbols_string     // string symbol to string value lookup table
{
public:
        symbols_string() ;					// create a new empty symbol table
        bool insert(std::string symbol,std::string value) ;	// returns false for duplicate symbol
        std::string lookup(std::string symbol) ;		// returns "" for undefined symbol
private:
       int nsymbols ;
       int size ;
       string_string *symbols ;
} ;

class jack_var		// class for a jack variable's info in a symbol table
{
public:
	jack_var() ;						// default constructor
								// option to supply all four values on creation
	jack_var(std::string symbol,std::string kind,std::string segment,int offset) ;
	std::string symbol ;					// variable's name
	std::string kind ;					// variable's class
	std::string segment ;					// segment where variable is stored
	int offset ;						// variable's offset in its segment
} ;

class symbols_var	// string symbol to jack_var lookup table
{
public:
        symbols_var() ;						// create a new empty symbol table
        bool insert(std::string symbol,jack_var value) ;	// returns false for duplicate symbol
        jack_var lookup(std::string symbol) ;			// returns var with name "" for undefined symbol
private:
       int nsymbols ;
       int size ;
       jack_var *symbols ;
} ;

#endif //JACK_SYMBOLS_H
