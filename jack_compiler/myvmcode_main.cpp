// a1696375
#include <iostream>
#include <stdlib.h>
#include <string>
#include "jacktokens.h"
#include "jacksymbols.h"
#include "jackxml.h"

//Create token variables.
static jacktokens *tokeniser = NULL;
static std::string token = "?";
static std::string tokenclass = "?";
static std::string tokenvalue = "?";
//Used to store the previous token if we dont know what to do with it yet.
std::string temp;
bool useTemp = 0;
//Used to determine the starting index of the arguments.
int argOffset;
//Create Symbol Tables.
symbols_var *funcTable;
symbols_var classTable;
//Variable to store temporary variable data.
jack_var jv;
//Variable Counts.
int staticCount = 0;
int localCount = 0;
int thisCount = 0;
int argumentCount = 0;

// vars for outputting vm code
std::string funcName;
std::string className;
int ifCount = 0;
int whileCount = 0;

//Get the next token and update appropriate variables.
void nextToken(){
	token = tokeniser->next_token();
	tokenclass = tokeniser->token_class();
	tokenvalue = tokeniser->token_value();
}
//If the token is not what is expected then erase the xml buffer and quit, otherwise move on.
void mustbe(std::string expected){
	if (token != expected){
		// jackxml::flush_output();
		jackxml::erase_buffer();
		// std::cout << "Error: expected \"" << expected << "\" but found \"" << token << "\"" << std::endl;
		exit(0);
	}
	nextToken();
}
// If we have the expected token, get the next token and return true, if not return false.
bool have(std::string expected){
	if (token != expected){
		return false;
	}
	nextToken();
	return true;
}
//Predeclaring all parse statements for recursion:
//classes
void parseProgram();
void parseClass();
void parseClassVarDec();
void parseType();
void parseSubroutineDec();
void parseParameterList();
void parseSubroutineBody();
void parseVarDec();
void parseClassName();
void parseSubroutineName();
void parseVarName();
//statements
void parseStatements();
void parseStatement();
void parseLetStatement();
void parseIfStatement();
void parseWhileStatement();
void parseDoStatement();
void parseReturnStatement();
//expressions
void parseExpression();
void parseTerm();
void parseExpressionList();
void parseOp();
void parseUnaryOp();
void parseKeywordConstant();

