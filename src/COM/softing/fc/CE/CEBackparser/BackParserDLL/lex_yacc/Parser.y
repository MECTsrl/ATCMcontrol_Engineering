/*
================================================================================
================================================================================
 declarations and definitions
================================================================================
================================================================================
*/
%{
// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include <iostream>

using namespace std;

#include "Global.h"
#include "Grammar.h"

#define exit throw_fatal_error

void throw_fatal_error(int i);
%}

/*
================================================================================
================================================================================
 yylval type definition
================================================================================
================================================================================
*/
%union 
{
    LexVal     *lexval;
    SysComment *syscmnt;
};

/*
================================================================================
================================================================================
 tokens
================================================================================
================================================================================
*/
%token <lexval>  SYSTEM_COMMENT_BEGIN SYSTEM_COMMENT_END

%token <lexval>  VARIABLE_COMMENT

%token <lexval>  VAR_GLOBAL END_VAR
%token <lexval>  TOKEN_CONST TOKEN_END_CONST
%token <lexval>  TYPE END_TYPE STRUCT END_STRUCT
%token <lexval>  CONFIGURATION END_CONFIGURATION
%token <lexval>  RESOURCE ON END_RESOURCE WITH TASK
%token <lexval>  PROGRAM END_PROGRAM FUNCTION_BLOCK END_FUNCTION_BLOCK FUNCTION END_FUNCTION
%token <lexval>  TYPE END_TYPE

%token <lexval>  SINT TOKEN_INT DINT LINT USINT TOKEN_UINT UDINT ULINT REAL LREAL TOKEN_DATE TIME_OF_DAY TOD
%token <lexval>  DATE_AND_TIME DT STRING WSTRING TIME TOKEN_BOOL TOKEN_BYTE TOKEN_WORD TOKEN_DWORD LWORD

%token <lexval>  AT EN ENO R_EDGE F_EDGE
%token <lexval>  VAR VAR_INPUT VAR_OUTPUT VAR_IN_OUT VAR_EXTERNAL VAR_ACCESS VAR_TEMP VAR_IN_EXT
%token <lexval>  VAR_CONFIG READ_ONLY READ_WRITE

%token <lexval>  STEP END_STEP INITIAL_STEP TRANSITION END_TRANSITION ACTION END_ACTION

%token <lexval>  FROM IF THEN ELSE ELSIF END_IF CASE END_CASE WHILE DO END_WHILE REPEAT UNTIL
%token <lexval>  END_REPEAT FOR TO BY END_FOR

%token <lexval>  EXIT RETURN GOTO

%token <lexval>  SYNCHRONIZED END_SYNCHRONIZED VAR_NATIVE END_NATIVE

%token <lexval>  QVT_BOOL QVT_BYTE QVT_WORD QVT_DWORD QVT_DINT QVT_LREAL CURL

%token <lexval>  SEMICOLON COLON COMMA DOT DOTDOT EQ NE LT LE GT GE MOD AND OR XOR NOT POW
%token <lexval>  PLUS MINUS TIMES DIV BECOMES RIGHT_ARROW PERIOD PIPE

%token <lexval>  FILENAME

%token <lexval>  IDENTIFIER

%token <lexval>  DT_LITERAL TOD_LITERAL INT_LITERAL DATE_LITERAL TOKEN_TRUE TOKEN_FALSE
%token <lexval>  PAREN_OPEN PAREN_CLOSE SQUARED_BRACKET_OPEN SQUARED_BRACKET_CLOSE CURLY_BRACES_OPEN CURLY_BRACES_CLOSE
%token <lexval>  TIME_LITERAL BOOL_LITERAL FLOAT_LITERAL STRING_LITERAL

/* %token <lexval>  NATIVE */
/* native blocks are ignored by backparsers !!! */

%token <lexval>  ARRAY OF

%token <lexval>  TOKEN_INT DINT LREAL TOKEN_DATE TIME_OF_DAY DATE_AND_TIME
%token <lexval>  TIME TOKEN_BOOL TOKEN_BYTE TOKEN_WORD TOKEN_DWORD SINT LINT USINT
%token <lexval>  TOKEN_UINT UDINT ULINT REAL LWORD STRING WSTRING
%token <lexval>  ANY ANY_NUM ANY_REAL ANY_INT ANY_BIT ANY_STRING ANY_DATE ANY_ELEMENTARY
%token <lexval>  ANY_MAGNITUDE ANY_DERIVED

