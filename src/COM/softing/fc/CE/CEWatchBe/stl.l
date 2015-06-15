
 /* Structured Text Grammar according to IEC 1131-3 (corrigenda 3 included) */
%n 1000
%e 4000
%p 6000
%{
#include "stdafx.h"
#include <ctype.h>
#include "fc_todebug\fc_assert.h"
//#include "fc_todebugto_mem.h"
#include "st_scan.h"

#define TOKENBUFSIZE 256
#pragma warning (disable: 4068 4102 4244)

//foreward:
static void STS_IncPos (stl_scan *pScanner,int iIncOffset);

%}

%x RANGES

letter                  [A-Za-z]
alphanum                [A-Za-z0-9]
digit                   [0-9]
octal_digit             [0-7]
hex_digit               [0-9A-Fa-f]
bit                     [01]
identifier              ({letter}|_)({alphanum}|_)*
integer                 {digit}(_?{digit})*
binary_integer          2#{bit}(_?{bit})*
octal_integer           8#{octal_digit}(_?{octal_digit})*
hex_integer             16#{hex_digit}(_?{hex_digit})*
real                    {integer}\.{integer}([Ee][+-]?{integer})?
fixed_point             {integer}(\.{integer})?
number                  {digit}{digit}*

direct_variable         "%"[IQM][XBWDL]?{integer}(\.{integer})*(\.{wcharacter_string})?

character_representation ([^$'\n]|"$"({hex_digit}{hex_digit}|[$'LlNnPpRrTt]))
character_string        "'"{character_representation}*"'"

wcharacter_representation ([^$"\n]|"$"({hex_digit}{hex_digit}{hex_digit}{hex_digit}|[$"LlNnPpRrTt]))
wcharacter_string       ["]{wcharacter_representation}*["]

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


days                    ({fixed_point}[dD]|{integer}[dD]_?{hours})
hours                   ({fixed_point}[hH]|{integer}[hH]_?{minutes})
minutes                 ({fixed_point}[mM]|{integer}[mM]_?{seconds})
seconds                 ({fixed_point}[sS]|{integer}[sS]_?{milliseconds})
milliseconds            {fixed_point}(ms|MS|mS|Ms)

interval                ({days}|{hours}|{minutes}|{seconds}|{milliseconds})

duration                (T|t|[Tt][Ii][Mm][Ee])#-?{interval}


int_or_bit_type_name   ([Ii][Nn][Tt]|[Uu][Ii][Nn][Tt]|[Dd][Ii][Nn][Tt]|[Uu][Dd][Ii][Nn][Tt]|[Ll][Ii][Nn][Tt]|[Bb][Oo][Oo][Ll]|[Bb][Yy][Tt][Ee]|[Ww][Oo][Rr][Dd]|[Dd][Ww][Oo][Rr][Dd]|[Ll][Ww][Oo][Rr][Dd])
real_type_name         [Ll][Rr][Ee][Aa][Ll]

