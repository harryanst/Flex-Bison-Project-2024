/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1000      //adjust as needed

extern int yylex();
extern FILE* yyin;
extern int line_number;
extern int method_count;
extern int var_id_count;
extern char numval[50];    // Variable to store yytext from INT_NUM and DOUBLE_NUM

int declaration_flag = 0;
int var_num = 0;




//Structs

typedef struct {		        // Data structure to store variable name & expression type
    char name[256];        		// Assuming maximum variable name length of 255 characters
    char expr_type[20];   	 	// Expression type: INT_NUM, DOUBLE_NUM, CHAR_VAR, STRING_VAR, BOOLEAN
} ExprEntry, DeclaredVar;



typedef struct {                        // Data structure to store a method's modifier, type, name
	char method_name[256];
	char method_type[256];
	char method_modifier[256];
} M_call;


typedef struct {			// Data structure to store a variable's modifier, name
	char modifier[256];
	char name[256];
} Var_check;


typedef struct {			// Data structure to store the name of extra variables declared
	char name[256];
} Extra_var;


typedef struct {			// Data structure to store a variable's name, value after assignment
	char name[256];
	char value[50];
} Assign_record;



//Tables
ExprEntry data_table[MAX_SIZE];
int expr_count = 0;

DeclaredVar declaredVar_table[MAX_SIZE];
int var_count = 0;

char public_name[256][MAX_SIZE];

M_call m_table[MAX_SIZE];
int m_count = 0;

Var_check id_table[MAX_SIZE];
int id_count = 0;

Extra_var ex_table[MAX_SIZE];
int ex_count = 0;

Assign_record assign_table[MAX_SIZE];
int ass_count = 0;



//Function Signatures
void addVar(char *name, char *expr_type);
int compareAll(ExprEntry *data_table);
void clearTable(ExprEntry *data_table);
void add_method(char *modifier, char *type, char *name);
void check_method(const char *name);
void add_declaredVar(char *name, char *expr_type);
char *searchVariable(char *varName);
void add_id(char *modifier, char *name);
void add_extra(char *name);
void clear_extra();
void check_var_private(char *name);
void clear_var_private(); 
void clear_private_methods(); 
void add_assign(char *name, char *value);
char *findOperationValue(char *name);





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



void clearTable(ExprEntry *data_table){
	 for (int i = 1; i < MAX_SIZE; i++) {  
		if (strcmp(data_table[i].expr_type, "") != 0){ 
			 strcpy(data_table[i].name, "");
			 strcpy(data_table[i].expr_type, "");
		}
	}
}



// Adds methods to the m_table
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



// Function to search for a variable in the declaredVar_table bottom up
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



void clear_var_private() {
    for (int i = 0; i < MAX_SIZE; i++) {
	if (strcmp(id_table[i].modifier, "private") == 0)
	{
        strcpy(id_table[i].name, "");
	strcpy(id_table[i].modifier, "");
	}
    }
}



// Removes the private methods from the m_table
void clear_private_methods() {
    for (int i = 0; i < MAX_SIZE; ++i) {
	if (strcmp(m_table[i].method_modifier, "Private") == 0)
	{
        strcpy(m_table[i].method_name, "");
	strcpy(m_table[i].method_type, "");
	strcpy(m_table[i].method_modifier, "");
	}
    }
}



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



void yyerror(char *s) {
   fprintf(stderr, "Error at line %d: %s\n", line_number, s);
}



/* Line 371 of yacc.c  */
#line 410 "parser.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.h".  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PUBLIC_CLASS = 258,
     CLASS_NAME = 259,
     LEFT_BRACE = 260,
     RIGHT_BRACE = 261,
     QMARK = 262,
     PUBLIC = 263,
     PRIVATE = 264,
     VAR = 265,
     INT = 266,
     DOUBLE = 267,
     CHAR = 268,
     BOOLEAN = 269,
     STRING = 270,
     M_PUBLIC = 271,
     M_PRIVATE = 272,
     VOID = 273,
     INT_NUM = 274,
     DOUBLE_NUM = 275,
     CHAR_VAR = 276,
     STRING_VAR = 277,
     TRUE = 278,
     FALSE = 279,
     DOT = 280,
     LEFT_BRACKET = 281,
     RIGHT_BRACKET = 282,
     COMMA = 283,
     MUL = 284,
     DIV = 285,
     PLUS = 286,
     MINUS = 287,
     EQUAL = 288,
     NEW = 289,
     RETURN = 290,
     DO = 291,
     WHILE = 292,
     AND = 293,
     OR = 294,
     CHECK_EQUAL = 295,
     LESS = 296,
     GREATER = 297,
     NOT_EQUAL = 298,
     FOR = 299,
     SWITCH = 300,
     CASE = 301,
     DOUBLE_DOT = 302,
     DEFAULT = 303,
     BREAK = 304,
     PRINT = 305,
     IF = 306,
     ELSE = 307,
     ELSE_IF = 308,
     SINGLE_COMMENT = 309,
     MULTILINE_COMMENT = 310
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 345 "parser.y"

 char *str;