%token <lexval>  QVT_BOOL QVT_BYTE QVT_WORD QVT_DWORD QVT_DINT QVT_LREAL

%token <lexval>  DIRECT_VARIABLE

%token <lexval>  RETAIN NON_RETAIN CONSTANT

%token <lexval>  IMPORT_DIRECTIVE PRAGMA_DIRECTIVE

%left OR
%left XOR
%left AND
%left EQ NE
%left LT LE GT GE
%left PLUS MINUS
%left TIMES DIV MOD
%left POW
%left NOT

%type <syscmnt> opt_sys_comment

%type <lexval> syscmt_attr
%type <lexval> syscmt_val
%type <lexval> any_literal signed_lit_or_const
%type <lexval> opt_var_modifier
%type <lexval> local_decl_scope
%type <lexval> function_name function_name_h
%type <lexval> simple_type generic_type qvt_type
%type <lexval> opt_edge_detection
%type <lexval> variable_name any_direct_var
%type <lexval> type_name basic_type simple_type generic_type qvt_type string_type
%type <lexval> var_declsection_end var_modifiers var_modifier

%%

/*
================================================================================
================================================================================
 grammar rules
================================================================================
================================================================================
*/

iec_source_file 
   : opt_sys_comment {SetFileSysComment($1);} opt_pou_declaration
   ;

/*
================================================================================
 pou declarations
================================================================================
*/
opt_pou_declaration
  : configuration_declaration
  | resource_declaration
  | program_declaration
  | functionblock_declaration
  | function_declaration
  | data_type_declaration
  | const_section_list
  | { emptyfile(); }
  | error { yyclearin(); YYRETURN(pou_error());}
  ;

/*
================================================================================
 configurations
================================================================================
*/
configuration_declaration
    : CONFIGURATION IDENTIFIER                  { 
                                                  configuration_begin($1, $2); 
                                                  set_next_impdir_insert_pos($2);
                                                }              
      opt_var_decl_sections                    
      END_CONFIGURATION                         { configuration_clear($5); }
    ;

/*
================================================================================
 resources
================================================================================
*/
resource_declaration
    : RESOURCE IDENTIFIER                       { resource_begin($1, $2); 
                                                  set_next_impdir_insert_pos($2);
                                                  resoure_set_next_taskconf_insert_pos($2);
                                                  resoure_set_next_progconf_insert_pos($2); 
                                                } 
      opt_target             
      opt_var_decl_sections                    
      opt_task_configuration_list               
      opt_program_configuration_list
      END_RESOURCE                              { resource_clear(); }
    ;
opt_target
    : ON IDENTIFIER                             { resource_set_target($2); 
                                                  set_next_impdir_insert_pos($2);
                                                  resoure_set_next_taskconf_insert_pos($2);
                                                  resoure_set_next_progconf_insert_pos($2); 
                                                }

    |
    ;

/*
================================================================================
 task lists
================================================================================
*/
opt_task_configuration_list 
    : task_configuration_list
    | /* NIL */              
    ;

task_configuration_list
    : task_configuration                                
    | task_configuration_list task_configuration                   
    ;

task_configuration
    : TASK IDENTIFIER                                       { task_configuration_begin($1, $2); }
      opt_sys_comment                                       { task_configuration_set_sys_comment($4); }  
      PAREN_OPEN opt_task_initialisation_list PAREN_CLOSE   { task_configuration_initialisation_list_range($6, $8); } 
      SEMICOLON                                             { task_configuration_clear($10);
                                                              resoure_set_next_taskconf_insert_pos($10); 
                                                              resoure_set_next_progconf_insert_pos($10); 
                                                            }
      ;

opt_task_initialisation_list
    : task_initialisation_list
    | /* NIL */               
    ;

task_initialisation_list
    : task_initialisation_list COMMA task_initialisation
    | task_initialisation
    ;

task_initialisation
    : IDENTIFIER BECOMES expression             { task_configuration_initialisation($1, $2); }
    ;

/*
================================================================================
 program lists
================================================================================
*/
opt_program_configuration_list 
 : program_configuration_list
 | /* NIL */
 ;

program_configuration_list
 : program_configuration SEMICOLON                              { resoure_set_next_progconf_insert_pos($2); program_configuration_clear($2); }
 | program_configuration_list program_configuration SEMICOLON   { resoure_set_next_progconf_insert_pos($3); program_configuration_clear($3); }
 ;

