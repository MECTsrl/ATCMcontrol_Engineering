%n 2000
%e 8000
%p 15000
%{

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include <cstdlib>
#include <iostream>

using namespace std;

#include "Grammar.h"

#define exit throw_fatal_error

static int inside_system_comment = 0;

void throw_fatal_error(int i);
void setlexval(yy_scan* scanner);
/* void setstringlexval(yy_scan* scanner);*/
void setlexval_without_newline(yy_scan* scanner);
void setlexval(const string& text, StringPos end, yy_scan* scanner);
void st_comment(yy_scan*);
void native_block(yy_scan*);
void consume_until(yy_scan*, const string& s, char* errMsg);
void consume_comment(yy_scan* scanner);
void set_variable_comment_lexval(yy_scan* scanner);
void string_literal(yy_scan*, int delim);

%}

letter                  [A-Za-z]
alphanum                [A-Za-z0-9]
digit                   [0-9]
octal_digit             [0-7]
hex_digit               [0-9A-Fa-f]
bit                     [01]
identifier              ({letter}|_)(_|{alphanum})*
integer                 {digit}(_?{digit})*
binary_integer          2#{bit}(_?{bit})*
octal_integer           8#{octal_digit}(_?{octal_digit})*
hex_integer             16#{hex_digit}(_?{hex_digit})*
real                    {integer}\.{integer}([Ee][+-]?{integer})?
fixed_point             {integer}(\.{integer})?
/*
string                  \"[^\"\n]*\"
*/

year                    {integer}
month                   {integer}
day                     {integer}
date_literal            {year}-{month}-{day}

date                    ([Dd][Aa][Tt][Ee]|D|d)#{date_literal}

day_hour                {integer}
day_minute              {integer}
day_second              {fixed_point}
daytime                 {day_hour}:{day_minute}:{day_second}

date_and_time           ([Dd][Aa][Tt][Ee]_[Aa][Nn][Dd]_[Tt][Ii][Mm][Ee]|[Dd][Tt])#{date_literal}-{daytime}
time_of_day             ([Tt][Ii][Mm][Ee]_[Oo][Ff]_[Dd][Aa][Yy]|[Tt][Oo][Dd])#{daytime}

