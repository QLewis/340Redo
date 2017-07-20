#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"

class Parser
{
	private:
		LexicalAnalyzer lexer;

		void syntax_error();
		Token expect(TokenType expected_type);
		Token peek();

		void parse_program();
		void parse_global_vars();
		void parse_var_list();
		void parse_scope();
		void parse_public_vars();
		void parse_private_vars();
		void parse_stmt_list();
		void parse_stmt();
	public:
		void ParseInput();

}; //end of Parser class


#endif