program_configuration
 : PROGRAM opt_var_modifier                     
   IDENTIFIER                                   { program_configuration_begin($1, $3); }
   opt_sys_comment                              { program_configuration_set_sys_comment($5); }  
   opt_task_name 
   COLON                                        
   IDENTIFIER                                   { program_configuration_set_program_class($9); }
   opt_conf_elements
 ;

opt_task_name 
 : WITH                                         
   IDENTIFIER                                   { program_configuration_set_task($2); }
 |               
 ;

opt_conf_elements
 : PAREN_OPEN conf_elements PAREN_CLOSE 
 | /* NIL */
 ;

conf_elements
 : conf_element
 | conf_elements COLON conf_element
 ;

conf_element
 : IDENTIFIER WITH IDENTIFIER
 | IDENTIFIER BECOMES expression
 | IDENTIFIER RIGHT_ARROW expression
 ;

/*
================================================================================
 programs
================================================================================
*/
program_declaration
   : PROGRAM IDENTIFIER				   { program_begin($1, $2); SuppressError();}
     opt_var_decl_sections { program_clear();} opt_end_program 
   ;
opt_end_program
   : END_PROGRAM
   |
   ;
/*
================================================================================
 functionblocks
================================================================================
*/
functionblock_declaration
   : FUNCTION_BLOCK IDENTIFIER                { fb_begin($1, $2); SuppressError();}
      opt_var_decl_sections { fb_clear();} opt_end_function_block 
   ;
opt_end_function_block
   : END_FUNCTION_BLOCK
   |
   ;
/*
================================================================================
 functions
================================================================================
*/
function_declaration
   : FUNCTION function_name_h { function_begin($1, $2); SuppressError();}
     opt_func_type_name
     opt_var_decl_sections { function_clear(); } opt_end_function 
   ;
opt_end_function
   : END_FUNCTION
   |
   ;
function_name_h
   : function_name
   | NOT
   ;
function_name
   : IDENTIFIER
   | MOD
   | AND
   | OR
   | XOR
   ;
opt_func_type_name
   : COLON type_name { function_set_function_type($2); }
   | /* NIL */
   ;

/*
================================================================================
 structured data types
================================================================================
*/
data_type_declaration
   : TYPE IDENTIFIER                        { struct_begin($1, $2); }
     COLON
     opt_var_decl_sections 
     SEMICOLON
     END_TYPE                               { struct_clear(); }
   ;

/*
================================================================================
 constants
================================================================================
*/
const_section_list
 :                              { globvarconstlist_begin(); } 
   var_decl_section_list        { globvarconstlist_end(); 
                                  globvarconstlist_clear(); } 
 ;
 
/*
================================================================================
 system comments
================================================================================
*/
opt_sys_comment
   : SYSTEM_COMMENT_BEGIN { syscmnt_begin($1); }
     opt_syscmt_attrval_list
     SYSTEM_COMMENT_END { $$ = syscmnt_end($4); }
   | { $$ = syscmnt_clear(); }
   ;
opt_syscmt_attrval_list
   : syscmt_attrval_list 
   | syscmt_attrval_list COMMA
   | /* NIL */
   ;
syscmt_attrval_list
   :                           syscmt_attrval_assign
   | syscmt_attrval_list COMMA syscmt_attrval_assign
   ;
syscmt_attrval_assign
   : syscmt_attr EQ syscmt_val { syscmnt_add_av($1, $3); }
   ;
syscmt_attr
   : IDENTIFIER
   ;
syscmt_val
   : any_literal
   | IDENTIFIER
   | /* NIL */ { $$ = 0; }
   ;
   
/*
================================================================================
 literals
================================================================================
*/
any_literal 
   : INT_LITERAL            
   | FLOAT_LITERAL             
   | TOKEN_TRUE                 
   | TOKEN_FALSE                
   | DATE_LITERAL              
   | DT_LITERAL                
   | TOD_LITERAL                
   | TIME_LITERAL               
   | STRING_LITERAL
   | BOOL_LITERAL
   ;
signed_lit_or_const
 : any_literal
 | PLUS any_literal
 | MINUS any_literal
 | IDENTIFIER
 | PLUS IDENTIFIER
 | MINUS IDENTIFIER
 ;

/*
================================================================================
 types
================================================================================
*/
type_name
   : basic_type { $$ = $1; }
   | IDENTIFIER { $$ = $1; }
   | ARRAY opt_array_dim_list OF basic_type { extract_array_type($1, $4);  
                                              $$ = $4; }
   | ARRAY opt_array_dim_list OF IDENTIFIER { $$ = $1; }
   ;
