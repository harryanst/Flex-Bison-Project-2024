%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
%}

%option noyywrap

%{
int line_number = 1;      // Variable to store line number
%}     



%%

[\n]    { line_number++; }   // Update line number on newline

"int" { printf("%s", yytext); return INT; }
"double" { printf("%s", yytext); return DOUBLE; }
"char" { printf("%s", yytext); return CHAR; }
"boolean" { printf("%s", yytext); return BOOLEAN; }
"String" { printf("%s", yytext); return STRING; }
"public class"  { printf("%s", yytext); return PUBLIC_CLASS; }
"private" { printf("%s", yytext); return PRIVATE; }
"protected" { printf("%s", yytext); return PROTECTED; }
"public" { printf("%s", yytext); return PUBLIC; }
"Private" { printf("%s", yytext); return M_PRIVATE; }
"Public" { printf("%s", yytext); return M_PUBLIC; }
"void" { printf("%s", yytext); return VOID; }
"return" { printf("%s", yytext); return RETURN; }
"true" { printf("%s", yytext); return TRUE; }
"false" { printf("%s", yytext); return FALSE; }
"do" { printf("%s", yytext); return DO; }
"while" { printf("%s", yytext); return WHILE; }
"for" { printf("%s", yytext); return FOR; }
"switch" { printf("%s", yytext); return SWITCH; }
"case" { printf("%s", yytext); return CASE; }
"if" { printf("%s", yytext); return IF; }
"else if" { printf("%s", yytext); return ELSE_IF; }
"else" { printf("%s", yytext); return ELSE; }
"break" { printf("%s", yytext); return BREAK; }
":" { printf("%s", yytext); return DOUBLE_DOT; }
"default:" { printf("%s", yytext); return DEFAULT; }
"out\.print" { printf("%s", yytext); return PRINT; }
"==" { printf("%s", yytext); return CHECK_EQUAL; }
"=" { printf("%s", yytext); return EQUAL; }
"new" { printf("%s", yytext); return NEW; }
"(" { printf("%s", yytext); return LEFT_BRACKET; }
")" { printf("%s", yytext); return RIGHT_BRACKET; }
"&&" { printf("%s", yytext); return AND; }
"||" { printf("%s", yytext); return OR; }
"<" { printf("%s", yytext); return LESS; }
">" { printf("%s", yytext); return GREATER; }
"!=" { printf("%s", yytext); return NOT_EQUAL; }
"/" { printf("%s", yytext); return DIV; }
"\*" { printf("%s", yytext); return MUL; }
"+" { printf("%s", yytext); return PLUS; }
"-" { printf("%s", yytext); return MINUS; }
"\." { printf("%s", yytext); return DOT; }
\'[^\']*\' { printf("%s", yytext); return CHAR_VAR; }
\"[^\"]*\" { printf("%s", yytext); return STRING_VAR; }
[A-Z_][a-zA-Z0-9_]*   { printf("%s", yytext); return CLASS_NAME; }
[a-zA-Z_][a-zA-Z0-9_]*   { printf("%s", yytext); return VAR; }
[0-9]+             { printf("%s", yytext); return INT_NUM;}
[0-9]+"."[0-9]+"d" { printf("%s", yytext); return DOUBLE_NUM; }
"{"   { printf("%s", yytext); return LEFT_BRACE; }
","   { printf("%s", yytext); return COMMA; }
"}"   { printf("%s\n", yytext); return RIGHT_BRACE; }
";"   { printf("%s\n", yytext); return QMARK; }
"\/\/".*    { printf("%s\n", yytext); return SINGLE_COMMENT; }
"\/\*"[^\*]*\*+([^\*\/][^\*]*\*+)*"\/"    { printf("%s\n", yytext); return MULTILINE_COMMENT; }
.    { printf("%s", yytext); }


%%
