%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


extern int yylex();
extern FILE* yyin;
extern int line_number;
extern int method_count;
extern int var_id_count;
extern char numval[50];    // Variable to store yytext from INT_NUM and DOUBLE_NUM


// Maximum number of normal_types (adjust as needed)
#define MAX_SIZE 1000

int declaration_flag = 0;
char public_name[256][MAX_SIZE];
int var_num = 0;

// Data structure to store variable name & expression type
typedef struct {
    char name[256];        		// Assuming maximum variable name length of 255 characters
    char expr_type[20];   	 	// Data type: INT_NUM, DOUBLE_NUM, CHAR_VAR, STRING_VAR, BOOLEAN
} ExprEntry, DeclaredVar;


//Data structure for a method
typedef struct {
	char method_name[256];
	char method_type[256];
	char method_modifier[256];
} M_call;


typedef struct {
	char modifier[256];
	char name[256];
} Var_check;


typedef struct {
	char name[256];
} Extra_var;


typedef struct {
	char name[256];
	char value[50];
} Assign_record;



// Creating a table of normal_types and calling it "data_table"
ExprEntry data_table[MAX_SIZE];
int expr_count = 0;


DeclaredVar declaredVar_table[MAX_SIZE];
int var_count = 0;


M_call m_table[MAX_SIZE];
int m_count = 0;


Var_check id_table[MAX_SIZE];
int id_count = 0;


Extra_var ex_table[MAX_SIZE];
int ex_count = 0;


Assign_record assign_table[MAX_SIZE];
int ass_count = 0;


// Function to add a variable with its INT_NUM or DOUBLE_NUM assignment to the assign_table
void add_assign(char *name, char *value) {
	
	if (ass_count < MAX_SIZE) {
        strcpy(assign_table[ass_count].name, name);
	strcpy(assign_table[ass_count].value, value);
        ass_count++;
    } else {
        fprintf(stderr, "Error: table full\n");
        exit(EXIT_FAILURE);
    }
}



// Function to search (bottom up) for the most recently assigned value (of type INT_NUM or DOUBLE_NUM) to a variable --> used in <opearations> rule 
char *findOperationValue(char *name) {
    char foundValue[256] = "";
    int found = 0;

    // Start searching from the end of the array
    for (int i = ass_count - 1; i >= 0; i--) {
        if (strcmp(name, assign_table[i].name) == 0) {
            found = 1;
			return assign_table[i].value;  // Store the found variable's name
            
        }
    }
    if(!found) {
	fprintf(stderr, "  Variable '%s' must have an assigned integer or double value!\n", name);
	exit(EXIT_FAILURE);
    }	
}



void add_id(char *modifier, char *name) {
	
	if (id_count < MAX_SIZE) {
        strcpy(id_table[id_count].name, name);
	strcpy(id_table[id_count].modifier, modifier);
        id_count++;
    } else {
        fprintf(stderr, "Error: table full\n");
        exit(EXIT_FAILURE);
    }
}

void add_extra(char *name) {
	
	if (ex_count < MAX_SIZE) {
        strcpy(ex_table[ex_count].name, name);
        ex_count++;
    } else {
        fprintf(stderr, "Error: table full\n");
        exit(EXIT_FAILURE);
    }
}



void clear_extra() {

	for(int i = 0; i < MAX_SIZE; i++){
		strcpy(ex_table[ex_count].name, "");
	}
	
	ex_count = 0;		
}


void clear_var_private() {
    for (int i = 0; i < MAX_SIZE; i++) {
	if (strcmp(id_table[i].modifier, "private") == 0)
	{
        strcpy(id_table[i].name, "");
	strcpy(id_table[i].modifier, "");
	}
    }
}


void check_var_private(char *name) {
    int flag = 0;

    for (int i = 0; i < MAX_SIZE; i++) {
        if (strcmp(id_table[i].name, name) == 0) {
            flag = 1;
            break; 
        }
    }

    if (flag == 0) {
        fprintf(stderr, "Public '%s' does not exist!\n", name);
        exit(EXIT_FAILURE);
    }
}