opt_array_dim_list
   : SQUARED_BRACKET_OPEN
     array_dim_list 
     SQUARED_BRACKET_CLOSE
   | /* NIL */
   ;
array_dim_list
   : expression DOTDOT expression
   | array_dim_list COMMA expression DOTDOT expression
   ;
basic_type
   : simple_type  { $$ = $1; }
   | generic_type { $$ = $1; }
   | qvt_type     { $$ = $1; }
   | string_type  { $$ = $1; }
   ;
string_type
   : STRING { $$ = $1;; }
   | STRING SQUARED_BRACKET_OPEN expression SQUARED_BRACKET_CLOSE { $$ = $1; }
   | WSTRING { $$ = $1; }
   | WSTRING SQUARED_BRACKET_OPEN expression SQUARED_BRACKET_CLOSE { $$ = $1; }
   ;
simple_type
   : TOKEN_INT     { $$ = $1; }
   | DINT          { $$ = $1; }
   | LREAL         { $$ = $1; }
   | TOKEN_DATE    { $$ = $1; }
   | TIME_OF_DAY   { $$ = $1; }
   | TOD           { $$ = $1; }
   | DATE_AND_TIME { $$ = $1; }
   | DT            { $$ = $1; }
   | TIME          { $$ = $1; }
   | TOKEN_BOOL    { $$ = $1; }
   | TOKEN_BYTE    { $$ = $1; }
   | TOKEN_WORD    { $$ = $1; }
   | TOKEN_DWORD   { $$ = $1; }
   | SINT          { $$ = $1; }
   | LINT          { $$ = $1; }
   | USINT         { $$ = $1; }
   | TOKEN_UINT    { $$ = $1; }
   | UDINT         { $$ = $1; }
   | ULINT         { $$ = $1; }
   | REAL          { $$ = $1; }
   | LWORD         { $$ = $1; }
   ;
 generic_type
   : ANY            { $$ = $1; }
   | ANY_NUM        { $$ = $1; }
   | ANY_REAL       { $$ = $1; }
   | ANY_INT        { $$ = $1; }
   | ANY_BIT        { $$ = $1; }
   | ANY_STRING     { $$ = $1; }
   | ANY_DATE       { $$ = $1; }
   | ANY_ELEMENTARY { $$ = $1; }
   | ANY_MAGNITUDE  { $$ = $1; }
   | ANY_DERIVED    { $$ = $1; }
   ;
 qvt_type
   : QVT_BOOL  { $$ = $1; }
   | QVT_BYTE  { $$ = $1; }
   | QVT_WORD  { $$ = $1; }
   | QVT_DWORD { $$ = $1; }
   | QVT_DINT  { $$ = $1; }
   | QVT_LREAL { $$ = $1; }
   ;
/*
================================================================================
 expressions
================================================================================
*/
expression
   : bin_expression
   | MINUS expression %prec NOT
   | PLUS expression %prec NOT
   | NOT expression
   | primary_expression
   ;
bin_expression
   : expression OR    expression
   | expression XOR   expression
   | expression AND   expression
   | expression EQ    expression
   | expression NE    expression
   | expression LT    expression
   | expression LE    expression
   | expression GT    expression
   | expression GE    expression
   | expression PLUS  expression
   | expression MINUS expression
   | expression TIMES expression
   | expression DIV   expression
   | expression MOD   expression
   | expression POW   expression
   ;
primary_expression 
   : any_literal 
   | variable
   | paren_expression
   | function_name paren_expression_list
   | qvt_type      paren_expression_list
   ;
paren_expression
   : PAREN_OPEN expression PAREN_CLOSE
   ;
paren_expression_list
   : PAREN_OPEN opt_expression_list PAREN_CLOSE
   ;
opt_expression_list 
   : expression_list
   | /* NIL */
   ;
expression_list
 : expression
 | expression_list COMMA expression
 ;

/*
================================================================================
 variable declaration sections
================================================================================
*/
opt_var_decl_sections
 : var_decl_section_list
 | /* NIL */
 ;
var_decl_section_list
 : var_decl_section 
 | var_decl_section_list var_decl_section
 ;
 var_decl_section
 : local_decl_scope opt_var_modifier { vardeclsect_begin($1, $2); }
   opt_var_decl_list 
   var_declsection_end               { vardeclsect_end($5); }
 | IMPORT_DIRECTIVE  STRING_LITERAL                { import_directive($2); 
                                       set_next_impdir_insert_pos($2);
                                       resoure_set_next_taskconf_insert_pos($2);
                                       resoure_set_next_progconf_insert_pos($2); 
                                     }
 ;
