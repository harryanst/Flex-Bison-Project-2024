%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern FILE* yyin;
extern int line_number;


// Data structure for a symbol entry
typedef struct {
    char name[256];        // Assuming maximum variable name length of 255 characters
    char data_type[20];   // Data type: INT, DOUBLE, CHAR, BOOLEAN, STRING
} SymbolEntry;


// Maximum number of normal_types (adjust as needed)
#define MAX_EXPR 100

// Creating a table of normal_types and calling it "symbol_table"
SymbolEntry symbol_table[MAX_EXPR];
int expr_count = 0;


// Function Signatures
void add_symbol(char *name, char *data_type);
void clearTable(SymbolEntry *symbol_table);
int compareAll(SymbolEntry *symbol_table);


void yyerror(char *s) {
   fprintf(stderr, "Error at line %d: %s\n", line_number, s);
}


// Function to add a normal_type to the normal_types table
void add_symbol(char *name, char *data_type) {
    if (expr_count < MAX_EXPR) {
        strcpy(symbol_table[expr_count].name, name);
        strcpy(symbol_table[expr_count].data_type, data_type);
        expr_count++;
    } else {
        fprintf(stderr, "Error: Expressions table full\n");
        exit(EXIT_FAILURE);
    }
}


// Function to clear the normal_types table
void clearTable(SymbolEntry *symbol_table){
	 for (int i = 1; i < MAX_EXPR; i++) {  
		if (strcmp(symbol_table[i].data_type, "") != 0){ 
			 strcpy(symbol_table[i].data_type, "");
		}
	}
}

//Function to compare entries of normal_types table
int compareAll(SymbolEntry *symbol_table) {
							
									
	// Get the value of the first entry
	char *firstValue = symbol_table[0].data_type;

	// Iterate over the remaining entries and compare their values
	for (int i = 1; i < MAX_EXPR; i++) {
		
		if (strcmp(symbol_table[i].data_type, "") != 0){
			
			// If any value is different from the first one, return 0
			if (strcmp(firstValue, symbol_table[i].data_type) != 0) {
								
			return 0;
			}
		}
		else{continue;}
	}

	// If all values are equal, return 1
	return 1;

}


%}



%union {
 char *str;
}


%token PUBLIC_CLASS CLASS_NAME LEFT_BRACE RIGHT_BRACE PUBLIC PRIVATE QMARK 
%token <str> VAR
%token <str> INT DOUBLE CHAR BOOLEAN STRING 
%token <str>  DOUBLE_NUM INT_NUM CHAR_VAR STRING_VAR TRUE FALSE 
%token DOT LEFT_BRACKET RIGHT_BRACKET COMMA  MUL DIV PLUS MINUS
%token EQUAL NEW  VOID PROTECTED M_PUBLIC M_PRIVATE
%token RETURN
%token DO WHILE AND OR CHECK_EQUAL LESS GREATER NOT_EQUAL FOR SWITCH CASE 
%token DOUBLE_DOT DEFAULT BREAK PRINT IF ELSE ELSE_IF
%token SINGLE_COMMENT MULTILINE_COMMENT

%type <str> variable
%type <str> data_type
%type <str> normal_type                  //only normal_type assignments with same line variable declarations
%type <str> extra_assigned_variables


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





variable_declaration:   c variable_modifier data_type variable extra_variables QMARK c
						| c CLASS_NAME variable EQUAL object_instance QMARK c
						| c variable_modifier data_type variable EQUAL normal_type extra_assigned_variables QMARK c
						{		
 							
							// Add the first variable and its expression to the symbol table
							add_symbol($4, $6);
							 
						}
						c
						{	
							
							if (!compareAll(symbol_table)) {
								printf("Error: Expressions are not all of the same type.\n");
								exit(EXIT_FAILURE);
							} 
							else {
								
								expr_count=0;
								clearTable(symbol_table);
							}
						}						
                                                ;


extra_variables: COMMA variable extra_variables
	             | ;


extra_assigned_variables: | COMMA variable EQUAL normal_type extra_assigned_variables 
                          {
					
					// Add extra variables and their assigned expressions to symbol_table 
					add_symbol($2, $4);
					
                          }  
			  ;



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



normal_type: DOUBLE_NUM | INT_NUM | TRUE | FALSE | CHAR_VAR | STRING_VAR ; 


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
