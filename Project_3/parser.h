#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <stdio.h>
#include "lexer.h"


class Parser
{
	private:
		LexicalAnalyzer lexer;
		void syntax_error();
		Token expect(TokenType expected_type);
		Token peek();
	public:
		void parse_program();
		void parse_global_vars();
		void parse_var_decl_list();
		void parse_var_decl();
		void parse_var_list();
		void parse_type_name();
		void parse_body();
		void parse_stmt_list();
		void parse_stmt();
		void parse_assignment_stmt();
		void parse_expression();
		void parse_unary_operator();
		void parse_binary_operator();
		void parse_primary();
		void parse_if_stmt();
		void parse_while_stmt();
		void parse_switch_stmt();
		void parse_case_list();
		void parse_case();
};
#endif