var_declsection_end
 : END_VAR                           { $$ = $1; }
 | END_STRUCT                        { $$ = $1; }
 | TOKEN_END_CONST                   { $$ = $1; }
 | END_NATIVE                        { $$ = $1; }
 ;
opt_var_decl_list
 : var_decl_list  
 | /* NIL */
 ;
var_decl_list
 : var_decl
 | var_decl_list var_decl
 ;
var_decl
 : var_decl_head 
   var_decl_tail 
   SEMICOLON             { vardecl_end_without_varcomment($3); } 
   opt_var_comment       { vardecl_clear(); }
 ;
var_decl_head
 : var_list      COLON   { varlistelem_end($2); }
 | var_list_elem AT      { varlistelem_end($2); }
   any_direct_var  COLON { varlistelem_address($4); }
/*
=== NOTE: direct variables without symbolic name are not supported !!!
 | AT DIRECT_VARIABLE COLON
=== END NOTE:
 */
 ;

any_direct_var  /* for Osai... */
 : DIRECT_VARIABLE       { $$ = $1; }
 | IDENTIFIER            { $$ = $1; }   
 ;

var_decl_tail
 : type_name opt_edge_detection opt_var_modifier opt_initializer { vardecl_type($1); }
 ;
var_list
 : var_list_elem
 | var_list COMMA { varlistelem_end($2); } var_list_elem
 ;
var_list_elem
 : IDENTIFIER { varlistelem_begin($1); } opt_sys_comment {  varlistelem_syscomment($3); }
 ;
local_decl_scope
   : VAR
   | VAR_INPUT
   | VAR_OUTPUT
   | VAR_IN_OUT
   | VAR_EXTERNAL
   | VAR_TEMP
   | VAR_IN_EXT
   | STRUCT
   | TOKEN_CONST                { globvarconstlist_set_poutype(_CST_FILE); }
   | VAR_GLOBAL                 { globvarconstlist_set_poutype(_GVL_FILE); }
   | VAR_NATIVE
   ;
opt_var_modifier
   : var_modifiers      
   | /* NIL */          { $$ = 0; }
   ;
var_modifiers
   : var_modifier                
   | var_modifiers var_modifier   
   ;
var_modifier
   : RETAIN
   | NON_RETAIN
   | CONSTANT       { $$ = 0; }
   ;
opt_edge_detection
 : F_EDGE               { vardecl_edge($1); } 
 | R_EDGE               { vardecl_edge($1); } 
 | /* NIL */            { $$ = 0; }
 ; 

opt_var_comment
 :  VARIABLE_COMMENT    { vardecl_var_comment($1); vardecl_end($1);}
 |
 ;
/*
================================================================================
 variables
================================================================================
*/
variable
 : DIRECT_VARIABLE
 | symbolic_variable
 ;
symbolic_variable
 : variable_name 
 | multi_element_variable
 ;
variable_name
 : IDENTIFIER
 ;
multi_element_variable
 : array_variable
 | structured_variable
 ;
array_variable
 : symbolic_variable SQUARED_BRACKET_OPEN expression_list SQUARED_BRACKET_CLOSE
   
 ;
structured_variable
 : symbolic_variable DOT IDENTIFIER 
 ; 

/*
================================================================================
 initializers
================================================================================
*/
opt_initializer
 : BECOMES                  { initializer_begin($1); }
   initializer              { initializer_clear(); }
 | /* NIL */
 ;

initializer
 : expression
 | SQUARED_BRACKET_OPEN array_initial_element_list SQUARED_BRACKET_CLOSE
 | paren_struct_init
 ;

paren_struct_init
 : PAREN_OPEN struct_init PAREN_CLOSE
 ;

struct_init
 :  IDENTIFIER BECOMES initializer
 |  struct_init COMMA IDENTIFIER BECOMES initializer
 ;

array_initial_element_list
 : array_initial_element
 | array_initial_element_list COMMA array_initial_element 
 ;

array_initial_element
 : init_simple
 | signed_lit_or_const                                 
   paren_array_initial_element_list
 | paren_expression    
   paren_array_initial_element_list
 ;

paren_array_initial_element_list
 : PAREN_OPEN opt_array_initial_elements PAREN_CLOSE    
 ;

opt_array_initial_elements
 : array_initial_element_list
 | /* NIL */
 ;

init_simple
 : signed_lit_or_const         
 | paren_expression             
 | paren_struct_init            
 ;
%%