int_or_bit_literal     (({int_or_bit_type_name}#[+-]?)?({integer}|{binary_integer}|{octal_integer}|{hex_integer}))
real_literal           (({real_type_name}#[+-]?)?{real})

machine_address        "{"[^\}]*"}"

%%


<RANGES>number { STS_IncPos (this, yyleng); return (STS_RANGES_NUMBER); }
<RANGES>".."   { STS_IncPos (this, yyleng); return (STS_DOTDOT);        }
<RANGES>","    { STS_IncPos (this, yyleng); return (',');               }
<RANGES>.      { return (STS_SYNTAX_ERROR); }


[Tt][Rr][Uu][Ee]                       { STS_IncPos(this,yyleng); return STS_TRUE_LITERAL;}
[Bb][Oo][Oo][Ll]#[Tt][Rr][Uu][Ee]      { STS_IncPos(this,yyleng); return STS_TRUE_LITERAL;}
[Ff][Aa][Ll][Ss][Ee]                   { STS_IncPos(this,yyleng); return STS_FALSE_LITERAL;}
[Bb][Oo][Oo][Ll]#[Ff][Aa][Ll][Ss][Ee]  { STS_IncPos(this,yyleng); return STS_FALSE_LITERAL;}
"+"                     { STS_IncPos(this,yyleng); return '+';}
"-"                     { STS_IncPos(this,yyleng); return '-';}
"*"                     { STS_IncPos(this,yyleng); return '*';}
"/"                     { STS_IncPos(this,yyleng); return '/';}
[Mm][Oo][Dd]            { STS_IncPos(this,yyleng); return STS_MOD;}
"**"                    { STS_IncPos(this,yyleng); return STS_POW;}
[Aa][Nn][Dd]            { STS_IncPos(this,yyleng); return STS_AND;}
[Oo][Rr]                { STS_IncPos(this,yyleng); return STS_OR;}
[Xx][Oo][Rr]            { STS_IncPos(this,yyleng); return STS_XOR;}
[Nn][Oo][Tt]            { STS_IncPos(this,yyleng); return STS_NOT;}
"&"                     { STS_IncPos(this,yyleng); return '&';}
[Ee][Qq]                { STS_IncPos(this,yyleng); return STS_EQ;}
[Nn][Ee]                { STS_IncPos(this,yyleng); return STS_NE;}
[Ll][Tt]                { STS_IncPos(this,yyleng); return STS_LT;}
[Ll][Ee]                { STS_IncPos(this,yyleng); return STS_LE;}
[Gg][Tt]                { STS_IncPos(this,yyleng); return STS_GT;}
[Gg][Ee]                { STS_IncPos(this,yyleng); return STS_GE;}
"="                     { STS_IncPos(this,yyleng); return STS_EQ_SHORT;}
"<>"                    { STS_IncPos(this,yyleng); return STS_NE_SHORT;}
"<"                     { STS_IncPos(this,yyleng); return STS_LT_SHORT;}
"<="                    { STS_IncPos(this,yyleng); return STS_LE_SHORT;}
">"                     { STS_IncPos(this,yyleng); return STS_GT_SHORT;}
">="                    { STS_IncPos(this,yyleng); return STS_GE_SHORT;}
"."                     { STS_IncPos(this,yyleng); return '.';}
","                     { STS_IncPos(this,yyleng); return ',';}
"["                     { STS_IncPos(this,yyleng); return '[';}
"]"                     { STS_IncPos(this,yyleng); return ']';}
"("                     { STS_IncPos(this,yyleng); return '(';}
")"                     { STS_IncPos(this,yyleng); return ')';}
[Aa][Dd][Dd]            { STS_IncPos(this,yyleng); return (STS_ADD);}
[Ss][Uu][Bb]            { STS_IncPos(this,yyleng); return (STS_SUB);}
[Mm][Uu][Ll]            { STS_IncPos(this,yyleng); return (STS_MUL);}
[Dd][Ii][Vv]            { STS_IncPos(this,yyleng); return (STS_DIV);}
[Bb][Yy][Tt][Ee]_[Tt][Oo]_[Dd][Ii][Nn][Tt]            { STS_IncPos(this,yyleng); return (STS_BYTE_TO_DINT); }
[Bb][Yy][Tt][Ee]_[Tt][Oo]_[Dd][Ww][Oo][Rr][Dd]        { STS_IncPos(this,yyleng); return (STS_BYTE_TO_DWORD); }
[Bb][Yy][Tt][Ee]_[Tt][Oo]_[Ll][Rr][Ee][Aa][Ll]        { STS_IncPos(this,yyleng); return (STS_BYTE_TO_LREAL); }
[Bb][Yy][Tt][Ee]_[Tt][Oo]_[Ww][Oo][Rr][Dd]            { STS_IncPos(this,yyleng); return (STS_BYTE_TO_WORD); }
[Dd][Ii][Nn][Tt]_[Tt][Oo]_[Bb][Yy][Tt][Ee]            { STS_IncPos(this,yyleng); return (STS_DINT_TO_BYTE); }
[Dd][Ii][Nn][Tt]_[Tt][Oo]_[Dd][Ww][Oo][Rr][Dd]        { STS_IncPos(this,yyleng); return (STS_DINT_TO_DWORD); }
[Dd][Ii][Nn][Tt]_[Tt][Oo]_[Ll][Rr][Ee][Aa][Ll]        { STS_IncPos(this,yyleng); return (STS_DINT_TO_LREAL); }
[Dd][Ii][Nn][Tt]_[Tt][Oo]_[Tt][Ii][Mm][Ee]            { STS_IncPos(this,yyleng); return (STS_DINT_TO_TIME); }
[Dd][Ii][Nn][Tt]_[Tt][Oo]_[Ww][Oo][Rr][Dd]            { STS_IncPos(this,yyleng); return (STS_DINT_TO_WORD); }
[Dd][Ww][Oo][Rr][Dd]_[Tt][Oo]_[Bb][Yy][Tt][Ee]        { STS_IncPos(this,yyleng); return (STS_DWORD_TO_BYTE); }
[Dd][Ww][Oo][Rr][Dd]_[Tt][Oo]_[Dd][Ii][Nn][Tt]        { STS_IncPos(this,yyleng); return (STS_DWORD_TO_DINT); }
[Dd][Ww][Oo][Rr][Dd]_[Tt][Oo]_[Ll][Rr][Ee][Aa][Ll]    { STS_IncPos(this,yyleng); return (STS_DWORD_TO_LREAL); }
[Dd][Ww][Oo][Rr][Dd]_[Tt][Oo]_[Ww][Oo][Rr][Dd]        { STS_IncPos(this,yyleng); return (STS_DWORD_TO_WORD); }
[Ll][Rr][Ee][Aa][Ll]_[Tt][Oo]_[Bb][Yy][Tt][Ee]        { STS_IncPos(this,yyleng); return (STS_LREAL_TO_BYTE); }
[Ll][Rr][Ee][Aa][Ll]_[Tt][Oo]O_[Dd][Ii][Nn][Tt]       { STS_IncPos(this,yyleng); return (STS_LREAL_TO_DINT); }
[Ll][Rr][Ee][Aa][Ll]_[Tt][Oo]_[Dd][Ww][Oo][Rr][Dd]    { STS_IncPos(this,yyleng); return (STS_LREAL_TO_DWORD); }
[Ll][Rr][Ee][Aa][Ll]_[Tt][Oo]_[Ww][Oo][Rr][Dd]        { STS_IncPos(this,yyleng); return (STS_LREAL_TO_WORD); }
[Tt][Ii][Mm][Ee]_[Tt][Oo]_[Dd][Ii][Nn][Tt]            { STS_IncPos(this,yyleng); return (STS_TIME_TO_DINT); }
[Ww][Oo][Rr][Dd]_[Tt][Oo]_[Bb][Yy][Tt][Ee]            { STS_IncPos(this,yyleng); return (STS_WORD_TO_BYTE); }
[Ww][Oo][Rr][Dd]_[Tt][Oo]_[Dd][Ii][Nn][Tt]            { STS_IncPos(this,yyleng); return (STS_WORD_TO_DINT); }
[Ww][Oo][Rr][Dd]_[Tt][Oo]_[Dd][Ww][Oo][Rr][Dd]        { STS_IncPos(this,yyleng); return (STS_WORD_TO_DWORD); }
[Ww][Oo][Rr][Dd]_[Tt][Oo]_[Ll][Rr][Ee][Aa][Ll]        { STS_IncPos(this,yyleng); return (STS_WORD_TO_LREAL); }

/* regular expressions */
{identifier}             { STS_IncPos(this,yyleng); return STS_IDENTIFIER; }
{direct_variable}        { STS_IncPos(this,yyleng); return STS_DIRECT_VARIABLE;}
{int_or_bit_literal}     { STS_IncPos(this,yyleng); return STS_INT_LITERAL;}
{real_literal}           { STS_IncPos(this,yyleng); return STS_FLOAT_LITERAL;}
{character_string}       { STS_IncPos(this,yyleng); return STS_STRING_LITERAL;}
{wcharacter_string}      { STS_IncPos(this,yyleng); return STS_WSTRING_LITERAL;}
{date}                   { STS_IncPos(this,yyleng); return STS_DATE_LITERAL;}
{date_and_time}          { STS_IncPos(this,yyleng); return STS_DT_LITERAL;}
{time_of_day}            { STS_IncPos(this,yyleng); return STS_TOD_LITERAL;}
{duration}               { STS_IncPos(this,yyleng); return STS_TIME_LITERAL;}
{machine_address}        { STS_IncPos(this,yyleng); return STS_MACHINE_ADDRESS;}
/* end of regular expressions */

[ \t]                    { STS_IncPos(this,1); /* white space */}
[\n]                     { STS_IncPos(this,2); /* white space */}

.                       return STS_SYNTAX_ERROR;
%%


class sts_scanner : public stl_scan
{
public:
    LPCTSTR m_pszInput;     //string buffer as input
    unsigned long m_ulLen;  //length of the buffer in bytes
    unsigned long m_ulRead; //current read position
	LONG m_lColumn;         //column
    LONG m_lOffset;         //absolute file offset position
	TCHAR m_chLast;		    //last character

    sts_scanner() : stl_scan(TOKENBUFSIZE)
    { 
        m_lColumn = 0L;
        m_chLast = 0;
        m_lOffset = 0L;
        m_pszInput = NULL;
        m_ulLen = 0L;
        m_ulRead = 0L;
	}
    void setStartState(int i){this->yy_start = i;}
	virtual int yygetc();
	virtual void yyerror(char *fmt, ...);
};

void sts_scanner::yyerror(char *fmt, ...)
{
	FC_DEBUGPRINT1(_T("CEPARSE> %d: Token buffer overflow"),yylineno);
}

sts_scanner::yygetc()
{
	int ch;

    if(m_pszInput) {             //##d-1618 cast unsigned!  
        ch = (m_ulRead<m_ulLen ? (unsigned char)m_pszInput[m_ulRead++] : EOF);

        if(ch == _T('\r')) { //skip the '\r'
            ch = yygetc();            
        }
    }
    else {
        ch = stl_scan::yygetc();
    }

	if(m_chLast == YYNEWLINE) {
		m_lColumn = 0L;
	}
	m_lColumn++;

	m_chLast = ch;
	return ch;		
}

 
sts_scanner *STS_CreateScanner ()
{
    sts_scanner *pScanner;

    pScanner = new sts_scanner;
    assert (pScanner != NULL);
    return pScanner;
}

void STS_DeleteScanner (sts_scanner *pScanner)
{
    delete (pScanner);
}

LONG STS_Column (const sts_scanner *pScanner)
{
    return (pScanner->m_lColumn);
}

LONG STS_GetOffset (const sts_scanner *pScanner)
{
    return (pScanner->m_lOffset);
}

LONG STS_GetStartOffset (const sts_scanner *pScanner)
{
    return (pScanner->m_lOffset - pScanner->yyleng);
}

void STS_IncPos (stl_scan *pScanner, int iIncOffset)
{
    sts_scanner *psc;
    
    psc = (sts_scanner *)pScanner;
    psc->m_lOffset += iIncOffset;
}

void STS_SetInputString (sts_scanner *pScanner, const TCHAR *pszInput, unsigned long ulLen)
{
    assert (pszInput != NULL);

    pScanner->m_pszInput = pszInput;
    pScanner->m_ulLen = ulLen;
}

int STS_NextToken (sts_scanner *pScanner, const char **pszToken)
{
	int iToken;
    
    iToken = pScanner->yylex ();
	*pszToken = pScanner->yytext;
	return (iToken);
}

int  STS_SetScannerMode(sts_scanner *pScanner, STS_ScannerMode mode)
{
    switch (mode)
    {
    case STS_EXPRESSION_SYNTAX:
        pScanner->setStartState(0);
        break;

    case STS_RANGES_SYNTAX:
        pScanner->setStartState (RANGES);
        break;

    default:
        return (0);
    };

    return (1);
}

/* end of lex file */
