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
	expect(ID);
	expect(LBRACE);
	Token t = peek();
	if (t.token_type == PUBLIC)
	{
		parse_public_vars();
		Token t2 = peek();
		if (t2.token_type == PRIVATE)
		{
			parse_private_vars();
			parse_stmt_list();
			expect(RBRACE);
		}
		else if (t2.token_type == ID) //private went to epsilon
		{
			parse_stmt_list();
			expect(RBRACE);
		}
		else
		{
			syntax_error();
		}
	}
	else if (t.token_type == PRIVATE) //public went to epsilon
	{
		parse_private_vars();
		parse_stmt_list();
		expect(RBRACE);
	}
	else if (t.token_type == ID) //public and private went to epsilon
	{
		parse_stmt_list();
		expect(RBRACE);
	}
	else
	{
		syntax_error();
	}
}//end of parse_scope

void Parser::parse_public_vars()
{
	//public_vars --> epsilon
	//public_vars --> PUBLIC COLON var_list SEMICOLON
	Token t = peek();
	if (t.token_type == PUBLIC) //public_vars --> PUBLIC COLON var_list SEMICOLON
	{
		expect(PUBLIC);
		expect(COLON);
		parse_var_list();
		expect(SEMICOLON);
	}
	else if (t.token_type == PRIVATE || t.token_type == ID) //public_vars --> epsilon
	{
		//done
	}
	else
	{
		syntax_error();
	}
}//end of parse_public_vars

void Parser::parse_private_vars()
{
	//private_vars --> epsilon
	//private_vars --> PRIVATE COLON var_list SEMICOLON
	Token t = peek();
	if (t.token_type == PRIVATE) //private_vars --> PRIVATE COLON var_list SEMICOLON
	{
		expect(PRIVATE);
		expect(COLON);
		parse_var_list();
		expect(SEMICOLON);
	}
	else if (t.token_type == ID) //private_vars --> epsilon
	{
		//done
	}
	else
	{
		syntax_error();
	}
}//end of parse_private_vars

void Parser::parse_stmt_list()
{
	//stmt_list --> stmt
	//stmt_list --> stmt stmt_list
	parse_stmt();
	Token t = peek();
	if (t.token_type == RBRACE) //stmt_list --> stmt
	{
		//done
	}
	else if (t.token_type == ID) //stmt_list --> stmt stmt_list
	{
		parse_stmt_list();
	}
	else
	{
		syntax_error();
	}
}//end of parse_stmt_list

void Parser::parse_stmt()
{
	//stmt --> ID EQUAL ID SEMICOLON
	//stmt --> scope
	Token t = lexer.GetToken();
	if (t.token_type == ID)
	{
		token t2 = peek();
		if (t2.token_type == EQUAL) //stmt --> ID EQUAL ID SEMICOLON
		{
			expect(EQUAL);
			expect(ID);
			expect(SEMICOLON);
		}
		else if (t2.token_type == LBRACE) //stmt --> scope
		{
			lexer.UngetToken(t);
			parse_scope();
		}
		else
		{
			syntax_error();
		}
	}
	else //FIRST(scope) == ID
	{
		syntax_error();
	}
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