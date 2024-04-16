%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern FILE* yyin; 
extern int line_number; 

void yyerror(const char *s) {
    fprintf(stderr, "Error at line %d: %s\n", line_number, s);
}

%}

%token PUBLIC_CLASS CLASS_NAME LEFT_BRACE RIGHT_BRACE PUBLIC PRIVATE QMARK VAR 
%token INT DOUBLE CHAR BOOLEAN STRING EQUAL NEW LEFT_BRACKET RIGHT_BRACKET VOID PROTECTED M_PUBLIC M_PRIVATE
%token COMMA RETURN INT_NUM DOUBLE_NUM CHAR_VAR STRING_VAR TRUE FALSE DOT
%token MUL DIV PLUS MINUS DO WHILE AND OR CHECK_EQUAL LESS GREATER NOT_EQUAL FOR SWITCH CASE 
%token DOUBLE_DOT DEFAULT BREAK PRINT IF ELSE ELSE_IF
%token SINGLE_COMMENT MULTILINE_COMMENT


%%

program: c public_class c
        | program c public_class c
        ;

public_class: PUBLIC_CLASS CLASS_NAME LEFT_BRACE class_block RIGHT_BRACE
                ;

class_block: variable_declarations method_declarations nested_class
		;

 
nested_class: c public_class c | ;


variable_declarations: variable_declaration | variable_declaration variable_declarations | ;


variable_declaration:  c int_decl c  
			|  c double_decl c 
			|  c char_decl c
			|  c string_decl c
			|  c boolean_decl c
			| c CLASS_NAME variable EQUAL object_instance QMARK c
	            ;

int_decl : variable_modifier INT variable more_variables QMARK 
	| variable_modifier INT variable EQUAL INT_NUM extra_int_variables QMARK
	| variable_modifier INT variable EQUAL operations QMARK 
	;


extra_int_variables: COMMA variable EQUAL INT_NUM 
	| COMMA variable EQUAL INT_NUM extra_int_variables
	| ;

double_decl : variable_modifier DOUBLE variable more_variables QMARK 
	| variable_modifier DOUBLE variable EQUAL DOUBLE_NUM extra_double_variables QMARK
	| variable_modifier DOUBLE variable EQUAL operations QMARK 
	;


extra_double_variables: COMMA variable EQUAL DOUBLE_NUM 
	| COMMA variable EQUAL DOUBLE_NUM extra_double_variables
	| ;

char_decl : variable_modifier CHAR variable more_variables QMARK 
	| variable_modifier CHAR variable EQUAL CHAR_VAR extra_char_variables QMARK
	;


extra_char_variables: COMMA variable EQUAL CHAR_VAR
	| COMMA variable EQUAL CHAR_VAR extra_char_variables
	| ;


string_decl : variable_modifier STRING variable more_variables QMARK 
	| variable_modifier STRING variable EQUAL STRING_VAR extra_string_variables QMARK
	;


extra_string_variables: COMMA variable EQUAL STRING_VAR
	| COMMA variable EQUAL STRING_VAR extra_string_variables
	| ;


boolean_decl : variable_modifier BOOLEAN variable more_variables QMARK 
	| variable_modifier BOOLEAN variable EQUAL boolean_var extra_boolean_variables QMARK
	;


extra_boolean_variables: COMMA variable EQUAL boolean_var
	| COMMA variable EQUAL boolean_var extra_boolean_variables
	| ;


more_variables: COMMA variable | COMMA variable more_variables | ;


method_declarations: c method_declaration c | c method_declaration c method_declarations | ;


method_declaration: method_modifier return_type variable LEFT_BRACKET parameters RIGHT_BRACKET LEFT_BRACE method_block return RIGHT_BRACE;


parameters: data_type variable
	| data_type variable COMMA parameters
	|;


method_block: variable_declarations commands ;


commands: c command c | c command c commands | ;


command: assignment | do_loop | for_loop | switch_case | break | method_call QMARK | print | if_else ;


assignment: variable EQUAL expression QMARK ;

member_access: variable DOT variable LEFT_BRACKET arguments RIGHT_BRACKET
		| variable DOT variable ;


method_call: variable LEFT_BRACKET arguments RIGHT_BRACKET ;


arguments: variable | variable COMMA arguments | normal_type | normal_type COMMA arguments | ; 


expression: normal_type | member_access | method_call | operations ;


operations: num operator_symbol operations | num | variable operator_symbol operations | variable;


do_loop: DO c LEFT_BRACE commands RIGHT_BRACE c  WHILE LEFT_BRACKET conditions RIGHT_BRACKET QMARK ;


conditions: condition | condition logic_operators conditions ;


condition: variable compare_symbol normal_type 
		| normal_type compare_symbol variable
		| variable compare_symbol VAR
		| normal_type compare_symbol normal_type ;


logic_operators: AND | OR ;


compare_symbol: LESS | GREATER | NOT_EQUAL | CHECK_EQUAL ;


operator_symbol: DIV | MUL | PLUS | MINUS ;


for_loop: FOR LEFT_BRACKET for_assignment QMARK conditions QMARK increment RIGHT_BRACKET c LEFT_BRACE commands RIGHT_BRACE c ;



for_assignment: variable EQUAL num | variable EQUAL variable ;


increment: variable EQUAL variable operator_symbol num 
		| variable EQUAL variable operator_symbol variable ;


switch_case: SWITCH LEFT_BRACKET variable RIGHT_BRACKET c LEFT_BRACE c cases c DEFAULT  commands RIGHT_BRACE
		| SWITCH LEFT_BRACKET variable RIGHT_BRACKET c LEFT_BRACE c cases c RIGHT_BRACE ;


cases: CASE normal_type DOUBLE_DOT commands | CASE normal_type DOUBLE_DOT commands cases ;


if_else: c IF LEFT_BRACKET conditions RIGHT_BRACKET c LEFT_BRACE commands RIGHT_BRACE c else_if c else c ;



else_if: c ELSE_IF LEFT_BRACKET conditions RIGHT_BRACKET c LEFT_BRACE commands RIGHT_BRACE c 
	|c ELSE_IF LEFT_BRACKET conditions RIGHT_BRACKET c LEFT_BRACE commands RIGHT_BRACE c else_if c
	| ;



else: c ELSE c LEFT_BRACE commands RIGHT_BRACE | ;



normal_type: INT_NUM | DOUBLE_NUM | TRUE | FALSE | CHAR_VAR | STRING_VAR ; 


object_instance: NEW CLASS_NAME LEFT_BRACKET RIGHT_BRACKET ; 


return: RETURN num QMARK ;
	|RETURN expression QMARK ; 

break: BREAK QMARK ;


print: PRINT LEFT_BRACKET STRING_VAR print_vars RIGHT_BRACKET QMARK ;


print_vars: COMMA variable | COMMA variable print_vars | ;


variable_modifier: PUBLIC | PRIVATE | ;


return_type: INT | DOUBLE | CHAR | BOOLEAN | STRING | VOID ; 


data_type: INT | DOUBLE | CHAR | BOOLEAN | STRING ;


method_modifier: M_PUBLIC | M_PRIVATE | PROTECTED ;


variable: VAR ;

num: INT_NUM | DOUBLE_NUM ;


c: comment | c comment | ;

comment: SINGLE_COMMENT | MULTILINE_COMMENT ;


boolean_var : TRUE | FALSE ;


%%


int main(int argc, char** argv) { 
    char filename[] = "file.txt";
    if(argc == 2)
    {
	strcpy(filename, argv[1]);
    }
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror(filename);
        return 1;
    }
    yyin = file;
    yyparse();
    fclose(file);
    return 0;
}
