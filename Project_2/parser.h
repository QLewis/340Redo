#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <stdio.h>
#include "lexer.h"
#include "SymbolTable.h"

class Parser
{
	private:
		LexicalAnalyzer lexer;

		void syntax_error();
		Token expect(TokenType expected_type);
		Token peek();
		SymbolTable symTab;

	public:
		void ParseInput();

		void parse_program();
		void parse_global_vars();
		char* parse_var_list();
		void parse_scope();
		void parse_public_vars();
		void parse_private_vars();
		void parse_stmt_list();
		void parse_stmt();

}; //end of Parser class


#endif