// Adds methods to the table
void add_method(char *modifier, char *type, char *name) {
	
	if (m_count < MAX_SIZE) {
        strcpy(m_table[m_count].method_name, name);
	strcpy(m_table[m_count].method_type, type);
	strcpy(m_table[m_count].method_modifier, modifier);
        m_count++;
    } else {
        fprintf(stderr, "Error: Methods table full\n");
        exit(EXIT_FAILURE);
    }
}

// Checks whether the method called exists
void check_method(const char *name) {
    int flag = 0;

    for (int i = 0; i < MAX_SIZE; i++) {
        if (strcmp(m_table[i].method_name, name) == 0) {
            flag = 1;
            break; 
        }
    }

    if (flag == 0) {
        fprintf(stderr, "Method '%s' does not exist!\n", name);
        exit(EXIT_FAILURE);
    }
}

// Removes the private methods from the table
void clear_private_methods() {
    for (int i = 0; i < MAX_SIZE; ++i) {
	if (strcmp(m_table[i].method_modifier, "Private") == 0)
	{
        strcpy(m_table[i].method_name, "");
	strcpy(m_table[i].method_type, "");
	strcpy(m_table[i].method_modifier, "");
	}
    }
    //m_count = 0;
    //printf("Methods Table for private cleared.\n");
}



// Function to add a normal_type to the normal_types table
void addVar(char *name, char *expr_type) {
    if (expr_count < MAX_SIZE) {
        strcpy(data_table[expr_count].name, name);
        strcpy(data_table[expr_count].expr_type, expr_type);
        expr_count++;
    } else {
        fprintf(stderr, "Error: Expressions table full\n");
        exit(EXIT_FAILURE);
    }
}


// Function to add a variable to the declared variables table
void add_declaredVar(char *name, char *expr_type) {
    if (expr_count < MAX_SIZE) {
        strcpy(declaredVar_table[var_count].name, name);
        strcpy(declaredVar_table[var_count].expr_type, expr_type);
        var_count++;
    } else {
        fprintf(stderr, "Error: Declared variables' table full\n");
        exit(EXIT_FAILURE);
    }
}


// Function to clear the normal_types table
void clearTable(ExprEntry *data_table){
	 for (int i = 1; i < MAX_SIZE; i++) {  
		if (strcmp(data_table[i].expr_type, "") != 0){ 
			 strcpy(data_table[i].expr_type, "");
		}
	}
}

//Function to compare entries of normal_types table
int compareAll(ExprEntry *data_table) {
							
									
	// Get the value of the first entry
	char *firstValue = data_table[0].expr_type;

	// Iterate over the remaining entries and compare their values
	for (int i = 1; i < MAX_SIZE; i++) {
		
		if (strcmp(data_table[i].expr_type, "") != 0){
			
			// If any value is different from the first one, return 0
			if (strcmp(firstValue, data_table[i].expr_type) != 0) {
								
			return 0;
			}
		}
		else{continue;}
	}

	// If all values are equal, return 1
	return 1;

}

// Function to search for a variable in the array bottom up
char *searchVariable(char *varName) {
    char foundVar[256] = "";
    int found = 0;

    // Start searching from the end of the array
    for (int i = var_count - 1; i >= 0; i--) {
        if (strcmp(varName, declaredVar_table[i].name) == 0) {
            strcpy(foundVar, declaredVar_table[i].name); // Store the found variable name
            found = 1;
            return declaredVar_table[i].expr_type;
        }
    }
    if(!found) {
	return "";
    }	
}

void yyerror(char *s) {
   fprintf(stderr, "Error at line %d: %s\n", line_number, s);
}


%}



%union {
 char *str;
}


%token PUBLIC_CLASS CLASS_NAME LEFT_BRACE RIGHT_BRACE QMARK 
%token <str> PUBLIC PRIVATE
%token <str> VAR
%token <str> INT DOUBLE CHAR BOOLEAN STRING 
%token <str> M_PUBLIC M_PRIVATE VOID
%token <str> INT_NUM DOUBLE_NUM CHAR_VAR STRING_VAR TRUE FALSE 
%token DOT LEFT_BRACKET RIGHT_BRACKET COMMA  MUL DIV PLUS MINUS  
%token EQUAL NEW   
%token RETURN
%token DO WHILE AND OR CHECK_EQUAL LESS GREATER NOT_EQUAL FOR SWITCH CASE 
%token DOUBLE_DOT DEFAULT BREAK PRINT IF ELSE ELSE_IF
%token SINGLE_COMMENT MULTILINE_COMMENT