/* Line 387 of yacc.c  */
#line 513 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 541 "parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   347

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  53
/* YYNRULES -- Number of rules.  */
#define YYNRULES  133
/* YYNRULES -- Number of states.  */
#define YYNSTATES  297

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   310

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     7,    12,    18,    22,    24,    28,    29,
      31,    34,    35,    43,    51,    52,    53,    67,    74,    75,
      76,    89,    90,    94,    95,   101,   105,   110,   111,   122,
     125,   130,   131,   137,   141,   146,   147,   149,   151,   153,
     155,   157,   160,   162,   164,   169,   174,   179,   186,   190,
     195,   197,   201,   203,   207,   208,   210,   212,   216,   220,
     224,   228,   232,   234,   236,   248,   250,   254,   258,   262,
     266,   270,   272,   274,   276,   278,   280,   282,   284,   286,
     288,   290,   304,   308,   312,   318,   324,   337,   348,   353,
     359,   374,   385,   398,   399,   406,   407,   409,   411,   413,
     415,   417,   419,   424,   428,   432,   433,   436,   443,   446,
     450,   451,   453,   455,   457,   459,   461,   463,   465,   467,
     469,   471,   473,   475,   477,   479,   481,   483,   485,   487,
     489,   492,   493,   495
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      57,     0,    -1,   107,    58,   107,    -1,    57,   107,    58,
     107,    -1,     3,     4,     5,    59,     6,    -1,    61,    69,
      60,    -1,    58,    -1,    60,   107,    58,    -1,    -1,    62,
      -1,    62,    61,    -1,    -1,   107,   101,   103,   105,    67,
       7,   107,    -1,   107,     4,   105,    33,    96,     7,   107,
      -1,    -1,    -1,   107,   101,   103,   105,    33,    95,    68,
       7,   107,    63,   107,    64,   107,    -1,   107,   103,   105,
      67,     7,   107,    -1,    -1,    -1,   107,   103,   105,    33,
      95,    68,     7,   107,    65,   107,    66,   107,    -1,    -1,
      28,   105,    67,    -1,    -1,    28,   105,    33,    95,    68,
      -1,   107,    70,   107,    -1,   107,    70,   107,    69,    -1,
      -1,   104,   102,   105,    26,    71,    27,     5,    72,    97,
       6,    -1,   103,   105,    -1,   103,   105,    28,    71,    -1,
      -1,   107,    61,   107,    73,   107,    -1,   107,    74,   107,
      -1,   107,    74,   107,    73,    -1,    -1,    75,    -1,    81,
      -1,    87,    -1,    90,    -1,    98,    -1,    77,     7,    -1,
      99,    -1,    92,    -1,    10,    33,    79,     7,    -1,    10,
      33,    80,     7,    -1,    10,    33,    95,     7,    -1,   105,
      25,   105,    26,    78,    27,    -1,   105,    25,   105,    -1,
     105,    26,    78,    27,    -1,   105,    -1,   105,    28,    78,
      -1,    95,    -1,    95,    28,    78,    -1,    -1,    76,    -1,
      77,    -1,    26,    80,    27,    -1,    80,    31,    80,    -1,
      80,    32,    80,    -1,    80,    29,    80,    -1,    80,    30,
      80,    -1,   106,    -1,   105,    -1,    36,   107,     5,    73,
       6,   107,    37,    26,    82,    27,     7,    -1,    83,    -1,
      83,    84,    82,    -1,   105,    85,    95,    -1,    95,    85,
     105,    -1,   105,    85,    10,    -1,    95,    85,    95,    -1,
      38,    -1,    39,    -1,    41,    -1,    42,    -1,    43,    -1,
      40,    -1,    30,    -1,    29,    -1,    31,    -1,    32,    -1,
      44,    26,    88,     7,    82,     7,    89,    27,   107,     5,
      73,     6,   107,    -1,   105,    33,   106,    -1,   105,    33,
     105,    -1,   105,    33,   105,    86,   106,    -1,   105,    33,
     105,    86,   105,    -1,    45,    26,   105,    27,   107,     5,
     107,    91,   107,    48,    73,     6,    -1,    45,    26,   105,
      27,   107,     5,   107,    91,   107,     6,    -1,    46,    95,
      47,    73,    -1,    46,    95,    47,    73,    91,    -1,   107,
      51,    26,    82,    27,   107,     5,    73,     6,   107,    93,
     107,    94,   107,    -1,   107,    53,    26,    82,    27,   107,
       5,    73,     6,   107,    -1,   107,    53,    26,    82,    27,
     107,     5,    73,     6,   107,    93,   107,    -1,    -1,   107,
      52,   107,     5,    73,     6,    -1,    -1,    20,    -1,    19,
      -1,    23,    -1,    24,    -1,    21,    -1,    22,    -1,    34,
       4,    26,    27,    -1,    35,    95,     7,    -1,    35,    79,
       7,    -1,    -1,    49,     7,    -1,    50,    26,    22,   100,
      27,     7,    -1,    28,   105,    -1,    28,   105,   100,    -1,
      -1,     8,    -1,     9,    -1,    11,    -1,    12,    -1,    13,
      -1,    14,    -1,    15,    -1,    18,    -1,    11,    -1,    12,
      -1,    13,    -1,    14,    -1,    15,    -1,    16,    -1,    17,
      -1,    10,    -1,    19,    -1,    20,    -1,   108,    -1,   108,
     107,    -1,    -1,    54,    -1,    55,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   379,   379,   380,   383,   398,   403,   403,   403,   406,
     406,   406,   412,   472,   474,   518,   473,   576,   637,   679,
     636,   740,   740,   751,   751,   771,   771,   771,   774,   792,
     793,   794,   797,   800,   800,   800,   803,   803,   803,   803,
     803,   803,   803,   803,   806,   807,   814,   842,   843,   846,
     854,   854,   854,   854,   854,   857,   857,   863,   864,   865,
     866,   867,   868,   869,   873,   876,   876,   879,   880,   881,
     882,   885,   885,   888,   888,   888,   888,   891,   891,   891,
     891,   894,   898,   898,   901,   902,   905,   906,   909,   909,
     912,   916,   917,   918,   922,   922,   926,   926,   926,   926,
     926,   926,   929,   932,   933,   934,   936,   939,   942,   942,
     942,   945,   945,   948,   948,   948,   948,   948,   948,   951,
     951,   951,   951,   951,   954,   954,   957,   960,   960,   962,
     962,   962,   964,   964
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PUBLIC_CLASS", "CLASS_NAME",
  "LEFT_BRACE", "RIGHT_BRACE", "QMARK", "PUBLIC", "PRIVATE", "VAR", "INT",
  "DOUBLE", "CHAR", "BOOLEAN", "STRING", "M_PUBLIC", "M_PRIVATE", "VOID",
  "INT_NUM", "DOUBLE_NUM", "CHAR_VAR", "STRING_VAR", "TRUE", "FALSE",
  "DOT", "LEFT_BRACKET", "RIGHT_BRACKET", "COMMA", "MUL", "DIV", "PLUS",
  "MINUS", "EQUAL", "NEW", "RETURN", "DO", "WHILE", "AND", "OR",
  "CHECK_EQUAL", "LESS", "GREATER", "NOT_EQUAL", "FOR", "SWITCH", "CASE",
  "DOUBLE_DOT", "DEFAULT", "BREAK", "PRINT", "IF", "ELSE", "ELSE_IF",
  "SINGLE_COMMENT", "MULTILINE_COMMENT", "$accept", "program",
  "public_class", "class_block", "nested_class", "variable_declarations",
  "variable_declaration", "$@1", "$@2", "$@3", "$@4", "extra_variables",
  "extra_assigned_variables", "method_declarations", "method_declaration",
  "parameters", "method_block", "commands", "command", "assignment",
  "member_access", "method_call", "arguments", "expression", "operations",
  "do_loop", "conditions", "condition", "logic_operators",
  "compare_symbol", "operator_symbol", "for_loop", "for_assignment",
  "increment", "switch_case", "cases", "if_else", "else_if", "else",
  "normal_type", "object_instance", "return", "break", "print",
  "print_vars", "variable_modifier", "return_type", "data_type",
  "method_modifier", "variable", "num", "c", "comment", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    56,    57,    57,    58,    59,    60,    60,    60,    61,
      61,    61,    62,    62,    63,    64,    62,    62,    65,    66,
      62,    67,    67,    68,    68,    69,    69,    69,    70,    71,
      71,    71,    72,    73,    73,    73,    74,    74,    74,    74,
      74,    74,    74,    74,    75,    75,    75,    76,    76,    77,
      78,    78,    78,    78,    78,    79,    79,    80,    80,    80,
      80,    80,    80,    80,    81,    82,    82,    83,    83,    83,
      83,    84,    84,    85,    85,    85,    85,    86,    86,    86,
      86,    87,    88,    88,    89,    89,    90,    90,    91,    91,
      92,    93,    93,    93,    94,    94,    95,    95,    95,    95,
      95,    95,    96,    97,    97,    97,    98,    99,   100,   100,
     100,   101,   101,   102,   102,   102,   102,   102,   102,   103,
     103,   103,   103,   103,   104,   104,   105,   106,   106,   107,
     107,   107,   108,   108
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     4,     5,     3,     1,     3,     0,     1,
       2,     0,     7,     7,     0,     0,    13,     6,     0,     0,
      12,     0,     3,     0,     5,     3,     4,     0,    10,     2,
       4,     0,     5,     3,     4,     0,     1,     1,     1,     1,
       1,     2,     1,     1,     4,     4,     4,     6,     3,     4,
       1,     3,     1,     3,     0,     1,     1,     3,     3,     3,
       3,     3,     1,     1,    11,     1,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,    13,     3,     3,     5,     5,    12,    10,     4,     5,
      14,    10,    12,     0,     6,     0,     1,     1,     1,     1,
       1,     1,     4,     3,     3,     0,     2,     6,     2,     3,
       0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     0,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
     131,   132,   133,   131,     0,   129,     1,     0,     0,   131,
     130,   131,     0,     2,     3,   131,     0,    27,     9,     0,
       4,     8,     0,    10,     0,   111,   112,   119,   120,   121,
     122,   123,     0,     0,     6,     5,   124,   125,   131,     0,
     126,     0,     0,    21,     0,    25,   113,   114,   115,   116,
     117,   118,     0,     0,    21,     0,     0,     0,     7,    26,
       0,     0,     0,     0,     0,    21,    97,    96,   100,   101,
      98,    99,    23,   131,    31,     0,   131,    23,   131,    22,
       0,     0,    17,     0,     0,     0,    13,     0,    12,     0,
     131,     0,    29,   102,   131,     0,    18,   131,    31,    14,
      23,   131,   105,    11,    30,   131,    24,    19,     0,     0,
     131,    15,   131,    55,    56,     0,     0,     0,    28,   131,
     131,    20,   104,   103,     0,    54,   131,   131,    16,    48,
       0,    52,    50,    32,   126,   131,     0,     0,     0,     0,
     131,    36,     0,    37,    38,    39,    43,    40,    42,     0,
       0,    54,    49,    54,    54,     0,     0,     0,     0,   106,
       0,   131,    41,     0,     0,    53,    51,   127,   128,     0,
       0,     0,     0,    63,    62,   131,     0,     0,     0,   110,
      34,     0,    47,   127,   128,     0,    63,    44,    45,     0,
       0,     0,     0,    46,     0,     0,     0,   131,     0,     0,
       0,    65,     0,     0,    57,    60,    61,    58,    59,   131,
       0,    83,    82,     0,   108,     0,   131,    71,    72,     0,
      76,    73,    74,    75,     0,     0,     0,     0,   131,   109,
     107,     0,    66,    70,    68,    69,    67,     0,     0,     0,
       0,   131,     0,   131,     0,     0,   131,     0,     0,     0,
       0,     0,     0,   131,     0,   131,    78,    77,    79,    80,
       0,   131,    87,   131,    93,    64,     0,    85,    84,    88,
       0,   131,     0,   131,    89,    86,    95,     0,    81,   131,
       0,     0,    90,   131,     0,     0,   131,   131,     0,     0,
     131,    94,     0,   131,    91,   131,    92
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     9,    16,    35,    17,    18,   105,   120,   101,
     112,    57,    81,    21,    38,    83,   102,   126,   140,   141,
     113,   114,   130,   115,   171,   143,   200,   201,   219,   224,
     260,   144,   176,   238,   145,   246,   146,   271,   279,   202,
      62,   109,   147,   148,   199,    32,    52,    84,    39,   186,
     174,   127,     5
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -195
static const yytype_int16 yypact[] =
{
     -29,  -195,  -195,    10,     4,   -29,  -195,     4,    24,   -29,
    -195,   -29,    34,  -195,  -195,    17,    51,    26,   141,   288,
    -195,     4,    30,  -195,    69,  -195,  -195,  -195,  -195,  -195,
    -195,  -195,   305,    69,  -195,     1,  -195,  -195,   -29,   293,
    -195,    56,    69,    -1,     4,    26,  -195,  -195,  -195,  -195,
    -195,  -195,    69,    29,    49,    69,   170,    86,  -195,  -195,
      73,    91,    97,   170,   101,    89,  -195,  -195,  -195,  -195,
    -195,  -195,    93,   -29,   305,    88,   -29,    93,   -29,  -195,
      69,   116,  -195,   139,    69,   144,  -195,   169,  -195,   105,
     -29,   182,   183,  -195,   -29,   170,  -195,   -29,   305,  -195,
      93,   -29,   175,   141,  -195,   -29,  -195,  -195,   217,   208,
     -29,  -195,   -29,  -195,  -195,   216,   218,   111,  -195,    13,
     -29,  -195,  -195,  -195,    69,   217,   -29,   123,  -195,   189,
     202,   204,   207,  -195,   197,   -29,   221,   222,   237,   224,
     -29,  -195,   245,  -195,  -195,  -195,  -195,  -195,  -195,   239,
     191,   217,  -195,   217,   217,   198,   261,    69,    69,  -195,
     246,     6,  -195,   243,   247,  -195,  -195,   263,   265,    99,
     268,   150,   270,   111,  -195,    15,   271,   248,   253,   254,
    -195,   217,  -195,  -195,  -195,   283,  -195,  -195,  -195,    99,
      99,    99,    99,  -195,   278,   217,    65,   -29,    69,   258,
     260,   131,   158,   158,  -195,  -195,  -195,   145,   145,   -29,
     282,  -195,  -195,   285,   254,   284,   -29,  -195,  -195,   217,
    -195,  -195,  -195,  -195,   217,   235,   272,    69,   -29,  -195,
    -195,   316,  -195,  -195,  -195,  -195,  -195,   296,   297,   290,
     252,    15,   217,   -29,    69,   170,   -29,   319,   299,   322,
     231,   281,     2,   -29,   323,    15,  -195,  -195,  -195,  -195,
      65,    52,  -195,    15,    75,  -195,   325,  -195,  -195,   252,
     326,   -29,   276,   -29,  -195,  -195,    92,   307,  -195,   -29,
     286,   217,  -195,   -29,   308,   329,   -29,    15,   331,   333,
      15,  -195,   334,   -29,    75,   -29,  -195
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -195,  -195,    67,  -195,  -195,   -17,  -195,  -195,  -195,  -195,
    -195,   -41,   -63,   292,  -195,   244,  -195,  -139,  -195,  -195,
    -195,   214,    53,   188,    -6,  -195,  -189,  -195,  -195,   142,
    -195,  -195,  -195,  -195,  -195,    77,  -195,    50,  -195,   -12,
    -195,  -195,  -195,  -195,   133,  -195,  -195,    -3,  -195,     7,
    -194,     0,  -195
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -132
static const yytype_int16 yytable[] =
{
       4,    23,   212,     7,  -131,    10,   210,     8,   262,    13,
       6,    14,   -33,    64,    87,    19,    33,    22,    19,   -35,
     -11,   -35,   180,   -11,    79,     1,     2,    55,    12,    42,
     232,    41,    56,   -11,   -11,    44,   194,   106,    45,    15,
      43,   -33,  -131,  -131,    72,    22,    36,    37,   -35,    54,
     263,    77,   -33,   248,   -33,     1,     2,    20,   -35,    60,
       1,     2,    65,    61,     1,     2,   268,     1,     2,     1,
       2,     1,     2,    82,    11,    40,    86,    55,    88,    40,
       1,     2,    63,   100,   183,   184,   110,    89,    34,    53,
      96,    92,   284,    73,    99,    75,   116,   103,   -35,    74,
     -35,   107,   247,    19,    76,   111,     1,     2,    78,    40,
     119,    58,   121,   131,    85,   117,   266,    55,   183,   184,
     128,    80,   269,    90,   270,   169,   133,   150,  -131,     1,
       2,   129,   132,   134,   149,   156,   124,   125,    95,   131,
     161,   131,   131,   172,  -131,  -131,     1,     2,   289,  -131,
    -131,   292,  -131,  -131,  -131,  -131,  -131,   188,   132,   135,
     132,   132,   173,   185,   177,   178,    91,   136,   137,   217,
     218,    93,   138,   139,   189,   190,    94,     1,     2,   189,
     190,   191,   192,   205,   206,   207,   208,    97,   203,    66,
      67,    68,    69,    70,    71,     1,     2,   213,   220,   221,
     222,   223,   203,   211,   164,   214,   165,   166,    40,   226,
     108,    98,   233,   236,   118,   151,   231,   167,   168,    68,
      69,    70,    71,   122,   169,   123,   203,    40,   240,   152,
     155,   234,   153,   251,   239,   154,    66,    67,    68,    69,
      70,    71,   163,   249,   159,   235,   252,   157,   158,   203,
     160,   250,   162,   264,    66,    67,    68,    69,    70,    71,
     256,   257,   258,   259,   272,   125,   175,   267,   179,   181,
     -97,   276,   -96,   278,   182,   187,   280,   193,   195,   282,
     197,   196,   198,   285,   209,   215,   288,   216,   203,   227,
     228,   230,    24,   294,   272,   296,    25,    26,   245,    27,
      28,    29,    30,    31,    46,    47,    48,    49,    50,   237,
     204,    51,   189,   190,   191,   192,    27,    28,    29,    30,
      31,   241,   242,   244,   243,   253,   254,   255,   261,   277,
     265,   273,   275,   281,   287,   286,   290,    59,   283,   291,
     293,   142,   104,   170,   295,   225,   274,   229
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-195)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_uint16 yycheck[] =
{
       0,    18,   196,     3,     3,     5,   195,     3,     6,     9,
       0,    11,     6,    54,    77,    15,    19,    17,    18,     6,
       3,     6,   161,     6,    65,    54,    55,    28,     4,    32,
     219,    24,    33,    16,    17,    35,   175,   100,    38,     5,
      33,    35,    16,    17,    56,    45,    16,    17,    35,    42,
      48,    63,    46,   242,    48,    54,    55,     6,     6,    52,
      54,    55,    55,    34,    54,    55,   260,    54,    55,    54,
      55,    54,    55,    73,     7,    10,    76,    28,    78,    10,
      54,    55,    33,    95,    19,    20,   103,    80,    21,    33,
      90,    84,   281,     7,    94,     4,   108,    97,    46,    26,
      48,   101,   241,   103,     7,   105,    54,    55,     7,    10,
     110,    44,   112,   125,    26,   108,   255,    28,    19,    20,
     120,    28,   261,     7,   263,    26,   126,   127,    53,    54,
      55,   124,   125,    10,   127,   135,    25,    26,    33,   151,
     140,   153,   154,   155,    52,     4,    54,    55,   287,     8,
       9,   290,    11,    12,    13,    14,    15,     7,   151,    36,
     153,   154,   155,   169,   157,   158,    27,    44,    45,    38,
      39,    27,    49,    50,    29,    30,     7,    54,    55,    29,
      30,    31,    32,   189,   190,   191,   192,     5,   181,    19,
      20,    21,    22,    23,    24,    54,    55,   197,    40,    41,
      42,    43,   195,   196,   151,   198,   153,   154,    10,   209,
      35,    28,   224,   225,     6,    26,   216,    19,    20,    21,
      22,    23,    24,     7,    26,     7,   219,    10,   228,    27,
      33,   224,    28,   245,   227,    28,    19,    20,    21,    22,
      23,    24,    51,   243,     7,    10,   246,    26,    26,   242,
      26,   244,     7,   253,    19,    20,    21,    22,    23,    24,
      29,    30,    31,    32,   264,    26,     5,   260,    22,    26,
       7,   271,     7,   273,    27,     7,   276,     7,     7,   279,
      27,    33,    28,   283,     6,    27,   286,    27,   281,     7,
       5,     7,     4,   293,   294,   295,     8,     9,    46,    11,
      12,    13,    14,    15,    11,    12,    13,    14,    15,    37,
      27,    18,    29,    30,    31,    32,    11,    12,    13,    14,
      15,     5,    26,    33,    27,     6,    27,     5,    47,    53,
       7,     6,     6,    26,     5,    27,     5,    45,    52,     6,
       6,   127,    98,   155,   294,   203,   269,   214
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    54,    55,    57,   107,   108,     0,   107,     3,    58,
     107,    58,     4,   107,   107,     5,    59,    61,    62,   107,
       6,    69,   107,    61,     4,     8,     9,    11,    12,    13,
      14,    15,   101,   103,    58,    60,    16,    17,    70,   104,
      10,   105,   103,   105,   107,   107,    11,    12,    13,    14,
      15,    18,   102,    33,   105,    28,    33,    67,    58,    69,
     105,    34,    96,    33,    67,   105,    19,    20,    21,    22,
      23,    24,    95,     7,    26,     4,     7,    95,     7,    67,
      28,    68,   107,    71,   103,    26,   107,    68,   107,   105,
       7,    27,   105,    27,     7,    33,   107,     5,    28,   107,
      95,    65,    72,   107,    71,    63,    68,   107,    35,    97,
      61,   107,    66,    76,    77,    79,    95,   105,     6,   107,
      64,   107,     7,     7,    25,    26,    73,   107,   107,   105,
      78,    95,   105,   107,    10,    36,    44,    45,    49,    50,
      74,    75,    77,    81,    87,    90,    92,    98,    99,   105,
     107,    26,    27,    28,    28,    33,   107,    26,    26,     7,
      26,   107,     7,    51,    78,    78,    78,    19,    20,    26,
      79,    80,    95,   105,   106,     5,    88,   105,   105,    22,
      73,    26,    27,    19,    20,    80,   105,     7,     7,    29,
      30,    31,    32,     7,    73,     7,    33,    27,    28,   100,
      82,    83,    95,   105,    27,    80,    80,    80,    80,     6,
      82,   105,   106,   107,   105,    27,    27,    38,    39,    84,
      40,    41,    42,    43,    85,    85,   107,     7,     5,   100,
       7,   107,    82,    95,   105,    10,    95,    37,    89,   105,
     107,     5,    26,    27,    33,    46,    91,    73,    82,   107,
     105,    95,   107,     6,    27,     5,    29,    30,    31,    32,
      86,    47,     6,    48,   107,     7,    73,   105,   106,    73,
      73,    93,   107,     6,    91,     6,   107,    53,   107,    94,
     107,    26,   107,    52,    82,   107,    27,     5,   107,    73,
       5,     6,    73,     6,   107,    93,   107
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
/* Line 1792 of yacc.c  */
#line 385 "parser.y"
    {
		method_count = method_count - 1;

			if (method_count == 0) {

			clear_private_methods();
			clear_var_private();

			}
		}
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 413 "parser.y"
    {
				char temp_type[20];
				strcpy(temp_type, "");
				
				if (strcmp((yyvsp[(3) - (7)].str), "int") == 0) {
   				 strcpy(temp_type, "INT_NUM");
				} else if (strcmp((yyvsp[(3) - (7)].str), "double") == 0) {
 				   strcpy(temp_type, "DOUBLE_NUM");
				} else if (strcmp((yyvsp[(3) - (7)].str), "boolean") == 0) {
 				   strcpy(temp_type, "BOOLEAN");
				} else if (strcmp((yyvsp[(3) - (7)].str), "String") == 0) {
 				   strcpy(temp_type, "STRING_VAR");
				} else if (strcmp((yyvsp[(3) - (7)].str), "char") == 0) {
  				  strcpy(temp_type, "CHAR_VAR");
				} else {
 				   strcpy(temp_type, "");
				}
				
				if(strcmp(temp_type, "") == 0) {
				 fprintf(stderr, "Error: Unknown type\n");
				 exit(EXIT_FAILURE);
				} else {
				add_declaredVar((yyvsp[(4) - (7)].str), temp_type); }

				if(declaration_flag == 1) { 
				 for(int i = 0; i < var_num; i++) {
				  add_declaredVar(public_name[i], temp_type); } }
			
				var_num = 0;
				declaration_flag = 0;


				if(var_id_count == 0) {
				add_id((yyvsp[(2) - (7)].str), (yyvsp[(4) - (7)].str));
				
				for (int i = 0; i < 100; i++){
					if (strcmp(ex_table[i].name, "") != 0) {
						add_id((yyvsp[(2) - (7)].str), ex_table[i].name);
					}
				}
				
				}


				if(var_id_count > 0) {
				add_id("method", (yyvsp[(4) - (7)].str));
				
				for (int i = 0; i < 100; i++){
					if (strcmp(ex_table[i].name, "") != 0) {
						add_id("method", ex_table[i].name);
					}
				}
				
				}

				clear_extra();

			}
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 474 "parser.y"
    {		
 							
							 if (strcmp((yyvsp[(6) - (9)].str), "INT_NUM") == 0 | strcmp((yyvsp[(6) - (9)].str), "DOUBLE_NUM") == 0){
			
							 add_assign((yyvsp[(4) - (9)].str), numval);
			
							 }

							
							
							// Add the first variable and its expression to the symbol table
							addVar((yyvsp[(4) - (9)].str), (yyvsp[(6) - (9)].str));
							add_declaredVar((yyvsp[(4) - (9)].str), (yyvsp[(6) - (9)].str));


							if(var_id_count == 0) {
							add_id((yyvsp[(2) - (9)].str), (yyvsp[(4) - (9)].str)); 
				
							for (int b = 0; b < 100; b++){
								if (strcmp(ex_table[b].name, "") != 0) {
								add_id((yyvsp[(2) - (9)].str), ex_table[b].name);
								}
							}

							}



							if(var_id_count > 0) {
							add_id("method", (yyvsp[(4) - (9)].str));
				
								for (int i = 0; i < 100; i++){
									if (strcmp(ex_table[i].name, "") != 0) {
									add_id("method", ex_table[i].name);
									}
								}
				
							}

							clear_extra();

							 
						}
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 518 "parser.y"
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
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 531 "parser.y"
    {

							if(strcmp((yyvsp[(3) - (13)].str), "int") == 0)
								{
									if(strcmp((yyvsp[(6) - (13)].str), "INT_NUM") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
							else if(strcmp((yyvsp[(3) - (13)].str), "double") == 0)
								{
									if(strcmp((yyvsp[(6) - (13)].str), "DOUBLE_NUM") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
							else if(strcmp((yyvsp[(3) - (13)].str), "char") == 0)
								{
									if(strcmp((yyvsp[(6) - (13)].str), "CHAR_VAR") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
							else if(strcmp((yyvsp[(3) - (13)].str), "boolean") == 0)
								{
									if(strcmp((yyvsp[(6) - (13)].str), "BOOLEAN") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
							else if(strcmp((yyvsp[(3) - (13)].str), "String") == 0)
								{
									if(strcmp((yyvsp[(6) - (13)].str), "STRING_VAR") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
						
						}
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 577 "parser.y"
    {
				char temp_type[20];
				strcpy(temp_type, "");
				
				if (strcmp((yyvsp[(2) - (6)].str), "int") == 0) {
   				 strcpy(temp_type, "INT_NUM");
				} else if (strcmp((yyvsp[(2) - (6)].str), "double") == 0) {
 				   strcpy(temp_type, "DOUBLE_NUM");
				} else if (strcmp((yyvsp[(2) - (6)].str), "boolean") == 0) {
 				   strcpy(temp_type, "BOOLEAN");
				} else if (strcmp((yyvsp[(2) - (6)].str), "String") == 0) {
 				   strcpy(temp_type, "STRING_VAR");
				} else if (strcmp((yyvsp[(2) - (6)].str), "char") == 0) {
  				  strcpy(temp_type, "CHAR_VAR");
				} else {
 				   strcpy(temp_type, "");
				}
				
				if(strcmp(temp_type, "") == 0) {
				 fprintf(stderr, "Error: Unknown type\n");
				 exit(EXIT_FAILURE);
				} else {
				add_declaredVar((yyvsp[(3) - (6)].str), temp_type); }

				if(declaration_flag == 1) { 
				 for(int i = 0; i < var_num; i++) {
				  add_declaredVar(public_name[i], temp_type); } }
			
				var_num = 0;
				declaration_flag = 0;


				if(var_id_count == 0) {
				add_id("public", (yyvsp[(3) - (6)].str));
				
				for (int i = 0; i < 100; i++){
					if (strcmp(ex_table[i].name, "") != 0) {
						add_id("public", ex_table[i].name);
					}
				}
				
				}


				if(var_id_count > 0) {
				add_id("method", (yyvsp[(3) - (6)].str));
				
				for (int i = 0; i < 100; i++){
					if (strcmp(ex_table[i].name, "") != 0) {
						add_id("method", ex_table[i].name);
					}
				}
				
				}

				clear_extra();

			}
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 637 "parser.y"
    {		
 							
							 if (strcmp((yyvsp[(5) - (8)].str), "INT_NUM") == 0 | strcmp((yyvsp[(5) - (8)].str), "DOUBLE_NUM") == 0){
			
								add_assign((yyvsp[(3) - (8)].str), numval);
			
							 }
	
							// Add the first variable and its expression to the symbol table
							addVar((yyvsp[(3) - (8)].str), (yyvsp[(5) - (8)].str));
							add_declaredVar((yyvsp[(3) - (8)].str), (yyvsp[(5) - (8)].str));


							if(var_id_count == 0) {
							add_id("public", (yyvsp[(3) - (8)].str)); 
				
							for (int b = 0; b < 100; b++){
								if (strcmp(ex_table[b].name, "") != 0) {
								add_id("public", ex_table[b].name);
								}
							}

							}



							if(var_id_count > 0) {
							add_id("method", (yyvsp[(3) - (8)].str));
				
								for (int i = 0; i < 100; i++){
									if (strcmp(ex_table[i].name, "") != 0) {
									add_id("method", ex_table[i].name);
									}
								}
				
							}

							clear_extra();

							 
						}
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 679 "parser.y"
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
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 692 "parser.y"
    {

							if(strcmp((yyvsp[(2) - (12)].str), "int") == 0)
								{
									if(strcmp((yyvsp[(5) - (12)].str), "INT_NUM") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
							else if(strcmp((yyvsp[(2) - (12)].str), "double") == 0)
								{
									if(strcmp((yyvsp[(5) - (12)].str), "DOUBLE_NUM") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
							else if(strcmp((yyvsp[(2) - (12)].str), "char") == 0)
								{
									if(strcmp((yyvsp[(5) - (12)].str), "CHAR_VAR") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
							else if(strcmp((yyvsp[(2) - (12)].str), "boolean") == 0)
								{
									if(strcmp((yyvsp[(5) - (12)].str), "BOOLEAN") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
							else if(strcmp((yyvsp[(2) - (12)].str), "String") == 0)
								{
									if(strcmp((yyvsp[(5) - (12)].str), "STRING_VAR") != 0)
										{
											fprintf(stderr, "Error: Type mismatch\n");
											exit(EXIT_FAILURE);
										}
								}
						
						}
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 741 "parser.y"
    {
			declaration_flag = 1;
			strcpy(public_name[var_num], (yyvsp[(2) - (3)].str));
			var_num++;

			add_extra((yyvsp[(2) - (3)].str));

		}
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 752 "parser.y"
    {
					
								if (strcmp((yyvsp[(4) - (5)].str), "INT_NUM") == 0 | strcmp((yyvsp[(4) - (5)].str), "DOUBLE_NUM") == 0){
			
									add_assign((yyvsp[(2) - (5)].str), numval);
			
								}
					
					
								// Add extra variables and their assigned expressions to data_table 
								addVar((yyvsp[(2) - (5)].str), (yyvsp[(4) - (5)].str));
								add_declaredVar((yyvsp[(2) - (5)].str), (yyvsp[(4) - (5)].str));
				
								add_extra((yyvsp[(2) - (5)].str));	
					
                          }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 775 "parser.y"
    {	
	add_method((yyvsp[(1) - (10)].str), (yyvsp[(2) - (10)].str), (yyvsp[(3) - (10)].str));
	var_id_count = var_id_count - 1;


	for (int i = 0; i < 100; i++) {
		if (strcmp(id_table[i].modifier, "method") == 0)
		{
        		strcpy(id_table[i].name, "");
			strcpy(id_table[i].modifier, "");
		}
    	}

	}
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 808 "parser.y"
    {
			// Print result of operation as a double
			char *eptr;
			printf("Value of %s is:  %lf \n", (yyvsp[(1) - (4)].str), strtod((yyvsp[(3) - (4)].str), &eptr)); 
	    }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 815 "parser.y"
    {
            if (strcmp((yyvsp[(3) - (4)].str), "INT_NUM") == 0 | strcmp((yyvsp[(3) - (4)].str), "DOUBLE_NUM") == 0){
			
					add_assign((yyvsp[(1) - (4)].str), numval);
			
			}

			
			check_var_private((yyvsp[(1) - (4)].str));

			char* foundVariableType = searchVariable((yyvsp[(1) - (4)].str));
			if(strcmp(foundVariableType, "") == 0)
				{
					fprintf(stderr, "Error: Undeclared variable\n");
					exit(EXIT_FAILURE);
				}
			else
				{
						if(strcmp(foundVariableType, (yyvsp[(3) - (4)].str)) != 0)
							{
								fprintf(stderr, "Error: Type mismatch\n");
								exit(EXIT_FAILURE);
							}
				}
		}
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 848 "parser.y"
    {
	check_method((yyvsp[(1) - (4)].str));
	}
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 863 "parser.y"
    {(yyval.str)=(yyvsp[(2) - (3)].str);}
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 864 "parser.y"
    {double temp; char *endptr1; char *endptr2; temp = strtod((yyvsp[(1) - (3)].str), &endptr1) + strtod((yyvsp[(3) - (3)].str), &endptr2); sprintf((yyval.str), "%lf", temp);}
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 865 "parser.y"
    {double temp; char *endptr1; char *endptr2; temp = strtod((yyvsp[(1) - (3)].str), &endptr1) - strtod((yyvsp[(3) - (3)].str), &endptr2); sprintf((yyval.str), "%lf", temp);}
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 866 "parser.y"
    {double temp; char *endptr1; char *endptr2; temp = strtod((yyvsp[(1) - (3)].str), &endptr1) * strtod((yyvsp[(3) - (3)].str), &endptr2); sprintf((yyval.str), "%lf", temp);}
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 867 "parser.y"
    {double temp; char *endptr1; char *endptr2; temp = strtod((yyvsp[(1) - (3)].str), &endptr1) / strtod((yyvsp[(3) - (3)].str), &endptr2); sprintf((yyval.str), "%lf", temp);}
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 868 "parser.y"
    { strcpy((yyval.str), numval); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 869 "parser.y"
    { strcpy((yyval.str), findOperationValue((yyval.str))); }
    break;


/* Line 1792 of yacc.c  */
#line 2521 "parser.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 966 "parser.y"




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
