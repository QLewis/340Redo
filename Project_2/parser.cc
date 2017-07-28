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
	cout << "parser.cc -- INSIDE PARSE_GLOBAL_VARS\n" << endl;
	cout << "parse_global_vars -- calling GetToken() now\n" << endl;

	Token t = lexer.GetToken();

	if (t.token_type != ID)
	{
		cout << "parse_global_vars -- t.token_type != ID\n" << endl;
		syntax_error();
	}
	else
	{
		cout << "parse_global_vars -- t.token_type is ID\n" << endl;
		cout << "parse_global_vars -- t.lexeme is " << t.lexeme << endl << endl;

		cout << "parse_global_vars -- calling peek for Token t2\n" << endl;
		Token t2 = peek();
		cout << "parse_global_vars -- t2.lexeme is " << t2.lexeme << endl << endl;

		if (t2.token_type == COMMA) //var_list SEMICOLON
		{
			cout << "parse_global_vars -- t2.token_type == COMMA\n" << endl;

			symTab.currentScope = "::";
			symTab.currentPermission = 1;

			lexer.UngetToken(t);

			cout << "parse_global_vars -- calling parse_var_list\n" << endl;
			parse_var_list();
			cout << "parse_global_vars -- done with parse_var_list\n" << endl;
			expect(SEMICOLON);
			cout << "parser.cc -- DONE WITH PARSE_GLOBAL_VARS\n" << endl;
		}
		else if (t2.token_type == LBRACE) //epsilon, the ID was from scope
		{
			cout << " parser.cc -- parse_global_vars -- t2.token_type == LBRACE\n" << endl;
			lexer.UngetToken(t);
		}
		else
		{
			cout << "parse_global_vars -- t2.token_type != COMMA nor LBRACE\n" << endl << endl;
			syntax_error();
		}
	}
}//end of parse_global_vars()


void Parser::parse_var_list() //DONE
{
	//var_list --> ID
	//var_list --> ID COMMA var_list
	cout << "parser.cc -- INSIDE PARSE_VAR_LIST\n" << endl;
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
			cout << "parse_var_list -- just added item to symbol table, about to add another\n" << endl;
			expect(COMMA);
			parse_var_list();
		}
		else if (t2.token_type == SEMICOLON) //ID
		{
			//done
			symTab.addItem(t.lexeme, symTab.currentScope, symTab.currentPermission);
			cout << "parser.cc -- just added item to symbol table, and DONE WITH PARSE_VAR_LIST\n" << endl;
		}
		else
		{
			syntax_error();
		}
	}
}//end of parse_var_list

