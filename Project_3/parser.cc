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
		//ID COMMA var_list
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
	//INT | REAL | BOOL
	Token t = lexer.GetToken();

	if (t.token_type == INT || t.token_type == REAL || t.token_type == BOOL)
	{
		//done
	}
	else
	{
		syntax_error();
	}
}//end of parse_type_name

void Parser::parse_body()
{
	//LBRACE stmt_list RBRACE
	expect(LBRACE);
	parse_stmt_list();
	expect(RBRACE);
}//end of parse_body

void Parser::parse_stmt_list()
{
	//stmt
	//stmt stmt_list

	parse_stmt();

	Token t = peek();

	if (t.token_type == RBRACE)
	{
		//stmt
	}
	else if (t.token_type == ID || t.token_type == IF || t.token_type == WHILE || t.token_type == SWITCH)
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
	//assignment_stmt
	//if_stmt
	//while_stmt
	//switch_stmt
	Token t = lexer.peek();
	if (t.token_type == ID)
	{
		parse_assignment_stmt();
	}
	else if (t.token_type == IF)
	{
		parse_if_stmt();
	}
	else if (t.token_type == WHILE)
	{
		parse_while_stmt();
	}
	else if (t.token_type == SWITCH)
	{
		parse_switch_stmt();
	}
	else
	{
		syntax_error();
	}
}//end of parse_stmt

void Parser::parse_assignment_stmt()
{
	expect(ID);
	expect(EQUAL);
	parse_expression();
	expect(SEMICOLON);
}//end of parse_assignment_stmt

void Parser::parse_expression()
{
	//primary
	//binary_operator expression expression
	//unary_operator expression expression

	Token t = peek();

	if (t.token_type == ID || t.token_type == NUM || t.token_type == REALNUM || t.token_type == TRUE || t.token_type == FALSE)
	{
		parse_primary();
	}
	else if (t.token_type == PLUS || t.token_type == MINUS || t.token_type == MULT || t.token_type == DIV)
	{
		parse_binary_operator();
		parse_expression();
		parse_expression();
	}
	else if (t.token_type == GREATER || t.token_type == LESS || t.token_type == GTEQ || t.token_type == LTEQ || t.token_type == EQUAL || t.token_type == NOTEQUAL)
	{
		parse_binary_operator();
		parse_expression();
		parse_expression();
	}
	else if (t.token_type == NOT)
	{
		parse_unary_operator();
		parse_expression();
		parse_expression();
	}
	else
	{
		syntax_error();
	}
}//end of parse_expression

void Parser::parse_unary_operator()
{
	expect(NOT);
}//end of parse_unary_operator

void Parser::parse_binary_operator()
{
	Token t = lexer.GetToken();

	if (t.token_type == PLUS || t.token_type == MINUS || t.token_type == MULT || t.token_type == DIV)
	{
		//done
	}
	else if (t.token_type == GREATER || t.token_type == LESS || t.token_type == GTEQ || t.token_type == LTEQ || t.token_type == EQUAL || t.token_type == NOTEQUAL)
	{
		//done
	}
	else
	{
		syntax_error();
	}
	
}//end of parse_binary_operator

void Parser::parse_primary()
{
	Token t = lexer.GetToken();

	if (t.token_type == ID || t.token_type == NUM || t.token_type == REALNUM || t.token_type == TRUE || t.token_type == FALSE)
	{
		//done
	}
}//end of parse_primary

void Parser::parse_if_stmt()
{
	expect(IF);
	expect(LPAREN);
	parse_expression();
	expect(RPAREN);
	parse_body();
}//end of parse_if_stmt

void Parser::parse_while_stmt()
{
	expect(WHILE);
	expect(LPAREN);
	parse_expression();
	expect(RPAREN);
	parse_body();
}//end of parse_while_stmt

void Parser::parse_switch_stmt()
{
	expect(SWITCH);
	expect(LPAREN);
	parse_expression();
	expect(RPAREN);
	expect(LBRACE);
	parse_case_list();
	expect(RBRACE);
}//end of parse_switch_stmt

void Parser::parse_case_list()
{
	parse_case();

	Token t = peek();

	if (t.token_type == CASE)
	{
		parse_case_list();
	}
	else if (t.token_type == RBRACE)
	{
		//done
	}
	else
	{
		syntax_error();
	}
}//end of parse_case_list

void Parser::parse_case()
{

}//end of parse_case