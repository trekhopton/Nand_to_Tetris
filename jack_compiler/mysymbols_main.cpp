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
//Opens, sets text, and closes an xml node with current token info.
void createTokenNode(){
	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
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
	jackxml::open_node("class");
	createTokenNode();
	mustbe("class");

	parseClassName();

	createTokenNode();
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

	createTokenNode();
	mustbe("}");
	jackxml::close_node("class");
}
//Parse ClassVarDec
void parseClassVarDec(){
	jackxml::open_node("classVarDec");
	if( token == "static" || token == "field"){
		if(token == "static"){
			jv.segment = "static";
		} else {
			jv.segment = "this";
		}
		createTokenNode();
		nextToken();
	}
	parseType();
	parseVarName();
	while(token == ","){
		createTokenNode();
		nextToken();
		parseVarName();
	}
	createTokenNode();
	mustbe(";");
	jv.segment = "local";
	jackxml::close_node("classVarDec");
}
//Parse Type
void parseType(){
	jackxml::open_node("type");
	if(token == "int" || token == "char" || token == "boolean"){
		jv.kind = tokenvalue;
		createTokenNode();
		nextToken();
	} else {
		jv.kind = tokenvalue;
		parseClassName();
	}
	jackxml::close_node("type");
}
//Parse SubroutineDec
void parseSubroutineDec(){

	jackxml::open_node("subroutineDec");
	funcTable = new symbols_var;

	createTokenNode();
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
		createTokenNode();
		nextToken();
	} else if (isConstructor){
		isConstructor = 0;
		nextToken();
	} else {
		parseType();
	}

	parseSubroutineName();

	createTokenNode();
	mustbe("(");

	parseParameterList();

	createTokenNode();
	mustbe(")");

	parseSubroutineBody();

	delete funcTable;
	argumentCount = 0;
	localCount = 0;
	jackxml::close_node("subroutineDec");
}
//Parse Parameter List
void parseParameterList(){
	jackxml::open_node("parameterList");
	if(token == "int" || token == "char" || token == "boolean" || token == "identifier"){
		jv.segment = "argument";
		parseType();
		parseVarName();
		while(token == ","){
			createTokenNode();
			nextToken();
			parseType();
			parseVarName();
		}
	}
	jackxml::close_node("parameterList");
}
//Parse SubroutineBody
void parseSubroutineBody(){
	jackxml::open_node("subroutineBody");
	createTokenNode();
	mustbe("{");

	while(token == "var"){
		parseVarDec();
	}
	parseStatements();

	createTokenNode();
	mustbe("}");
	jackxml::close_node("subroutineBody");
}
//Parse VarDec
void parseVarDec(){
	jackxml::open_node("varDec");
	jv.segment = "local";
	createTokenNode();
	mustbe("var");
	parseType();
	parseVarName();
	while(token == ","){
		createTokenNode();
		nextToken();
		parseVarName();
	}
	createTokenNode();
	mustbe(";");
	jackxml::close_node("varDec");
}
//Parse ClassName
void parseClassName(){
	jackxml::open_node("className");
	createTokenNode();
	mustbe("identifier");
	jackxml::close_node("className");
}
//Parse SubroutineName
void parseSubroutineName(){
	jackxml::open_node("subroutineName");
	createTokenNode();
	mustbe("identifier");
	jackxml::close_node("subroutineName");
}
//Parse VarName
void parseVarName(){
	jackxml::open_node("varName");
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

	jackxml::open_node("identifier");
	jackxml::node_text(jv.symbol + "," + jv.kind + "," + jv.segment +  "," + std::to_string(jv.offset));
	jackxml::close_node("identifier");

	jackxml::close_node("varName");
}
//Parse Statements
void parseStatements(){
	jackxml::open_node("statements");
	while(token == "let" || token == "if" || token == "while" || token == "do" || token == "return"){
		parseStatement();
	}
	jackxml::close_node("statements");
}
//Parse Statement
void parseStatement(){
	jackxml::open_node("statement");
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
	jackxml::close_node("statement");
}
//Parse LetStatement
void parseLetStatement(){
	jackxml::open_node("letStatement");
    createTokenNode();
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
		createTokenNode();
		nextToken();

		parseExpression();

		createTokenNode();
		mustbe("]");
	}

	createTokenNode();
    mustbe("=");

	parseExpression();

	createTokenNode();
    mustbe(";");

    jackxml::close_node("letStatement");
}
//Parse IfStatement
void parseIfStatement(){
	jackxml::open_node("ifStatement");
	createTokenNode();
	mustbe("if");

	createTokenNode();
	mustbe("(");

	parseExpression();

	createTokenNode();
	mustbe(")");

	createTokenNode();
	mustbe("{");

	parseStatements();

	createTokenNode();
	mustbe("}");

	if(token == "else"){
		createTokenNode();
		nextToken();

		createTokenNode();
		mustbe("{");

		parseStatements();

		createTokenNode();
		mustbe("}");
	}

	jackxml::close_node("ifStatement");
}
//Parse WhileStatement
void parseWhileStatement(){
	jackxml::open_node("whileStatement");
	createTokenNode();
	mustbe("while");

	createTokenNode();
	mustbe("(");

	parseExpression();

	createTokenNode();
	mustbe(")");

	createTokenNode();
	mustbe("{");

	parseStatements();

	createTokenNode();
	mustbe("}");

	jackxml::close_node("whileStatement");

}
//Parse DoStatement
void parseDoStatement(){
	//'do' ((className | varName) '.')? subroutineName '(' expressionList ')' ';'
	jackxml::open_node("doStatement");
	createTokenNode();
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
			jackxml::open_node("className");
			jackxml::open_node("identifier");
			jackxml::node_text(temp);
			jackxml::close_node("identifier");
			jackxml::close_node("className");
			useTemp = 0;
		}

		createTokenNode();
		nextToken();

		parseSubroutineName();

		createTokenNode();
		mustbe("(");

	} else if(token == "("){
		jackxml::open_node("subroutineName");
		jackxml::open_node("identifier");
		jackxml::node_text(temp);
		jackxml::close_node("identifier");
		jackxml::close_node("subroutineName");
		useTemp = 0;

		createTokenNode();
		nextToken();
	}

	parseExpressionList();

	createTokenNode();
	mustbe(")");

	createTokenNode();
	mustbe(";");

	jackxml::close_node("doStatement");
}
//Parse Return Statement
void parseReturnStatement(){
	jackxml::open_node("returnStatement");
	createTokenNode();
	mustbe("return");

	while(token != ";"){
		parseExpression();
	}

	createTokenNode();
	mustbe(";");
	jackxml::close_node("returnStatement");
}
//Parse Expression
void parseExpression(){
	jackxml::open_node("expression");
	parseTerm();
	//while we have a symbol that isn't one of these, it must be an operator
	while(tokenclass == "symbol" && token != ")" && token != "]" && token != ";" && token != ","){
		parseOp();
		parseTerm();
	}
	jackxml::close_node("expression");
}
//Parse Term
void parseTerm(){
	jackxml::open_node("term");
	if(token == "integerConstant" || token == "stringConstant"){
		createTokenNode();
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

			createTokenNode();
			nextToken();

			parseExpression();

			createTokenNode();
			mustbe("]");
		} else if(token == "."){

			if(funcTable->lookup(temp).symbol != ""){
				jv = funcTable->lookup(temp);
				parseVarName();
			} else if(classTable.lookup(temp).symbol != ""){
				jv = classTable.lookup(temp);
				parseVarName();
			} else {
				jackxml::open_node("className");
				jackxml::open_node("identifier");
				jackxml::node_text(temp);
				jackxml::close_node("identifier");
				jackxml::close_node("className");
				useTemp = 0;
			}



			createTokenNode();
			nextToken();

			parseSubroutineName();

			createTokenNode();
			mustbe("(");

			parseExpressionList();

			createTokenNode();
			mustbe(")");

		} else if(token == "("){
			jackxml::open_node("subroutineName");
			jackxml::open_node("identifier");
			jackxml::node_text(temp);
			jackxml::close_node("identifier");
			jackxml::close_node("subroutineName");
			useTemp = 0;

			createTokenNode();
			nextToken();

			parseExpressionList();

			createTokenNode();
			mustbe(")");
		} else {
			if(funcTable->lookup(temp).symbol != ""){
				jv = funcTable->lookup(temp);
				parseVarName();
			} else if(classTable.lookup(temp).symbol != ""){
				jv = classTable.lookup(temp);
				parseVarName();
			}
		}
	} else if(tokenclass == "symbol"){
		if(token == "("){
			createTokenNode();
			nextToken();

			parseExpression();

			createTokenNode();
			mustbe(")");
		} else if(token == "-" || token == "~"){
			parseUnaryOp();
			parseTerm();
		}
	}
	jackxml::close_node("term");
}
//Parse ExpressionList
void parseExpressionList(){
	jackxml::open_node("expressionList");
	if(token != ")"){
		parseExpression();
		jv.segment = "argument";
		while(token == ","){
			createTokenNode();
			nextToken();

			parseExpression();
		}
	}
	jackxml::close_node("expressionList");
}
//Parse Op
void parseOp(){
	if(token == "+" || token == "-" || token == "*" || token == "/" ||
	token == "&" || token == "|" || token == "<" || token == ">" || token == "="){
		createTokenNode();
		nextToken();
	}
}
//Parse UnaryOp
void parseUnaryOp(){
	if(token == "-" || token == "~"){
		createTokenNode();
		nextToken();
	}
}
//Parse KeywordConstant
void parseKeywordConstant(){
	if(token == "true" || token == "false" || token == "null" || token == "this"){
		createTokenNode();
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
