/*
 * Copyright (C) Rida Bazzi, 2016
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
    "PUBLIC", "PRIVATE",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRACE", "ID", "ERROR"// DONE: Add labels for new token types here (as string)
};

#define KEYWORDS_COUNT 2
string keyword[] = {
    "public", "private"
};

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << /*this->line_no <<*/ "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    //this->line_no = 1;
    tmp.lexeme = "";
    //tmp.line_no = 1;
    tmp.token_type = ERROR;
}

//TODO: Add SkipComment functionality
bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    //line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        //line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::SkipComment()
{
    cerr << "lexer.cc -- INSIDE SKIPCOMMENT\n" << endl;
    char c;
    bool comment_encountered = false;

    input.GetChar(c);
    cerr << "SkipComment -- char c is " << c << endl << endl;
    //line_no += (c == '\n');

    if (c == '/')
    {
        input.GetChar(c);
        //line_no += (c == '\n');
        if (c == '/')
        {
            cerr << "SkipComment has found a comment\n" << endl;
            while (!input.EndOfInput() && (c != '\n'))
            {
                comment_encountered = true;
                input.GetChar(c);
                //line_no += (c == '\n');
            }
            if (isspace(c))
            {
                SkipSpace();
            }
            SkipComment();
        }
    }
    else
    {
        input.UngetChar(c);
    }

    /*if (!input.EndOfInput())
    {
        cerr << "SkipComment -- not at end of input, and done with comment\n" << endl;
        input.UngetChar(c);
    }*/
    return comment_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++)
    {
        if (s == keyword[i])
        {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++)
    {
        if (s == keyword[i])
        {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

//DONE: Check for Public and Private here
Token LexicalAnalyzer::ScanIdOrKeyword()
{
   char c;
   
   input.GetChar(c);

   if (isalpha(c))
   {
        tmp.lexeme = "";
        while (!input.EndOfInput() && /*isalnum(c)*/ (isalpha(c) || isdigit(c)))
        {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput())
        {
            input.UngetChar(c);
        }
        //This is where the program should check for public and private
        if (IsKeyword(tmp.lexeme))
        {
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        }
        else
        {
            tmp.token_type = ID;
        }
   }
   else
   {
        if (!input.EndOfInput())
        {
            input.UngetChar(c);
        }
        tmp.token_type = ERROR;
   }
   return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    cerr << "INSIDE GET TOKEN\n" << endl;
    char c;
    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        cerr << "GetToken -- !tokens.empty() function -- returning " << tmp.lexeme << endl << endl;
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }
    cerr << "lexer.cc -- GetToken -- calling SkipSpace\n" << endl;
    SkipSpace();
    cerr << "lexer.cc -- GetToken -- calling SkipComment\n" << endl;
    SkipComment();

    tmp.lexeme = "";
    //tmp.line_no = line_no;
    input.GetChar(c);
    cerr << "GetToken -- char c is " << c << endl << endl;
    switch (c) {
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '{':
            tmp.token_type = LBRACE;
            return tmp;
        case '}':
            tmp.token_type = RBRACE;
            return tmp;
        default:
            if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}

/*int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
}*/
