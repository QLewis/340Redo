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

//DONE: START PARSING HERE
void Parser::parse_program()
{
	//program --> global_vars scope
	parse_global_vars();
	parse_scope();
}//end of parse_program

void Parser::parse_global_vars() //DONE
{
	//global_vars --> epsilon
	//global_vars -->  var_list SEMICOLON
	cerr << " parser.cc -- INSIDE PARSE_GLOBAL_VARS\n" << endl;
	Token t = lexer.GetToken();

	if (t.token_type != ID)
	{
		syntax_error();
	}
	else
	{

		Token t2 = peek();

		if (t2.token_type == COMMA) //var_list SEMICOLON
		{
			cerr << "parse_global_vars -- t2.token_type is COMMA and about to call parse_var_list\n" << endl;


			symTab.currentScope = "::";
			symTab.currentPermission = 1;

			lexer.UngetToken(t);

			parse_var_list();

			cerr << "parse_global_vars -- done with parse_var_list\n" << endl;
			expect(SEMICOLON);
			cerr << "LEAVING PARSE_GLOBAL_VARS\n" << endl;
		}
		else if (t2.token_type == SEMICOLON) //varList of one variable
		{
			cerr << "parse_global_vars -- t2.token_type == SEMICOLON and about to call parse_var_list\n" << endl;
			symTab.currentScope = "::";
			symTab.currentPermission = 1;

			lexer.UngetToken(t);
			parse_var_list();
			expect(SEMICOLON);
			cerr << "LEAVING PARSE_GLOBAL_VARS\n" << endl;
		}
		else if (t2.token_type == LBRACE) //epsilon, the ID was from scope
		{
			lexer.UngetToken(t);
		}
		else
		{
			cerr << "parse_global_vars -- t2.token_type != COMMA or LBRACE\n" << endl;
			syntax_error();
		}
	}
}//end of parse_global_vars()


void Parser::parse_var_list() //DONE
{
	//var_list --> ID
	//var_list --> ID COMMA var_list
	cerr << "parser.cc -- INSIDE PARSE_VAR_LIST\n" << endl;

	Token t = lexer.GetToken();

	if (t.token_type != ID)
	{
		syntax_error();
	}
	else
	{
		Token t2 = peek();

		if (t2.token_type == COMMA) //ID COMMA var_list
		{
			symTab.addItem(t.lexeme, symTab.currentScope, symTab.currentPermission);
			cerr << "parse_var_list just added " << t.lexeme << ", " << symTab.currentScope << ", " << symTab.currentPermission << " to the symbol table, adding another variable\n" << endl;

			expect(COMMA);
			parse_var_list();
		}
		else if (t2.token_type == SEMICOLON) //ID
		{
			//done
			symTab.addItem(t.lexeme, symTab.currentScope, symTab.currentPermission);
			cerr << "parse_var_list just added " << t.lexeme << ", " << symTab.currentScope << ", " << symTab.currentPermission << " to the symbol table, LEAVING PARSE_VAR_LIST\n" << endl;
		}
		else
		{
			syntax_error();
		}
	}
}//end of parse_var_list

void Parser::parse_scope() //TODO: double check parse_stmt_list, pop symbol table
{
	//scope --> ID LBRACE public_vars private_vars stmt_list RBRACE

	Token t = lexer.GetToken();

	if (t.token_type != ID)
	{
		syntax_error();
	}
	else
	{
		symTab.currentScope = t.lexeme;

		expect(LBRACE);

		Token t2 = peek();

		if (t2.token_type == PUBLIC)
		{
			parse_public_vars();

			Token t3 = peek();
			if (t3.token_type == PRIVATE)
			{
				parse_private_vars();
				parse_stmt_list();
				expect(RBRACE);
			}
			else if (t3.token_type == ID) //private went to epsilon
			{
				parse_stmt_list();
				expect(RBRACE);
			}
			else
			{
				syntax_error();
			}
		}
		else if (t2.token_type == PRIVATE) //public went to epsilon
		{
			parse_private_vars();
			parse_stmt_list();
			expect(RBRACE);
		}
		else if (t2.token_type == ID) //public and private went to epsilon
		{
			parse_stmt_list();
			expect(RBRACE);
			//TODO: pop everything from currentScope out of the symbol table
			symTab.removeScope(symTab.currentScope);
			cerr << "parse_scope -- leaving scope " << symTab.currentScope << endl << endl;
		}
		else
		{
			syntax_error();
		}
	}
}//end of parse_scope