//Parse Program
void parseProgram(){
	parseClass();
}
//Parse Class
void parseClass(){


	mustbe("class");

	parseClassName();


	mustbe("{");

	while(token == "constructor" || token == "function" || token == "method" ||
	token == "static" || token == "field"){
		if(token == "constructor" || token == "function" || token == "method"){
			parseSubroutineDec();
		}
		if(token == "static" || token == "field"){
			parseClassVarDec();
		}
	}


	mustbe("}");

}
//Parse ClassVarDec
void parseClassVarDec(){

	if( token == "static" || token == "field"){
		if(token == "static"){
			jv.segment = "static";
		} else {
			jv.segment = "this";
		}

		nextToken();
	}
	parseType();
	parseVarName();
	while(token == ","){

		nextToken();
		parseVarName();
	}

	mustbe(";");
	jv.segment = "local";

}
//Parse Type
void parseType(){

	if(token == "int" || token == "char" || token == "boolean"){
		jv.kind = tokenvalue;

		nextToken();
	} else {
		jv.kind = tokenvalue;
		parseClassName();
	}

}
//Parse SubroutineDec
void parseSubroutineDec(){

	funcTable = new symbols_var;
	bool isConstructor = 0;
	if(token == "constructor"){
		isConstructor = 1;
	}
	if(!(have("constructor") || have("function"))){
		mustbe("method");
		argOffset = 1;
	} else {
		argOffset = 0;
	}

	if(token == "void"){

		nextToken();
	} else if (isConstructor){
		isConstructor = 0;
		nextToken();
	} else {
		parseType();
	}

	parseSubroutineName();


	mustbe("(");

	parseParameterList();


	mustbe(")");

	parseSubroutineBody();

	delete funcTable;
	argumentCount = 0;
	localCount = 0;
}
//Parse Parameter List
void parseParameterList(){
	if(token == "int" || token == "char" || token == "boolean" || token == "identifier"){
		jv.segment = "argument";
		parseType();
		parseVarName();
		while(token == ","){

			nextToken();
			parseType();
			parseVarName();
		}
	}
}
//Parse SubroutineBody
void parseSubroutineBody(){


	mustbe("{");

	while(token == "var"){
		parseVarDec();
	}
	jackxml::buffer_line("function " + className + "." + funcName + " " + std::to_string(argumentCount + localCount));
	parseStatements();


	mustbe("}");

}
//Parse VarDec
void parseVarDec(){

	jv.segment = "local";

	mustbe("var");
	parseType();
	parseVarName();
	while(token == ","){

		nextToken();
		parseVarName();
	}

	mustbe(";");

}
//Parse ClassName
void parseClassName(){
	className = tokenvalue;
	mustbe("identifier");
}
//Parse SubroutineName
void parseSubroutineName(){
	funcName = tokenvalue;
	mustbe("identifier");
}
//Parse VarName
void parseVarName(){

	std::string tokenToUse;
	if(useTemp){
		tokenToUse = temp;
		useTemp = 0;
	} else {
		tokenToUse = tokenvalue;
		mustbe("identifier");
	}

	jv.symbol = tokenToUse;

	if(jv.segment == "static"){
		jv.offset = staticCount;
		if(classTable.insert(tokenToUse, jv) != 0){
			// std::cout << "Var Added: " << tokenToUse << " : " << jv.symbol << " : "  << jv.kind << " : "  << jv.segment << " : "  << jv.offset << std::endl;
			staticCount++;
		}
		jv = classTable.lookup(tokenToUse);

	} else if(jv.segment == "this"){
		jv.offset = thisCount;
		if(classTable.insert(tokenToUse, jv) != 0){
			// std::cout << "Var Added: " << tokenToUse << " : " << jv.symbol << " : "  << jv.kind << " : "  << jv.segment << " : "  << jv.offset << std::endl;
			thisCount++;
		}
		jv = classTable.lookup(tokenToUse);

	} else if(jv.segment == "local"){
		jv.offset = localCount;
		if(classTable.lookup(tokenToUse).symbol == ""){
			if(funcTable->insert(tokenToUse, jv) != 0){
				// std::cout << "Var Added: " << tokenToUse << " : " << jv.symbol << " : "  << jv.kind << " : "  << jv.segment << " : "  << jv.offset << std::endl;
				localCount++;
			}
			jv = funcTable->lookup(tokenToUse);
		} else {
			jv = classTable.lookup(tokenToUse);
		}

	} else if(jv.segment == "argument"){
		jv.offset = argumentCount + argOffset;
		if(classTable.lookup(tokenToUse).symbol == ""){
			if(funcTable->insert(tokenToUse, jv) != 0){
				// std::cout << "Var Added: " << tokenToUse << " : " << jv.symbol << " : "  << jv.kind << " : "  << jv.segment << " : "  << jv.offset << std::endl;
				argumentCount++;
			}
			jv = funcTable->lookup(tokenToUse);
		} else {
			jv = classTable.lookup(tokenToUse);
		}
	}
}
//Parse Statements
void parseStatements(){
	while(token == "let" || token == "if" || token == "while" || token == "do" || token == "return"){
		parseStatement();
	}
}
//Parse Statement
void parseStatement(){
	if(token == "let"){
		parseLetStatement();
	} else if(token == "if"){
		parseIfStatement();
	} else if(token == "while"){
		parseWhileStatement();
	} else if(token == "do"){
		parseDoStatement();
	} else {
		parseReturnStatement();
	}
}
//Parse LetStatement
void parseLetStatement(){
    mustbe("let");
	if(funcTable->lookup(tokenvalue).symbol != ""){
		jv = funcTable->lookup(tokenvalue);
	} else if(classTable.lookup(tokenvalue).symbol != ""){
		jv = classTable.lookup(tokenvalue);
	} else {
		//std::cout << "it aint here" << std::endl;
	}
	parseVarName();
	if(token == "["){
		nextToken();
		parseExpression();
		mustbe("]");
	}
    mustbe("=");
	parseExpression();
    mustbe(";");
}
//Parse IfStatement
void parseIfStatement(){
	mustbe("if");
	mustbe("(");
	parseExpression();
	mustbe(")");
	std::string n = std::to_string(ifCount++);
	jackxml::buffer_line("if-goto IF_TRUE" + n);
	jackxml::buffer_line("goto IF_FALSE" + n);
	jackxml::buffer_line("label IF_TRUE" + n);
	mustbe("{");
	parseStatements();
	mustbe("}");
	if(n != "0"){
		jackxml::buffer_line("goto IF_END" + n);
	}
	jackxml::buffer_line("label IF_FALSE" + n);
	if(token == "else"){
		nextToken();
		mustbe("{");
		parseStatements();
		mustbe("}");
	}
	if(n != "0"){
		jackxml::buffer_line("label IF_END" + n);
	}
}
//Parse WhileStatement
void parseWhileStatement(){
	std::string n = std::to_string(whileCount++);
	jackxml::buffer_line("label WHILE_EXP" + n);
	mustbe("while");
	mustbe("(");
	parseExpression();
	mustbe(")");
	jackxml::buffer_line("not");
	jackxml::buffer_line("if-goto WHILE_END" + n);
	mustbe("{");
	parseStatements();
	mustbe("}");
	jackxml::buffer_line("goto WHILE_EXP" + n);
	jackxml::buffer_line("label WHILE_END" + n);
}
//Parse DoStatement
void parseDoStatement(){
	mustbe("do");
	useTemp = 1;
	temp = tokenvalue;
	nextToken();
	if(token == "."){
		if(funcTable->lookup(temp).symbol != ""){
			jv = funcTable->lookup(temp);
			parseVarName();
		} else if(classTable.lookup(temp).symbol != ""){
			jv = classTable.lookup(temp);
			parseVarName();
		} else {
			useTemp = 0;
		}
		nextToken();

		parseSubroutineName();


		mustbe("(");

	} else if(token == "("){

		useTemp = 0;


		nextToken();
	}
	parseExpressionList();
	mustbe(")");
	mustbe(";");


}
//Parse Return Statement
void parseReturnStatement(){
	mustbe("return");

	if(token == ";"){
		jackxml::buffer_line("push constant 0");
		jackxml::buffer_line("return");
	}
	while(token != ";"){
		parseExpression();
	}
	mustbe(";");
}
//Parse Expression
void parseExpression(){
	parseTerm();
	//while we have a symbol that isn't one of these, it must be an operator
	while(tokenclass == "symbol" && token != ")" && token != "]" && token != ";" && token != ","){
		
		parseOp();
		parseTerm();
	}
}
//Parse Term
void parseTerm(){

	if(token == "integerConstant" || token == "stringConstant"){
		jackxml::buffer_line("push constant" + tokenvalue);
		nextToken();
	} else if(tokenclass == "keyword"){
		if(token == "true" || token == "false" || token == "null" || token == "this"){
			parseKeywordConstant();
		}
	} else if(tokenclass == "identifier"){
		//Because we dont know if the identifier is a classname, subroutine name
		//or varname, we are going to store the token and not create an xml node
		//until we know what it is by reading the next token.
		temp = tokenvalue;
		useTemp = 1;
		nextToken();
		if(token == "["){
			parseVarName();
			nextToken();
			parseExpression();
			mustbe("]");
		} else if(token == "."){

			if(funcTable->lookup(temp).symbol != ""){
				jv = funcTable->lookup(temp);
				parseVarName();
			} else if(classTable.lookup(temp).symbol != ""){
				jv = classTable.lookup(temp);
				parseVarName();
			} else {
				useTemp = 0;
			}
			nextToken();
			parseSubroutineName();
			mustbe("(");
			parseExpressionList();
			mustbe(")");
		} else if(token == "("){
			useTemp = 0;


			nextToken();

			parseExpressionList();


			mustbe(")");
		} else {
			if(funcTable->lookup(temp).symbol != ""){
				jv = funcTable->lookup(temp);
				jackxml::buffer_line("push " + jv.segment + " " + std::to_string(jv.offset));
				parseVarName();
			} else if(classTable.lookup(temp).symbol != ""){
				jv = classTable.lookup(temp);
				jackxml::buffer_line("push " + jv.segment + " " + std::to_string(jv.offset));
				parseVarName();
			}
		}
	} else if(tokenclass == "symbol"){
		if(token == "("){

			nextToken();

			parseExpression();


			mustbe(")");
		} else if(token == "-" || token == "~"){
			parseUnaryOp();
			parseTerm();
		}
	}

}
//Parse ExpressionList
void parseExpressionList(){

	if(token != ")"){
		parseExpression();
		jv.segment = "argument";
		while(token == ","){

			nextToken();

			parseExpression();
		}
	}

}
//Parse Op
void parseOp(){
	if(token == "+" || token == "-" || token == "*" || token == "/" ||
	token == "&" || token == "|" || token == "<" || token == ">" || token == "="){
		nextToken();
	}
}
//Parse UnaryOp
void parseUnaryOp(){
	if(token == "-" || token == "~"){

		nextToken();
	}
}
//Parse KeywordConstant
void parseKeywordConstant(){
	if(token == "true" || token == "false" || token == "null" || token == "this"){
		if(token == "true"){
			jackxml::buffer_line("push constant 0");
			jackxml::buffer_line("not");
		} else if(token == "false"){
			jackxml::buffer_line("push constant 0");
		}
		nextToken();
	}
}

int main(){
	// create a tokeniser and read first token to initialise it
	tokeniser = new jacktokens();
	nextToken();
	//parse the program
	parseProgram();
	//check for the end of file
	mustbe("?");
	//output xml
	jackxml::flush_output();

	return 0;
}
