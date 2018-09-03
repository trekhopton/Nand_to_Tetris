//a1696375
// convert Hack assembly into binary
#include <iostream>
#include <bitset>
#include "tokens.h"
#include "myTable.h"

// to simplify the code
using namespace std;

//takes an integer number and returns a string containing the 15 bit binary representation of that number
string decimalToBinary(int number){

	string binary = bitset<15>(number).to_string();

	return binary;
}

// assume that standard input is the contents of a Hack assembly file
// read each token and output its details
int main()
{
	//create the tables for dest, comp and jump and fill them with hard coded key and value pairs
	table compTable;
	table destTable;
	table jumpTable;

	compTable.insert("0", "0101010");
	compTable.insert("1", "0111111");
	compTable.insert("-1", "0111010");
	compTable.insert("D", "0001100");
	compTable.insert("A", "0110000");
	compTable.insert("!D", "0001101");
	compTable.insert("!A", "0110001");
	compTable.insert("-D", "0001111");
	compTable.insert("-A", "0110011");
	compTable.insert("D+1", "0011111");
	compTable.insert("A+1", "0110111");
	compTable.insert("D-1", "0001110");
	compTable.insert("A-1", "0110010");
	compTable.insert("D+A", "0000010");
	compTable.insert("D-A", "0010011");
	compTable.insert("A-D", "0000111");
	compTable.insert("D&A", "0000000");
	compTable.insert("D|A", "0010101");

	compTable.insert("M", "1110000"); //19 to 28
	compTable.insert("!M", "1110001");
	compTable.insert("-M", "1110011");
	compTable.insert("M+1", "1110111");
	compTable.insert("M-1", "1110010");
	compTable.insert("D+M", "1000010");
	compTable.insert("D-M", "1010011");
	compTable.insert("M-D", "1000111");
	compTable.insert("D&M", "1000000");
	compTable.insert("D|M", "1010101");

	destTable.insert("null", "000");
	destTable.insert("M", "001");
	destTable.insert("D", "010");
	destTable.insert("MD", "011");
	destTable.insert("A", "100");
	destTable.insert("AM", "101");
	destTable.insert("AD", "110");
	destTable.insert("AMD", "111");

	jumpTable.insert("null", "000");
	jumpTable.insert("JGT", "001");
	jumpTable.insert("JEQ", "010");
	jumpTable.insert("JGE", "011");
	jumpTable.insert("JLT", "100");
	jumpTable.insert("JNE", "101");
	jumpTable.insert("JLE", "110");
	jumpTable.insert("JMP", "111");

	// create a new assembler tokeniser then read the first token
	tokens *tokeniser = new tokens(getchar);
	string token = tokeniser->next_token();

	// a token counters
	int tokens = 0;

	string label;

	while ( token != "?" ){

		string aluop = "null";
		string destination = "null";
		string ajump = "null";
		bool valid = 1;

		//check for A-instruction
		if(token == "address"){
			cout << "0" << decimalToBinary(stoi(tokeniser->token_value())) << endl;
			token = tokeniser->next_token();
		//check for C-instruction
		} else {
			if(token == "null" || token == "dest"){
				destination = tokeniser->token_value();
			} else if(token == "dest-comp?"){
				destination = tokeniser->token_value();
				aluop = tokeniser->token_value();
			} else if (token == "comp"){
				aluop = tokeniser->token_value();
			} else {
				//error invalid token
				valid = 0;
			}
			//if everything so far is valid get the next token
			if(valid){
				token = tokeniser->next_token();
				//next token should be = or ;
				if(token == "equals"){
					//if token is equals then the previous token was a destination so set the aluop back to null
					aluop = "null";
					token = tokeniser->next_token();
					// this token should be an aluop, if it is, get the next token to check for jump
					if(token == "dest-comp?" || token == "comp"){
						aluop = tokeniser->token_value();
						token = tokeniser->next_token();
						//this should be true if a jump is to follow
						if(token == "semi"){
							token = tokeniser->next_token();
							if(token == "jump"){
								ajump = tokeniser->token_value();
								token = tokeniser->next_token();
							}
						}
					} else {
						destination = "null";
						aluop = "null";
						ajump = "null";
					}
				} else if(token == "semi"){
					//if the token is a semi then the previous token was a aluop so set the destination to back to null
					destination = "null";
					token = tokeniser->next_token();
					if(token == "jump"){
						ajump = tokeniser->token_value();
						token = tokeniser->next_token();
					}
				} else {
					destination = "null";
					ajump = "null";
				}
			} else { //it's not a valid command so get the next token
				token = tokeniser->next_token();
			}

			if(aluop != "null"){
				cout << "111" << compTable.lookup(aluop) << destTable.lookup(destination) << jumpTable.lookup(ajump) << endl;
			}
		}
	}

	return 0;
}
