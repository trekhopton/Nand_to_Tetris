// a1696375
#include <iostream>
#include <stdlib.h>
#include "jacktokens.h"
#include "jackxml.h"

//Create token variables.
static jacktokens *tokeniser = NULL;
static std::string token = "?";
static std::string tokenclass = "?";
static std::string tokenvalue = "?";
//Get the next token and update appropriate variables.
void nextToken(){
	token = tokeniser->next_token();
	tokenclass = tokeniser->token_class();
	tokenvalue = tokeniser->token_value();
}
//If the token is not what is expected then erase the xml buffer and quit, otherwise move on
void mustbe(std::string expected){
	if (token != expected){
		// jackxml::flush_output();
		jackxml::erase_buffer();
		//std::cout << "Error: expected \"" << expected << "\" but found \"" << token << "\"" << std::endl;
		exit(0);
	}
	nextToken();
}
// if we have the expected token, get the next token and return true, if not return false
bool have(std::string expected){
	if (token != expected){
		return false;
	}
	nextToken();
	return true;
}
//predeclaring all parse statements fro recursion
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
	jackxml::open_node(tokenclass); //keyword
	jackxml::node_text(tokenvalue); //class
	jackxml::close_node(tokenclass); //keyword
	mustbe("class");

	parseClassName();

	jackxml::open_node(tokenclass); //symbol
	jackxml::node_text(tokenvalue); //{
	jackxml::close_node(tokenclass); //symbol
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

	jackxml::open_node(tokenclass); //symbol
	jackxml::node_text(tokenvalue); //}
	jackxml::close_node(tokenclass); //symbol
	mustbe("}");
	jackxml::close_node("class");
}
//Parse ClassVarDec
void parseClassVarDec(){
	jackxml::open_node("classVarDec");
	if( token == "static" || token == "field"){
		jackxml::open_node(tokenclass);
		jackxml::node_text(tokenvalue);
		jackxml::close_node(tokenclass);
		nextToken();
	}
	parseType();
	parseVarName();
	while(token == ","){
		jackxml::open_node(tokenclass);
		jackxml::node_text(tokenvalue);
		jackxml::close_node(tokenclass);
		nextToken();
		parseVarName();
	}
	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe(";");
	jackxml::close_node("classVarDec");
}
//Parse Type
void parseType(){
	jackxml::open_node("type");
	if(token == "int" || token == "char" || token == "boolean"){
		jackxml::open_node(tokenclass);
		jackxml::node_text(tokenvalue);
		jackxml::close_node(tokenclass);
		nextToken();
	} else {
		parseClassName();
	}
	jackxml::close_node("type");
}
//Parse SubroutineDec
void parseSubroutineDec(){

	jackxml::open_node("subroutineDec");
	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	if(!(have("constructor") || have("function"))){
		mustbe("method");
	}

	if(token == "void"){
		jackxml::open_node(tokenclass);
		jackxml::node_text(tokenvalue);
		jackxml::close_node(tokenclass);
		nextToken();
	} else {
		parseType();
	}

	parseSubroutineName();

	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("(");

	parseParameterList();

	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe(")");

	parseSubroutineBody();
	jackxml::close_node("subroutineDec");
}
//Parse Parameter List
void parseParameterList(){
	jackxml::open_node("parameterList");
	if(token == "int" || token == "char" || token == "boolean" || token == "identifier"){
		parseType();
		parseVarName();
		while(token == ","){
			jackxml::open_node(tokenclass);
			jackxml::node_text(tokenvalue);
			jackxml::close_node(tokenclass);
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
	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("{");

	while(token == "var"){
		parseVarDec();
	}
	parseStatements();

	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("}");
	jackxml::close_node("subroutineBody");
}
//Parse VarDec
void parseVarDec(){
	jackxml::open_node("varDec");
	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("var");
	parseType();
	parseVarName();
	while(token == ","){
		jackxml::open_node(tokenclass);
		jackxml::node_text(tokenvalue);
		jackxml::close_node(tokenclass);
		nextToken();
		parseVarName();
	}
	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe(";");
	jackxml::close_node("varDec");
}
//Parse ClassName
void parseClassName(){
	jackxml::open_node("className");
	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("identifier");
	jackxml::close_node("className");
}
//Parse SubroutineName
void parseSubroutineName(){
	jackxml::open_node("subroutineName");
	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("identifier");
	jackxml::close_node("subroutineName");
}
//Parse VarName
void parseVarName(){
	jackxml::open_node("varName");
	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("identifier");
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
    jackxml::open_node(tokenclass);
    jackxml::node_text(tokenvalue);
    jackxml::close_node(tokenclass);
    mustbe("let");

	parseVarName();

	if(token == "["){
		jackxml::open_node(tokenclass);
	    jackxml::node_text(tokenvalue);
	    jackxml::close_node(tokenclass);
		nextToken();

		parseExpression();

		jackxml::open_node(tokenclass);
	    jackxml::node_text(tokenvalue);
	    jackxml::close_node(tokenclass);
		mustbe("]");
	}

	jackxml::open_node(tokenclass);
    jackxml::node_text(tokenvalue);
    jackxml::close_node(tokenclass);
    mustbe("=");

	parseExpression();

	jackxml::open_node(tokenclass);
    jackxml::node_text(tokenvalue);
    jackxml::close_node(tokenclass);
    mustbe(";");

    jackxml::close_node("letStatement");
}
//Parse IfStatement
void parseIfStatement(){
	jackxml::open_node("ifStatement");
	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("if");

	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("(");

	parseExpression();

	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe(")");

	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("{");

	parseStatements();

	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("}");

	if(token == "else"){
		jackxml::open_node(tokenclass);
		jackxml::node_text(tokenvalue);
		jackxml::close_node(tokenclass);
		nextToken();

		jackxml::open_node(tokenclass);
		jackxml::node_text(tokenvalue);
		jackxml::close_node(tokenclass);
		mustbe("{");

		parseStatements();

		jackxml::open_node(tokenclass);
		jackxml::node_text(tokenvalue);
		jackxml::close_node(tokenclass);
		mustbe("}");
	}

	jackxml::close_node("ifStatement");
}
//Parse WhileStatement
void parseWhileStatement(){
	jackxml::open_node("whileStatement");
	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("while");

	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("(");

	parseExpression();

	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe(")");

	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("{");

	parseStatements();

	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("}");

	jackxml::close_node("whileStatement");

}
//Parse DoStatement
void parseDoStatement(){
	//'do' ((className | varName) '.')? subroutineName '(' expressionList ')' ';'
	jackxml::open_node("doStatement");
	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("do");

	std::string temp = tokenvalue;
	nextToken();
	if(token == "."){
		jackxml::open_node("className");
		jackxml::open_node("identifier");
		jackxml::node_text(temp);
		jackxml::close_node("identifier");
		jackxml::close_node("className");

		jackxml::open_node(tokenclass);
		jackxml::node_text(tokenvalue);
		jackxml::close_node(tokenclass);
		nextToken();

		parseSubroutineName();

		jackxml::open_node(tokenclass);
		jackxml::node_text(tokenvalue);
		jackxml::close_node(tokenclass);
		mustbe("(");

	} else if(token == "("){
		jackxml::open_node("subroutineName");
		jackxml::open_node("identifier");
		jackxml::node_text(temp);
		jackxml::close_node("identifier");
		jackxml::close_node("subroutineName");

		jackxml::open_node(tokenclass);
		jackxml::node_text(tokenvalue);
		jackxml::close_node(tokenclass);
		nextToken();
	}

	parseExpressionList();

	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe(")");

	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe(";");

	jackxml::close_node("doStatement");
}
//Parse Return Statement
void parseReturnStatement(){
	jackxml::open_node("returnStatement");
	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
	mustbe("return");

	while(token != ";"){
		parseExpression();
	}

	jackxml::open_node(tokenclass);
	jackxml::node_text(tokenvalue);
	jackxml::close_node(tokenclass);
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
		jackxml::open_node(tokenclass);
		jackxml::node_text(tokenvalue);
		jackxml::close_node(tokenclass);
		nextToken();
	} else if(tokenclass == "keyword"){
		if(token == "true" || token == "false" || token == "null" || token == "this"){
			parseKeywordConstant();
		}
	} else if(tokenclass == "identifier"){
		//Because we dont know if the identifier is a classname, subroutine name
		//or varname, we are going to store the token and not create an xml node
		//until we know what it is by reading the next token.
		std::string temp = tokenvalue;
		nextToken();
		if(token == "["){
			jackxml::open_node("varName");
			jackxml::open_node("identifier");
			jackxml::node_text(temp);
			jackxml::close_node("identifier");
			jackxml::close_node("varName");

			jackxml::open_node(tokenclass);
			jackxml::node_text(tokenvalue);
			jackxml::close_node(tokenclass);
			nextToken();

			parseExpression();

			jackxml::open_node(tokenclass);
			jackxml::node_text(tokenvalue);
			jackxml::close_node(tokenclass);
			mustbe("]");
		} else if(token == "."){
			//because we can't tell if it's a varname or class name, just
			//assume it's a class name
			jackxml::open_node("className");
			jackxml::open_node("identifier");
			jackxml::node_text(temp);
			jackxml::close_node("identifier");
			jackxml::close_node("className");

			jackxml::open_node(tokenclass);
			jackxml::node_text(tokenvalue);
			jackxml::close_node(tokenclass);
			nextToken();

			parseSubroutineName();

			jackxml::open_node(tokenclass);
			jackxml::node_text(tokenvalue);
			jackxml::close_node(tokenclass);
			mustbe("(");

			parseExpressionList();

			jackxml::open_node(tokenclass);
			jackxml::node_text(tokenvalue);
			jackxml::close_node(tokenclass);
			mustbe(")");

		} else if(token == "("){
			jackxml::open_node("subroutineName");
			jackxml::open_node("identifier");
			jackxml::node_text(temp);
			jackxml::close_node("identifier");
			jackxml::close_node("subroutineName");

			jackxml::open_node(tokenclass);
			jackxml::node_text(tokenvalue);
			jackxml::close_node(tokenclass);
			nextToken();

			parseExpressionList();

			jackxml::open_node(tokenclass);
			jackxml::node_text(tokenvalue);
			jackxml::close_node(tokenclass);
			mustbe(")");
		} else {
			jackxml::open_node("varName");
			jackxml::open_node("identifier");
			jackxml::node_text(temp);
			jackxml::close_node("identifier");
			jackxml::close_node("varName");
		}
	} else if(tokenclass == "symbol"){
		if(token == "("){
			jackxml::open_node(tokenclass);
			jackxml::node_text(tokenvalue);
			jackxml::close_node(tokenclass);
			nextToken();

			parseExpression();

			jackxml::open_node(tokenclass);
			jackxml::node_text(tokenvalue);
			jackxml::close_node(tokenclass);
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

		while(token == ","){
			jackxml::open_node(tokenclass);
			jackxml::node_text(tokenvalue);
			jackxml::close_node(tokenclass);
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
		jackxml::open_node(tokenclass);
		jackxml::node_text(tokenvalue);
		jackxml::close_node(tokenclass);
		nextToken();
	}
}
//Parse UnaryOp
void parseUnaryOp(){
	if(token == "-" || token == "~"){
		jackxml::open_node(tokenclass);
		jackxml::node_text(tokenvalue);
		jackxml::close_node(tokenclass);
		nextToken();
	}
}
//Parse KeywordConstant
void parseKeywordConstant(){
	if(token == "true" || token == "false" || token == "null" || token == "this"){
		jackxml::open_node(tokenclass);
		jackxml::node_text(tokenvalue);
		jackxml::close_node(tokenclass);
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
