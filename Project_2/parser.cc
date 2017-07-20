#include <iostream>
#include <cstdlib>
#include "parser.h"

using namespace std;

void Parser::syntax_error()
{
	cout << "Syntax Error\n";
	exit(1);

}

Token Parser::expect(TokenType expected_type)
{
	Token t = lexer.GetToken();
	if (t.token_type != expected_type)
	{
		syntax_error();
	}
	return t;
}

Token Parser::peek()
{
	Token t = lexer.GetToken();
	lexer.UngetToken(t);
	return t;
}

//TODO: START PARSING HERE
void Parser::parse_program()
{
	//program --> global_vars scope
	parse_global_vars();
	parse_scope();
}//end of parse_program

void Parser::parse_global_vars();
{
	//global_vars --> epsilon
	//global_vars --> var_list SEMICOLON
	Token t = lexer.GetToken();
	if (t.token_type == ID)
	{
		Token t2 = peek();
		if (t2.token_type == COMMA) //var_list SEMICOLON
		{
			lexer.UngetToken(t);
			parse_var_list();
			expect(SEMICOLON);
		}
		else if (t2.token_type == LBRACE) //epsilon
		{
			lexer.UngetToken(t);
		}
		else
		{
			syntax_error();
		}
	}
	else
	{
		syntax_error();
	}
}//end of parse_global_vars()

void Parser::parse_var_list()
{
	//var_list --> ID
	//var_list --> ID COMMA var_list
	expect(ID);
	Token t = peek();
	if (t.token_type == COMMA) //ID COMMA var_list
	{
		expect(COMMA);
		parse_var_list();
	}
	else if (t.token_type == SEMICOLON) //ID
	{
		//done
	}
	else
	{
		syntax_error();
	}
}//end of parse_var_list

void Parser::parse_scope()
{
	//scope --> ID LBRACE public_vars private_vars stmt_list RBRACE
}//end of parse_scope

void Parser::parse_public_vars()
{
	//public_vars --> epsilon
	//public_vars --> PUBLIC COLON var_list SEMICOLON
}//end of parse_public_vars

void Parser::parse_private_vars()
{
	//private_vars --> epsilon
	//private_vars --> PRIVATE COLON var_list SEMICOLON
}//end of parse_private_vars

void Parser::parse_stmt_list()
{
	//stmt_list --> stmt
	//stmt_list --> stmt stmt_list
}//end of parse_stmt_list

void Parser::parse_stmt()
{
	//stmt --> ID EQUAL ID SEMICOLON
	//stmt --> scope
}

//END OF PARSING
void Parser::ParseInput()
{
	parse_program();
	expect(END_OF_FILE);
}

int main()
{
	Parser parser;

	parser.ParseInput();

}