void Parser::parse_scope() //TODO: double check parse_stmt_list, pop symbole table
{
	//scope --> ID LBRACE public_vars private_vars stmt_list RBRACE
	cout << "parser.cc -- INSIDE PARSE_SCOPE\n" << endl;
	cout << "parse_scope -- calling GetToken() now\n" << endl;

	Token t = lexer.GetToken();

	if (t.token_type != ID)
	{
		cout << "parse_scope -- t.token_type != ID\n" << endl;
		syntax_error();
	}
	else
	{
		cout << "parse_scope -- t.token_type == ID and t.lexeme is " << t.lexeme << endl << endl;
		symTab.currentScope = t.lexeme;

		cout << "parse_scope -- expecting LBRACE right now\n" << endl;
		expect(LBRACE);

		Token t2 = peek();
		cout << "parse_scope -- t2.token_type is " << t2.token_type << " and t2.lexeme is " << t2.lexeme << endl << endl;

		if (t2.token_type == PUBLIC)
		{
			cout << "parse_scope -- calling parse_public_vars\n" << endl;
			parse_public_vars();

			Token t3 = peek();
			if (t3.token_type == PRIVATE)
			{
				cout << "parse_scope -- calling parse_private_vars\n" << endl;
				parse_private_vars();
				parse_stmt_list();
				expect(RBRACE);
				cout << "parse_scope -- public and private -- DONE WITH PARSE_SCOPE\n" << endl;
			}
			else if (t3.token_type == ID) //private went to epsilon
			{
				parse_stmt_list();
				expect(RBRACE);
				cout << "parse_scope -- public only -- DONE WITH PARSE_SCOPE\n" << endl;
			}
			else
			{
				syntax_error();
			}
		}
		else if (t2.token_type == PRIVATE) //public went to epsilon
		{
			cout << "parse_scope -- public went to epsilon, calling parse_private_vars\n" << endl;
			parse_private_vars();
			parse_stmt_list();
			expect(RBRACE);
			cout << "parse_scope -- private only -- DONE WITH PARSE_SCOPE\n" << endl;
		}
		else if (t2.token_type == ID) //public and private went to epsilon
		{
			cout << "parse_scope -- neither public nor private -- calling parse_stmt_list\n" << endl;
			parse_stmt_list();
			expect(RBRACE);
			//TODO: pop everything from currentScope out of the symbol table
			cout << "parse_scope -- end of parse_scope, popping the current scope out of the symbole table\n" << endl;
			symTab.removeScope(currentScope);
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
	cout << "parser.cc -- INSIDE PARSE_PUBLIC_VARS\n" << endl;
	Token t = peek();
	if (t.token_type == PUBLIC)
	{
		expect(PUBLIC);
		expect(COLON);

		symTab.currentPermission = 1;
		parse_var_list();

		expect(SEMICOLON);
		cout << "parse_public_vars -- LEAVING PARSE_PUBLIC_VARS\n" << endl;
	}
	else if (t.token_type == PRIVATE || t.token_type == ID)
	{
		//public_vars --> epsilon
		cout << "parse_public_vars went to epsilon -- LEAVING PARSE_PUBLIC_VARS\n" << endl;
	}
	else
	{
		cout << "parse_public_vars -- neither PUBLIC nor EPSILON -- LEAVING PARSE_PUBLIC_VARS\n" << endl;
		syntax_error();
	}
}//end of parse_public_vars

void Parser::parse_private_vars() //DONE
{
	//private_vars --> epsilon
	//private_vars --> PRIVATE COLON var_list SEMICOLON
	cout << "parser.cc -- INSIDE PARSE_PRIVATE_VARS\n" << endl;

	Token t = peek();
	if (t.token_type == PRIVATE) // PRIVATE COLON var_list SEMICOLON
	{
		expect(PRIVATE);
		expect(COLON);

		symTab.currentPermission = 0;
		cout << "parse_private_vars -- calling parse_var_list\n" << endl;
		parse_var_list();
			
		expect(SEMICOLON);
		cout << "parse_private_vars -- LEAVING PARSE_PRIVATE_VARS\n" << endl;
	}
	else if (t.token_type == ID)
	{
		//private_vars --> epsilon
		cout << "parse_private_vars -- went to epsilon -- LEAVING PARSE_PRIVATE_VARS\n" << endl;
	}
	else
	{
		cout << "parse_private_vars -- t.token_type not ID nor PRIVATE\n" << endl;
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
		Token t2 = peek();
		if (t2.token_type == EQUAL) //stmt --> ID EQUAL ID SEMICOLON
		{
			symbolTableItem* id1 = symTab.searchItem(t.lexeme); //find first variable in the symbol table

			expect(EQUAL);

			Token t3 = lexer.GetToken();
			if (t3.token_type != ID)
			{
				syntax_error();
			}
			else
			{
				symbolTableItem* id2 = symTab.searchItem(t3.lexeme); //find second variable in the symbol table

				//matching scopes
				if (id1->scope == id2->scope)
				{
					//printf(id1->scope + "." + id1->name + "=" + id2->scope + "." + id2->name);
					cout << id1->scope << "." << id1->name << "=" << id2->scope << "." << id2->name << endl;
				}
				else
				{
					//check if first ID is public or private
					string id1Str = "";
					string id2Str = "";

					if (id1->permission ==  1) //public
					{
						id1Str += id1->scope;
						id1Str += ".";
						id1Str += id1->name;
					}
					else //private
					{
						id1Str += "?";
						id1Str += ".";
						id1Str += id1->name;
					}

					//check if second id is public or private
					if (id2->permission ==  1) //public
					{
						id2Str += id1->scope;
						id2Str += ".";
						id2Str += id1->name;
					}
					else //private
					{
						id2Str += "?";
						id2Str += ".";
						id2Str += id1->name;
					}

					//printf(id1Str + "=" + id2Str);
					cout << id1Str << "=" << id2Str << endl;
				}
			}
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