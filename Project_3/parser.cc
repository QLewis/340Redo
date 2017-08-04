#include <iostream>
#include <cstdlib>

#include "parser.h"

using namespace std;

void Parser::syntax_error()
{
	cout << "Syntax_error\n";
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
	parse_global_vars();
	parse_body();
}//end of parse_program

void Parser::parse_global_vars()
{
	//global_vars --> epsilon
	//global_vars --> var_decl_list

	Token t = lexer.GetToken();

	if (t.token_type == LBRACE) //global_vars --> epsilon
	{
		lexer.UngetToken(t);
	}
	else if (t.token_type == ID) //global_vars --> var_decl_list
	{
		parse_var_decl_list();
	}
	else
	{
		syntax_error();
	}
}//end of parse_global_vars

void Parser::parse_var_decl_list()
{
	//var_decl_list --> var_decl
	//var_decl_list --> var_decl var_decl_list
	parse_var_decl();

	Token t = peek();
	if (t.token_type == LBRACE) //followed by parse_body
	{
		//done
	}
	else if (t.token_type == ID)
	{
		parse_var_decl_list();
	}
	else
	{
		syntax_error(0);
	}
}//end of parse_var_decl_list

void Parser::parse_var_decl()
{
	//var_decl --> var_list COLON type_name SEMICOLON
	parse_var_list();
	expect(COLON);
	parse_type_name();
	expect(SEMICOLON);
}//end of parse_var_decl

void Parser::parse_var_list()
{
	expect(ID);

	Token t = peek();
	if (t.token_type == COMMA)
	{
		expect(COMMA);
		parse_var_list();
	}
	else if (t.token_type == COLON)
	{
		//done
	}
	else
	{
		syntax_error();
	}
}//end of parse_var_list

void Parser::parse_type_name()
{

}//end of parse_type_name

void Parser::parse_body()
{

}//end of parse_body

void Parser::parse_stmt_list()
{

}//end of parse_stmt_list

void Parser::parse_stmt()
{

}//end of parse_stmt

void Parser::parse_assignment_stmt()
{

}//end of parse_assignment_stmt

void Parser::parse_expression()
{

}//end of parse_expression

void Parser::parse_unary_operator()
{

}//end of parse_unary_operator

void Parser::parse_binary_operator()
{

}//end of parse_binary_operator

void Parser::parse_primary()
{

}//end of parse_primary

void Parser::parse_if_stmt()
{

}//end of parse_if_stmt

void Parser::parse_while_stmt()
{

}//end of parse_while_stmt

void Parser::parse_switch_stmt()
{

}//end of parse_switch_stmt

void Parser::parse_case_list()
{

}//end of parse_case_list

void Parser::parse_case()
{

}//end of parse_case