void Parser::parse_public_vars() //DONE
{
	//public_vars --> epsilon
	//public_vars --> PUBLIC COLON var_list SEMICOLON
	Token t = peek();
	if (t.token_type == PUBLIC)
	{
		expect(PUBLIC);
		expect(COLON);

		symTab.currentPermission = 1;
		parse_var_list();

		expect(SEMICOLON);
	}
	else if (t.token_type == PRIVATE || t.token_type == ID)
	{
		//public_vars --> epsilon
	}
	else
	{
		syntax_error();
	}
}//end of parse_public_vars

void Parser::parse_private_vars() //DONE
{
	//private_vars --> epsilon
	//private_vars --> PRIVATE COLON var_list SEMICOLON
	Token t = peek();
	if (t.token_type == PRIVATE) // PRIVATE COLON var_list SEMICOLON
	{
		expect(PRIVATE);
		expect(COLON);

		symTab.currentPermission = 0;
		parse_var_list();
			
		expect(SEMICOLON);
	}
	else if (t.token_type == ID)
	{
		//private_vars --> epsilon
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
	cerr << "INSIDE PARSE_STMT_LIST\n" << endl;

	cerr << "parse_stmt_list -- calling parse_stmt\n" << endl;

	parse_stmt();
	Token t = peek();
	if (t.token_type == RBRACE) //stmt_list --> stmt
	{
		cerr << "parse_stmt_list -- only one stmt\n" << endl;
		//done
	}
	else if (t.token_type == ID) //stmt_list --> stmt stmt_list
	{
		cerr << "parse_stmt_list -- more than one stmt\n" << endl;
		parse_stmt_list();
	}
	else
	{
		cerr << "parse_stmt_list -- t.lexeme (" << t.lexeme << ") is not an ID or an RBRACE\n" << endl;
		syntax_error();
	}
}//end of parse_stmt_list

void Parser::parse_stmt()
{
	//stmt --> ID EQUAL ID SEMICOLON
	//stmt --> scope
	cerr << "INSIDE PARSE_STMT\n" << endl;

	Token t = lexer.GetToken();
	if (t.token_type == ID)
	{
		Token t2 = peek();
		if (t2.token_type == EQUAL) //stmt --> ID EQUAL ID SEMICOLON
		{
			cerr << "parse_stmt -- calling searchItem on " << t.lexeme << endl << endl;

			string leftID = symTab.searchItem(t.lexeme); //get the first variable

			cerr << "parse_stmt -- finished calling searchItem on " << t.lexeme <<  endl << endl;


			expect(EQUAL);

			Token t3 = lexer.GetToken();

			if (t3.token_type != ID)
			{
				syntax_error();
			}
			else
			{
				cerr << "parse_stmt -- calling searchItem on " << t3.lexeme << endl << endl;

				string rightID = symTab.searchItem(t3.lexeme); //get the second variable

				cerr << "parse_stmt -- finished calling searchItem on " << t3.lexeme << endl << endl;

				cout << leftID << t.lexeme << " = " << rightID << t3.lexeme << endl;

				expect(SEMICOLON);
			}
		}
		else if (t2.token_type == LBRACE) //stmt --> scope
		{
			cerr << "parse_stmt calling parse_scope\n" << endl;
			lexer.UngetToken(t);
			parse_scope();
		}
		else
		{
			cerr << "parse_stmt -- t2.lexeme (" << t2.lexeme << ") is not ID OR LBRACE\n" << endl;
			syntax_error();
		}
	}
	else //FIRST(scope) == ID
	{
		cerr << "parse_stmt -- t.lexeme (" << t.lexeme << ") is not an ID\n" << endl;
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