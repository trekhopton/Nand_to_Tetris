//a1696375
// convert Hack assembly into binary
#include <iostream>
#include <bitset>
#include <vector>
#include "tokens.h"
#include "myTable.h"

// to simplify the code
using namespace std;

//Takes a number and returns a string containing the 15 bit binary representation of that number.
string decimalToBinary(int number){

	string binary = bitset<15>(number).to_string();

	return binary;
}

// Assume that standard input is the contents of a Hack assembly file
// Read each token and output its details
int main()
{
	//create the tables for dest, comp and jump and fill them with hard coded key and value pairs.
	table compTable;
	table destTable;
	table jumpTable;

	compTable.insert("0", "0101010"); //1 to 18 the 'a bit' is 0
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

	compTable.insert("M", "1110000"); //19 to 28 the 'a bit' is 1
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

	//create symbol table and predefined symbols
	table symbolTable;

	symbolTable.insert("R0", "0");
	symbolTable.insert("R1", "1");
	symbolTable.insert("R2", "2");
	symbolTable.insert("R3", "3");
	symbolTable.insert("R4", "4");
	symbolTable.insert("R5", "5");
	symbolTable.insert("R6", "6");
	symbolTable.insert("R7", "7");
	symbolTable.insert("R8", "8");
	symbolTable.insert("R9", "9");
	symbolTable.insert("R10", "10");
	symbolTable.insert("R11", "11");
	symbolTable.insert("R12", "12");
	symbolTable.insert("R13", "13");
	symbolTable.insert("R14", "14");
	symbolTable.insert("R15", "15");

	symbolTable.insert("SP", "0");
	symbolTable.insert("LCL", "1");
	symbolTable.insert("ARG", "2");
	symbolTable.insert("THIS", "3");
	symbolTable.insert("THAT", "4");

	symbolTable.insert("SCREEN", "16384");
	symbolTable.insert("KBD", "24576");

	//Create a new assembler tokeniser then read the first token.
	tokens *tokeniser = new tokens(getchar);
	string token = tokeniser->next_token();

	//To store and count the lines.
	int lineCount = 0;
	vector<string> lines;

	int error = 0;

	//First pass.
	while ( token != "?" ){

		string aluop = "null";
		string destination = "null";
		string ajump = "null";

		//Check for labels.
		if(token == "label"){
			symbolTable.insert(tokeniser->token_value(), to_string(lineCount));
			token = tokeniser->next_token();
		} else if(token == "address"){ //Check for A-instruction.
			if(isdigit(tokeniser->token_value()[0])){
				lines.push_back("0" + decimalToBinary(stoi(tokeniser->token_value())));
			} else {
				lines.push_back(tokeniser->token_value());
			}
			token = tokeniser->next_token();
			lineCount++;
		} else { //Check for C-instruction.
			// These are the only ways a C-instruction can start.
			if(token == "null" || token == "dest"){
				destination = tokeniser->token_value();
			} else if(token == "dest-comp?"){
				destination = tokeniser->token_value();
				aluop = tokeniser->token_value();
			} else if (token == "comp"){
				aluop = tokeniser->token_value();
			} else {
				//Error invalid token.
				error = 1;
			}

			token = tokeniser->next_token();
			//The next token should be '=' or ';'.
			if(token == "equals"){
				//If token is equals then the previous token was a
				//destination so set the aluop back to null.
				aluop = "null";
				token = tokeniser->next_token();
				// This token should be an aluop, if it is, get the next token to check for jump.
				if(token == "dest-comp?" || token == "comp"){
					aluop = tokeniser->token_value();
					token = tokeniser->next_token();
					//This should be true if a jump is to follow.
					if(token == "semi"){
						token = tokeniser->next_token();
						if(token == "jump"){
							ajump = tokeniser->token_value();
							token = tokeniser->next_token();
						}
					}
				} else {
					//Error wrong token following equals.
					error = 2;
				}
			} else if(token == "semi"){
				//If the token is a semi then the previous token was a aluop so
				//set the destination to back to null.
				destination = "null";
				token = tokeniser->next_token();
				if(token == "jump"){
					ajump = tokeniser->token_value();
					token = tokeniser->next_token();
				}
			} else {
				destination = "null";
			}
			//Add the binary C-instruction to the lines vector and increment the lineCount.
			lines.push_back("111" + compTable.lookup(aluop) +
			destTable.lookup(destination) + jumpTable.lookup(ajump));
			lineCount++;
		}
	}
	//Keep track of where to store the variables.
	int varIndex = 15;
	//Second Pass. Loop throught the lines and output the binary.
	// If it's a symbol look it up and if it doesn't exist add it as a variable
	if(error == 0){
		for(int i = 0; i < lineCount; i++){
			if(isdigit(lines[i][0])){
				cout << lines[i] << endl;
			} else {
				if(!symbolTable.contains(lines[i])){
					symbolTable.insert(lines[i], to_string(++varIndex));
				}
				cout << "0" << decimalToBinary(stoi(symbolTable.lookup(lines[i]))) << endl;
			}
		}
	}

	return 0;
}