direct_field            (({alphanum}|_)+|["]([^"]*)["])|"$"({alphanum}|_)+
direct_variable         "%"{alphanum}*("*+"|"*."|"*")?({direct_field}(\.{direct_field})*)?(!{alphanum}+)*

days                    ({fixed_point}[dD]|{integer}[dD]_?{hours})
hours                   ({fixed_point}[hH]|{integer}[hH]_?{minutes})
minutes                 ({fixed_point}[mM]|{integer}[mM]_?{seconds})
seconds                 ({fixed_point}[sS]|{integer}[sS]_?{milliseconds})
milliseconds            {fixed_point}(ms|MS|Ms|mS)

interval                ({days}|{hours}|{minutes}|{seconds}|{milliseconds})

duration                (T|t|[Tt][Ii][Mm][Ee])#-?{interval}

int_type_name           ([Ss][Ii][Nn][Tt]|[Ii][Nn][Tt]|[Dd][Ii][Nn][Tt]|[Ll][Ii][Nn][Tt]|[Uu][Ss][Ii][Nn][Tt]|[Uu][Ii][Nn][Tt]|[Uu][Dd][Ii][Nn][Tt]|[Uu][Ll][Ii][Nn][Tt])
bit_type_name           ([Bb][Oo][Oo][Ll]|[Bb][Yy][Tt][Ee]|[Ww][Oo][Rr][Dd]|[Dd][Ww][Oo][Rr][Dd]|[Ll][Ww][Oo][Rr][Dd])
real_type_name          ([Rr][Ee][Aa][Ll]|[Ll][Rr][Ee][Aa][Ll])

int_or_bit_literal      ((({int_type_name}|{bit_type_name})#[+-]?)?({integer}|{binary_integer}|{octal_integer}|{hex_integer}))
real_literal            (({real_type_name}#[+-]?)?{real})

lit_true                ([Bb][Oo][Oo][Ll]#)?[Tt][Rr][Uu][Ee]
lit_false               ([Bb][Oo][Oo][Ll]#)?[Ff][Aa][Ll][Ss][Ee]


/* === TODO: not used anymore ? */
%start ST_COMMENT NATIVE_BLOCK
/* === END OF TODO */

%%


/* === system comments ============================================ */
"]]*)"                   { inside_system_comment = 0; setlexval(this); return SYSTEM_COMMENT_END; }
"(*[["                   { inside_system_comment = 1; setlexval(this); return SYSTEM_COMMENT_BEGIN; }
/* === end of system comments ===================================== */

/* === st variable comments ================================================ */
"(*["                    { set_variable_comment_lexval(this); return VARIABLE_COMMENT; }
/* === end of st variable comments ========================================= */

/* === st comments ================================================ */
"(*"                     { st_comment(this); }
/* === end of st comments ========================================= */

/* === native blocks ============================================== */
/* are treated like comments, since they are only ignored by backparsers */
[Nn][Aa][Tt][Ii][Vv][Ee] { native_block(this); }
/* === end of native blocks ======================================= */

/* === string literals ================================================ */
"\""                     { string_literal(this, '"');  return STRING_LITERAL; }
"'"                      { string_literal(this, '\''); return STRING_LITERAL; }
/* === end of st comments ========================================= */


/* === keywords =================================================== */
/*
[Tt][Rr][Uu][Ee]                                                        { setlexval(this); return                 TRUE; }
[Ff][Aa][Ll][Ss][Ee]                                                    { setlexval(this); return                FALSE; }
*/
[Mm][Oo][Dd]                                                            { setlexval(this); return                  MOD; }
[Aa][Nn][Dd]                                                            { setlexval(this); return                  AND; }
[Oo][Rr]                                                                { setlexval(this); return                   OR; }
[Xx][Oo][Rr]                                                            { setlexval(this); return                  XOR; }
[Nn][Oo][Tt]                                                            { setlexval(this); return                  NOT; }
[Cc][Oo][Nn][Ff][Ii][Gg][Uu][Rr][Aa][Tt][Ii][Oo][Nn]                    { setlexval(this); return        CONFIGURATION; }
[Ee][Nn][Dd]_[Cc][Oo][Nn][Ff][Ii][Gg][Uu][Rr][Aa][Tt][Ii][Oo][Nn]       { setlexval(this); return    END_CONFIGURATION; }
[Rr][Ee][Ss][Oo][Uu][Rr][Cc][Ee]                                        { setlexval(this); return             RESOURCE; }
[Ee][Nn][Dd]_[Rr][Ee][Ss][Oo][Uu][Rr][Cc][Ee]                           { setlexval(this); return         END_RESOURCE; }
[Oo][Nn]                                                                { setlexval(this); return                   ON; }
[Ww][Ii][Tt][Hh]                                                        { setlexval(this); return                 WITH; }
[Tt][Aa][Ss][Kk]                                                        { setlexval(this); return                 TASK; }
[Tt][Yy][Pp][Ee]                                                        { setlexval(this); return                 TYPE; }
[Ee][Nn][Dd]_[Tt][Yy][Pp][Ee]                                           { setlexval(this); return             END_TYPE; }
[Ss][Tt][Rr][Uu][Cc][Tt]                                                { setlexval(this); return               STRUCT; }
[Ee][Nn][Dd]_[Ss][Tt][Rr][Uu][Cc][Tt]                                   { setlexval(this); return           END_STRUCT; }
[Pp][Rr][Oo][Gg][Rr][Aa][Mm]                                            { setlexval(this); return              PROGRAM; }
[Ee][Nn][Dd]_[Pp][Rr][Oo][Gg][Rr][Aa][Mm]                               { setlexval(this); return          END_PROGRAM; }
[Ff][Uu][Nn][Cc][Tt][Ii][Oo][Nn]                                        { setlexval(this); return             FUNCTION; }
[Ee][Nn][Dd]_[Ff][Uu][Nn][Cc][Tt][Ii][Oo][Nn]                           { setlexval(this); return         END_FUNCTION; }
[Ff][Uu][Nn][Cc][Tt][Ii][Oo][Nn]_[Bb][Ll][Oo][Cc][Kk]                   { setlexval(this); return       FUNCTION_BLOCK; }
[Ee][Nn][Dd]_[Ff][Uu][Nn][Cc][Tt][Ii][Oo][Nn]_[Bb][Ll][Oo][Cc][Kk]      { setlexval(this); return   END_FUNCTION_BLOCK; }
[Ss][Ii][Nn][Tt]                                                        { setlexval(this); return                 SINT; }
[Ii][Nn][Tt]                                                            { setlexval(this); return            TOKEN_INT; }
[Dd][Ii][Nn][Tt]                                                        { setlexval(this); return                 DINT; }
[Ll][Ii][Nn][Tt]                                                        { setlexval(this); return                 LINT; }
[Uu][Ss][Ii][Nn][Tt]                                                    { setlexval(this); return                USINT; }
[Uu][Ii][Nn][Tt]                                                        { setlexval(this); return           TOKEN_UINT; }
[Uu][Dd][Ii][Nn][Tt]                                                    { setlexval(this); return                UDINT; }
[Uu][Ll][Ii][Nn][Tt]                                                    { setlexval(this); return                ULINT; }
[Rr][Ee][Aa][Ll]                                                        { setlexval(this); return                 REAL; }
[Ll][Rr][Ee][Aa][Ll]                                                    { setlexval(this); return                LREAL; }
[Dd][Aa][Tt][Ee]                                                        { setlexval(this); return           TOKEN_DATE; }
[Tt][Ii][Mm][Ee]_[Oo][Ff]_[Dd][Aa][Yy]                                  { setlexval(this); return          TIME_OF_DAY; }
[Tt][Oo][Dd]                                                            { setlexval(this); return                  TOD; }
[Dd][Aa][Tt][Ee]_[Aa][Nn][Dd]_[Ti][Ii][Mm][Ee]                          { setlexval(this); return        DATE_AND_TIME; }
[Dd][Tt]                                                                { setlexval(this); return                   DT; }
[Ss][Tt][Rr][Ii][Nn][Gg]                                                { setlexval(this); return               STRING; }
[Ww][Ss][Tt][Rr][Ii][Nn][Gg]                                            { setlexval(this); return              WSTRING; }
[Tt][Ii][Mm][Ee]                                                        { setlexval(this); return                 TIME; }
[Bb][Oo][Oo][Ll]                                                        { setlexval(this); return           TOKEN_BOOL; }
[Bb][Yy][Tt][Ee]                                                        { setlexval(this); return           TOKEN_BYTE; }
[Ww][Oo][Rr][Dd]                                                        { setlexval(this); return           TOKEN_WORD; }
[Dd][Ww][Oo][Rr][Dd]                                                    { setlexval(this); return          TOKEN_DWORD; }
[Ll][Ww][Oo][Rr][Dd]                                                    { setlexval(this); return                LWORD; }
[Aa][Nn][Yy]                                                            { setlexval(this); return                  ANY; }
[Aa][Nn][Yy]_[Nn][Uu][Mm]                                               { setlexval(this); return              ANY_NUM; }
[Aa][Nn][Yy]_[Rr][Ee][Aa][Ll]                                           { setlexval(this); return             ANY_REAL; }
[Aa][Nn][Yy]_[Ii][Nn][Tt]                                               { setlexval(this); return              ANY_INT; }
[Aa][Nn][Yy]_[Bb][Ii][Tt]                                               { setlexval(this); return              ANY_BIT; }
[Aa][Nn][Yy]_[Ss][Tt][Rr][Ii][Nn][Gg]                                   { setlexval(this); return           ANY_STRING; }
[Aa][Nn][Yy]_[Dd][Aa][Tt][Ee]                                           { setlexval(this); return             ANY_DATE; }
[Aa][Nn][Yy]_[Ee][Ll][Ee][Mm][Ee][Nn][Tt][Aa][Rr][Yy]                   { setlexval(this); return       ANY_ELEMENTARY; }
[Aa][Nn][Yy]_[Mm][Aa][Gg][Nn][Ii][Tt][Uu][Dd][Ee]                       { setlexval(this); return        ANY_MAGNITUDE; }
[Aa][Nn][Yy]_[Dd][Ee][Rr][Ii][Vv][Ee][Dd]                               { setlexval(this); return          ANY_DERIVED; }
[Aa][Tt]                                                                { setlexval(this); return                   AT; }
[Aa][Rr][Rr][Aa][Yy]                                                    { setlexval(this); return                ARRAY; }
[Ee][Nn]                                                                { setlexval(this); return                   EN; }
[Ee][Nn][Oo]                                                            { setlexval(this); return                  ENO; }
[Rr]_[Ee][Dd][Gg][Ee]                                                   { setlexval(this); return               R_EDGE; }
[Ff]_[Ee][Dd][Gg][Ee]                                                   { setlexval(this); return               F_EDGE; }
[Vv][Aa][Rr]                                                            { setlexval(this); return                  VAR; }
[Vv][Aa][Rr]_[Ii][Nn][Pp][Uu][Tt]                                       { setlexval(this); return            VAR_INPUT; }
[Vv][Aa][Rr]_[Oo][Uu][Tt][Pp][Uu][Tt]                                   { setlexval(this); return           VAR_OUTPUT; }
[Vv][Aa][Rr]_[Ii][Nn]_[Oo][Uu][Tt]                                      { setlexval(this); return           VAR_IN_OUT; }
[Vv][Aa][Rr]_[Ee][Xx][Tt][Ee][Rr][Nn][Aa][Ll]                           { setlexval(this); return         VAR_EXTERNAL; }
[Vv][Aa][Rr]_[Aa][Cc][Cc][Ee][Ss][Ss]                                   { setlexval(this); return           VAR_ACCESS; }
[Vv][Aa][Rr]_[Gg][Ll][Oo][Bb][Aa][Ll]                                   { setlexval(this); return           VAR_GLOBAL; }
[Vv][Aa][Rr]_[Tt][Ee][Mm][Pp]                                           { setlexval(this); return             VAR_TEMP; }
[Vv][Aa][Rr]_[Ii][Nn]_[Ee][Xx][Tt]                                      { setlexval(this); return           VAR_IN_EXT; }
[Ee][Nn][Dd]_[Vv][Aa][Rr]                                               { setlexval(this); return              END_VAR; }
[Rr][Ee][Tt][Aa][Ii][Nn]                                                { setlexval(this); return               RETAIN; }
[Nn][Oo][Nn]_[Rr][Ee][Tt][Aa][Ii][Nn]                                   { setlexval(this); return           NON_RETAIN; }
[Cc][Oo][Nn][Ss][Tt][Aa][Nn][Tt]                                        { setlexval(this); return             CONSTANT; }
[Vv][Aa][Rr]_[Cc][Oo][Nn][Ff][Ii][Gg]                                   { setlexval(this); return           VAR_CONFIG; }
[Rr][Ee][Aa][Dd]_[Oo][Nn][Ll][Yy]                                       { setlexval(this); return            READ_ONLY; }
[Rr][Ee][Aa][Dd]_[Ww][Rr][Ii][Tt][Ee]                                   { setlexval(this); return           READ_WRITE; }
[Ss][Tt][Ee][Pp]                                                        { setlexval(this); return                 STEP; }
[Ee][Nn][Dd]_[Ss][Tt][Ee][Pp]                                           { setlexval(this); return             END_STEP; }
[Ii][Nn][Ii][Tt][Ii][Aa][Ll]_[Ss][Tt][Ee][Pp]                           { setlexval(this); return         INITIAL_STEP; }
[Tt][Rr][Aa][Nn][Ss][Ii][Tt][Ii][Oo][Nn]                                { setlexval(this); return           TRANSITION; }
[Ee][Nd][Dd]_[Tt][Rr][Aa][Nn][Ss][Ii][Tt][Ii][Oo][Nn]                   { setlexval(this); return       END_TRANSITION; }
[Aa][Cc][Tt][Ii][Oo][Nn]                                                { setlexval(this); return               ACTION; }
[Ee][Nn][Dd]_[Aa][Cc][Tt][Ii][Oo][Nn]                                   { setlexval(this); return           END_ACTION; }
[Ff][Rr][Oo][Mm]                                                        { setlexval(this); return                 FROM; }
[Ii][Ff]                                                                { setlexval(this); return                   IF; }
[Tt][Hh][Ee][Nn]                                                        { setlexval(this); return                 THEN; }
[Ee][Ll][Ss][Ii][Ff]                                                    { setlexval(this); return                ELSIF; }
[Ee][Ll][Ss][Ee]                                                        { setlexval(this); return                 ELSE; }
[Ee][Nn][Dd]_[Ii][Ff]                                                   { setlexval(this); return               END_IF; }
[Cc][Aa][Ss][Ee]                                                        { setlexval(this); return                 CASE; }
[Oo][Ff]                                                                { setlexval(this); return                   OF; }
[Ee][Nn][Dd]_[Cc][Aa][Ss][Ee]                                           { setlexval(this); return             END_CASE; }
[Ww][Hh][Ii][Ll][Ee]                                                    { setlexval(this); return                WHILE; }
[Dd][Oo]                                                                { setlexval(this); return                   DO; }
[Ee][Nn][Dd]_[Ww][Hh][Ii][Ll][Ee]                                       { setlexval(this); return            END_WHILE; }
[Re][Er][Pp][Ee][Aa][Tt]                                                { setlexval(this); return               REPEAT; }
[Uu][Nn][Tt][Ii][Ll]                                                    { setlexval(this); return                UNTIL; }
[Ee][Nn][Dd]_[Re][Er][Pp][Ee][Aa][Tt]                                   { setlexval(this); return           END_REPEAT; }
[Ff][Oo][Rr]                                                            { setlexval(this); return                  FOR; }
[Tt][Oo]                                                                { setlexval(this); return                   TO; }
[Bb][Yy]                                                                { setlexval(this); return                   BY; }
[Ee][Nn][Dd]_[Ff][Oo][Rr]                                               { setlexval(this); return              END_FOR; }
[Ee][Xx][Ii][Tt]                                                        { setlexval(this); return                 EXIT; }
[Rr][Ee][Tt][Uu][Rr][Nn]                                                { setlexval(this); return               RETURN; }
[Gg][Oo][Tt][Oo]                                                        { setlexval(this); return                 GOTO; }
[Ss][Yy][Nn][Cc][Hh][Rr][Oo][Nn][Ii][Zz][Ee][Dd]                        { setlexval(this); return         SYNCHRONIZED; }
[Ee][Nn][Dd]_[Ss][Yy][Nn][Cc][Hh][Rr][Oo][Nn][Ii][Zz][Ee][Dd]           { setlexval(this); return     END_SYNCHRONIZED; }
[Vv][Aa][Rr]_[Nn][Aa][Tt][Ii][Vv][Ee]                                   { native_block(this); return           VAR_NATIVE; }
[Cc][Oo][Nn][Ss][Tt]                                                    { setlexval(this); return          TOKEN_CONST; }
[Ee][Nn][Dd]_[Cc][Oo][Nn][Ss][Tt]                                       { setlexval(this); return      TOKEN_END_CONST; }
[Qq][Vv][Tt]_[Bb][Oo][Oo][Ll]                                           { setlexval(this); return             QVT_BOOL; }
[Qq][Vv][Tt]_[Bb][Yy][Tt][Ee]                                           { setlexval(this); return             QVT_BYTE; }
[Qq][Vv][Tt]_[Ww][Oo][Rr][Dd]                                           { setlexval(this); return             QVT_WORD; }
[Qq][Vv][Tt]_[Dd][Ww][Oo][Rr][Dd]                                       { setlexval(this); return            QVT_DWORD; }
[Qq][Vv][Tt]_[Dd][Ii][Nn][Tt]                                           { setlexval(this); return             QVT_DINT; }
[Qq][Vv][Tt]_[Ll][Rr][Ee][Aa][Ll]                                       { setlexval(this); return            QVT_LREAL; }
/* === end of keywords =============================================== */


"+"                     { setlexval(this); return PLUS; }
"-"                     { setlexval(this); return MINUS; }
"*"                     { setlexval(this); return TIMES; }
"/"                     { setlexval(this); return DIV; }
"**"                    { setlexval(this); return POW; }
"&"                     { setlexval(this); return AND; }
"="                     { setlexval(this); return EQ; }
"<>"                    { setlexval(this); return NE; }
"<"                     { setlexval(this); return LT; }
"<="                    { setlexval(this); return LE; }
">"                     { setlexval(this); return GT; }
">="                    { setlexval(this); return GE; }
":"                     { setlexval(this); return COLON; }
":="                    { setlexval(this); return BECOMES; }
"=>"                    { setlexval(this); return RIGHT_ARROW; }
";"                     { setlexval(this); return SEMICOLON; }
"."                     { setlexval(this); return DOT; }
".."                    { setlexval(this); return DOTDOT; }
","                     { setlexval(this); return COMMA; }
"["                     { setlexval(this); return SQUARED_BRACKET_OPEN; }
"]"                     { setlexval(this); return SQUARED_BRACKET_CLOSE; }
"("                     { setlexval(this); return PAREN_OPEN; }
")"                     { setlexval(this); return PAREN_CLOSE; }
"{"                     { setlexval(this); return CURLY_BRACES_OPEN; }
"}"                     { setlexval(this); return CURLY_BRACES_CLOSE; }
"|"                     { setlexval(this); return PIPE; }
"..."                   { setlexval(this); return PERIOD; }

/* compiler directives */
#[Ii][Mm][Pp][Oo][Rr][Tt]                                       { setlexval(this); return            IMPORT_DIRECTIVE; }
/*
#[Ii][Mm][Pp][Oo][Rr][Tt].*$ { setlexval_without_newline(this); return IMPORT_DIRECTIVE; }
*/
#[Pp][Rr][Aa][Gg][Mm][Aa].*$ { setlexval_without_newline(this); return PRAGMA_DIRECTIVE; }
/* end of directives */

/* regular expressions */
{identifier}             { setlexval(this); return IDENTIFIER; }
{int_or_bit_literal}     { setlexval(this); return INT_LITERAL; }
{real_literal}           { setlexval(this); return FLOAT_LITERAL; }
{date}                   { setlexval(this); return DATE_LITERAL; }
{date_and_time}          { setlexval(this); return DT_LITERAL; }
{time_of_day}            { setlexval(this); return TOD_LITERAL; }
{duration}               { setlexval(this); return TIME_LITERAL; }

{lit_true}               { setlexval(this); return BOOL_LITERAL; }
{lit_false}              { setlexval(this); return BOOL_LITERAL; }

{direct_variable}        { setlexval(this); return DIRECT_VARIABLE; } 
/*
{string}                 { setstringlexval(this); return STRING_LITERAL; } 
*/

/* end of regular expressions */

[ \t\r\n]                
.                        


%%

void st_comment(yy_scan* scanner)
{
//    consume_until(scanner, "*)", "end of file in comment"); // Does not support nested comments.
    consume_comment(scanner);                             // Supports nested comments.
}

void native_block(yy_scan* scanner)
{
    consume_until(scanner, "END_NATIVE", "end of file in native block");
}

void consume_until(yy_scan* scanner, const string& s, char* errMsg)
{
    //=== consume input characters until string s matches case insensitive

    //=== look for the first char of string s at beginning
    string::size_type i = 0;

    //=== consume input characters until:
    //=== - end of file is reached or
    //=== - last character of string s did match
    for( int c = scanner->input();
         c != EOF && i < s.length();
         c = scanner->input())
    {
        //=== compare i-th character case insensitive
        if ( (c == tolower(s.at(i))) ||
             (c == toupper(s.at(i))) 
           )
        {
            //=== if matched continue loop
            //=== but try to match character i+1 in string s next time
           ++i;
        }
        //=== if i-th character did not match but a non empty
        //=== prefix of string s did match already ...
        else if ( i > 0 )
        {
            //=== then push back consumed characters
            for( int j = i; j > 0; --j )
            {
                scanner->unput(s.at(j));
            }
            //=== retry match with first character of string s next time
            i = 0;
        }
    }

    //=== if not all characters of string s did match
    //=== then eof appeared
    if ( i < s.length() )
    {
        scanner->yyerror(errMsg);
    }
}

void consume_comment(yy_scan* scanner)
{
    int comments = 1;
    int c_old = 0;
    int c = scanner->input();
    while (c != EOF)
    {
        if ((c_old == '(') && (c == '*'))
        {
            comments++;
        }
        else if ((c_old == '*') && (c == ')'))
        {
            comments--;
        }
        if (comments == 0)
        {
            end_of_last_comment = scanner->yypos;
            break;
        }
        c_old = c;
        c = scanner->input();
    }
    if (c == EOF)
    {
        scanner->yyerror("end of file in comment");
    }
}

void set_variable_comment_lexval(yy_scan* scanner)
{
    int comments = 1;
    int c_old = 0;
    int c_old_old = 0;
    string var_comment = "";
    int c = scanner->input();
    while (c != EOF)
    {
        var_comment += c;
        if ((c_old_old == '(') && (c_old == '*') && (c == '['))
        {
            comments++;
        }
        else if ((c_old_old == ']') && (c_old == '*') && (c == ')'))
        {
            comments--;
        }
        if (comments == 0)
        {
            break;
        }
        c_old_old = c_old;
        c_old = c;
        c = scanner->input();
    }
    if (c == EOF)
    {
        scanner->yyerror("end of file in comment");
    }
    else
    {
        var_comment = var_comment.substr(0, var_comment.length() - 3);   /* ]*) -> 3 */
        setlexval(var_comment, scanner->yypos, scanner);
    }
}

void setlexval(yy_scan* scanner)
{
    string text = scanner->yytext;
    setlexval(text, scanner->yypos - 1, scanner);
}

void setlexval_without_newline(yy_scan* scanner)
{
    string text = scanner->yytext;
    setlexval(text, scanner->yypos - 1, scanner);
}

void setlexval(const string& text, StringPos end, yy_scan* scanner)
{
    LexVal *lv;
    lv = new LexVal(text, end, scanner->yylineno);

    // insert lv at the front of the list of lexvals 
    lv->m_pPrevious = scanner->yylval.lexval;
    scanner->yylval.lexval = lv;
}

void string_literal(yy_scan* scanner, int delim)
{
    string literal;
    literal = delim;
    int dollar = 0;
    int c = scanner->input();
    while (c != EOF && c != '\n')
    {
        literal += c;

        if ((c == delim) && (!dollar))
        {
            break;
        }
        else if (c == '$' && inside_system_comment == 0)
        {
            dollar = !dollar;
        }
        else 
        {
            dollar = 0;
        }

        c = scanner->input();
    }
    if (c == EOF)
    {
        scanner->yyerror("end of file in string literal");
    }
    else
    {
        setlexval(literal, scanner->yypos, scanner);
    }
}

void throw_fatal_error(int i)
{
    throw "FATAL SCANNER FAILURE !!!";
}