%left PLUS MINUS
%left MUL DIV 
%nonassoc LEFT_BRACKET RIGHT_BRACKET


%type <str> variable
%type <str> expr_type
%type <str> normal_type                  //only normal_type assignments with same line variable declarations
%type <str> method_modifier
%type <str> return_type
%type <str> variable_modifier
%type <str> operations
%type <str> num

%%

program: c public_class c
        | program c public_class c
	| error '\n' ;

public_class: PUBLIC_CLASS CLASS_NAME LEFT_BRACE class_block RIGHT_BRACE

		{
		method_count = method_count - 1;

			if (method_count == 0) {

			// printf("\n original method \n");

			clear_private_methods();
			clear_var_private();

			}
		}

                ;

class_block: variable_declarations method_declarations nested_class

		;


nested_class: public_class | nested_class c public_class | ;


variable_declarations: variable_declaration | variable_declaration variable_declarations | ;





variable_declaration:   c variable_modifier expr_type variable extra_variables QMARK c
			{
				char temp_type[20];
				strcpy(temp_type, "");
				
				if (strcmp($3, "int") == 0) {
   				 strcpy(temp_type, "INT_NUM");
				} else if (strcmp($3, "double") == 0) {
 				   strcpy(temp_type, "DOUBLE_NUM");
				} else if (strcmp($3, "boolean") == 0) {
 				   strcpy(temp_type, "BOOLEAN");
				} else if (strcmp($3, "String") == 0) {
 				   strcpy(temp_type, "STRING_VAR");
				} else if (strcmp($3, "char") == 0) {
  				  strcpy(temp_type, "CHAR_VAR");
				} else {
 				   strcpy(temp_type, "");
				}
				
				if(strcmp(temp_type, "") == 0) {
				 fprintf(stderr, "Error: Unknown type\n");
				 exit(EXIT_FAILURE);
				} else {
				add_declaredVar($4, temp_type); }

				if(declaration_flag == 1) { 
				 for(int i = 0; i < var_num; i++) {
				  add_declaredVar(public_name[i], temp_type); } }
			
				var_num = 0;
				declaration_flag = 0;


				if(var_id_count == 0) {
				add_id($2, $4);
				
				for (int i = 0; i < 100; i++){
					if (strcmp(ex_table[i].name, "") != 0) {
						add_id($2, ex_table[i].name);
					}
				}
				
				}


				if(var_id_count > 0) {
				add_id("method", $4);
				
				for (int i = 0; i < 100; i++){
					if (strcmp(ex_table[i].name, "") != 0) {
						add_id("method", ex_table[i].name);
					}
				}
				
				}

				clear_extra();

			}
			
						| c CLASS_NAME variable EQUAL object_instance QMARK c
						| c variable_modifier expr_type variable EQUAL normal_type extra_assigned_variables QMARK c
						{		
 							
							 if (strcmp($6, "INT_NUM") == 0 | strcmp($6, "DOUBLE_NUM") == 0){
			
							 add_assign($4, numval);
			
							 }

							
							
							// Add the first variable and its expression to the symbol table
							addVar($4, $6);
							add_declaredVar($4, $6);


							if(var_id_count == 0) {
							add_id($2, $4); //Πεταει segmatation
				
							for (int b = 0; b < 100; b++){
								if (strcmp(ex_table[b].name, "") != 0) {
								add_id($2, ex_table[b].name);
								}
							}

							}



							if(var_id_count > 0) {
							add_id("method", $4);
				
								for (int i = 0; i < 100; i++){
									if (strcmp(ex_table[i].name, "") != 0) {
									add_id("method", ex_table[i].name);
									}
								}
				
							}

							clear_extra();

							 
						}
						c
						{	
							
							if (!compareAll(data_table)) {
								printf("Error: Expressions are not all of the same type.\n");
								exit(EXIT_FAILURE);
							} 
							else {
								
								expr_count=0;
								clearTable(data_table);
							}
						}
						c
						{

							if(strcmp($3, "int") == 0)
								{
									if(strcmp($6, "INT_NUM") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
							else if(strcmp($3, "double") == 0)
								{
									if(strcmp($6, "DOUBLE_NUM") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
							else if(strcmp($3, "char") == 0)
								{
									if(strcmp($6, "CHAR_VAR") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
							else if(strcmp($3, "boolean") == 0)
								{
									if(strcmp($6, "BOOLEAN") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
							else if(strcmp($3, "String") == 0)
								{
									if(strcmp($6, "STRING_VAR") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
						
						}

| c expr_type variable extra_variables QMARK c
			{
				char temp_type[20];
				strcpy(temp_type, "");
				
				if (strcmp($2, "int") == 0) {
   				 strcpy(temp_type, "INT_NUM");
				} else if (strcmp($2, "double") == 0) {
 				   strcpy(temp_type, "DOUBLE_NUM");
				} else if (strcmp($2, "boolean") == 0) {
 				   strcpy(temp_type, "BOOLEAN");
				} else if (strcmp($2, "String") == 0) {
 				   strcpy(temp_type, "STRING_VAR");
				} else if (strcmp($2, "char") == 0) {
  				  strcpy(temp_type, "CHAR_VAR");
				} else {
 				   strcpy(temp_type, "");
				}
				
				if(strcmp(temp_type, "") == 0) {
				 fprintf(stderr, "Error: Unknown type\n");
				 exit(EXIT_FAILURE);
				} else {
				add_declaredVar($3, temp_type); }

				if(declaration_flag == 1) { 
				 for(int i = 0; i < var_num; i++) {
				  add_declaredVar(public_name[i], temp_type); } }
			
				var_num = 0;
				declaration_flag = 0;


				if(var_id_count == 0) {
				add_id("public", $3);
				
				for (int i = 0; i < 100; i++){
					if (strcmp(ex_table[i].name, "") != 0) {
						add_id("public", ex_table[i].name);
					}
				}
				
				}


				if(var_id_count > 0) {
				add_id("method", $3);
				
				for (int i = 0; i < 100; i++){
					if (strcmp(ex_table[i].name, "") != 0) {
						add_id("method", ex_table[i].name);
					}
				}
				
				}

				clear_extra();

			}
			
						| c expr_type variable EQUAL normal_type extra_assigned_variables QMARK c
						{		
 							
							 if (strcmp($5, "INT_NUM") == 0 | strcmp($5, "DOUBLE_NUM") == 0){
			
								add_assign($3, numval);
			
							 }
	
							// Add the first variable and its expression to the symbol table
							addVar($3, $5);
							add_declaredVar($3, $5);


							if(var_id_count == 0) {
							add_id("public", $3); //Πεταει segmatation
				
							for (int b = 0; b < 100; b++){
								if (strcmp(ex_table[b].name, "") != 0) {
								add_id("public", ex_table[b].name);
								}
							}

							}



							if(var_id_count > 0) {
							add_id("method", $3);
				
								for (int i = 0; i < 100; i++){
									if (strcmp(ex_table[i].name, "") != 0) {
									add_id("method", ex_table[i].name);
									}
								}
				
							}

							clear_extra();

							 
						}
						c
						{	
							
							if (!compareAll(data_table)) {
								printf("Error: Expressions are not all of the same type.\n");
								exit(EXIT_FAILURE);
							} 
							else {
								
								expr_count=0;
								clearTable(data_table);
							}
						}
						c
						{

							if(strcmp($2, "int") == 0)
								{
									if(strcmp($5, "INT_NUM") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
							else if(strcmp($2, "double") == 0)
								{
									if(strcmp($5, "DOUBLE_NUM") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
							else if(strcmp($2, "char") == 0)
								{
									if(strcmp($5, "CHAR_VAR") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
							else if(strcmp($2, "boolean") == 0)
								{
									if(strcmp($5, "BOOLEAN") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
							else if(strcmp($2, "String") == 0)
								{
									if(strcmp($5, "STRING_VAR") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
						
						}
						
                                                ;


extra_variables: | COMMA variable extra_variables
		{
			declaration_flag = 1;
			strcpy(public_name[var_num], $2);
			var_num++;

			add_extra($2);

		};


extra_assigned_variables: | COMMA variable EQUAL normal_type extra_assigned_variables 
                          {
					
								if (strcmp($4, "INT_NUM") == 0 | strcmp($4, "DOUBLE_NUM") == 0){
			
									add_assign($2, numval);
			
								}
					
					
								// Add extra variables and their assigned expressions to data_table 
								addVar($2, $4);
								add_declaredVar($2, $4);
				
								add_extra($2);	
					
                          };



method_declarations: c method_declaration c | c method_declaration c method_declarations | ;


method_declaration: method_modifier return_type variable LEFT_BRACKET parameters RIGHT_BRACKET LEFT_BRACE method_block return RIGHT_BRACE
	{	
	add_method($1, $2, $3);
	var_id_count = var_id_count - 1;


	for (int i = 0; i < 100; i++) {
		if (strcmp(id_table[i].modifier, "method") == 0)
		{
        		strcpy(id_table[i].name, "");
			strcpy(id_table[i].modifier, "");
		}
    	}

	}
	;


parameters: expr_type variable
	| expr_type variable COMMA parameters
	|;


method_block: c variable_declarations c commands c ;


commands: c command c | c command c commands | ;


command: assignment | do_loop | for_loop | switch_case | break | method_call QMARK | print | if_else ;


assignment: VAR EQUAL expression QMARK 
	    | VAR EQUAL operations QMARK 
	    {
			// Print result of operation as a double
			char *eptr;
			printf("Value of %s is:  %lf \n", $1, strtod($3, &eptr)); 
	    }
		
	    | VAR EQUAL normal_type QMARK
	    {
            if (strcmp($3, "INT_NUM") == 0 | strcmp($3, "DOUBLE_NUM") == 0){
			
					add_assign($1, numval);
			
			}

			
			check_var_private($1);

			char* foundVariableType = searchVariable($1);
			if(strcmp(foundVariableType, "") == 0)
				{
					fprintf(stderr, "Error: Undeclared variable\n");
					exit(EXIT_FAILURE);
				}
			else
				{
						if(strcmp(foundVariableType, $3) != 0)
							{
								fprintf(stderr, "Error: Type mismatch\n");
								exit(EXIT_FAILURE);
							}
				}
		};


member_access: variable DOT variable LEFT_BRACKET arguments RIGHT_BRACKET
		| variable DOT variable ;


method_call: variable LEFT_BRACKET arguments RIGHT_BRACKET 

	{
	check_method($1);
	}
	;


arguments: variable | variable COMMA arguments | normal_type | normal_type COMMA arguments | ; 


expression: member_access | method_call ;



//operations and num rules are of type <str>. To perform operations i convert string values to double and then convert back to string again to store the result in $$

operations:  LEFT_BRACKET operations RIGHT_BRACKET    {$$=$2;}  
			| operations PLUS operations  {double temp; char *endptr1; char *endptr2; temp = strtod($1, &endptr1) + strtod($3, &endptr2); sprintf($$, "%lf", temp);} 
			| operations MINUS operations {double temp; char *endptr1; char *endptr2; temp = strtod($1, &endptr1) - strtod($3, &endptr2); sprintf($$, "%lf", temp);}
			| operations MUL operations   {double temp; char *endptr1; char *endptr2; temp = strtod($1, &endptr1) * strtod($3, &endptr2); sprintf($$, "%lf", temp);}
			| operations DIV operations   {double temp; char *endptr1; char *endptr2; temp = strtod($1, &endptr1) / strtod($3, &endptr2); sprintf($$, "%lf", temp);}
			| num 			      { strcpy($$, numval); }            
			| variable                    { strcpy($$, findOperationValue($$)); }
			;


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


return: RETURN normal_type QMARK 
	|RETURN expression QMARK 
	| ; 

break: BREAK QMARK ;


print: PRINT LEFT_BRACKET STRING_VAR print_vars RIGHT_BRACKET QMARK ;


print_vars: COMMA variable | COMMA variable print_vars | ;


variable_modifier: PUBLIC | PRIVATE ;


return_type: INT | DOUBLE | CHAR | BOOLEAN | STRING | VOID ; 


expr_type: INT | DOUBLE | CHAR | BOOLEAN | STRING ;


method_modifier: M_PUBLIC | M_PRIVATE ;


variable: VAR ;


num: INT_NUM | DOUBLE_NUM ;

c: comment | comment c | ;

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
