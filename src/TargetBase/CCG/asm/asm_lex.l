/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asm_lex.l 2     17.04.07 16:28 Ef $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: asm_lex.l $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/asm_lex.l $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			ASM
 *
 * =CURRENT 	 $Date: 17.04.07 16:28 $
 *			 $Revision: 2 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ------------------------------------------------------------------------------
 * Change Log
 *
 * Ref.		Date	 Author	Comment
 *
 * ##
 * ------------------------------------------------------------------------------
 */




 /* Structured Text Grammar according to IEC 1131-3 (corrigenda 3 included) */
%n 1000
%e 4000
%p 5000
%{
#yy_movetop #include <ctype.h>
#yy_movetop #include "fc_todebug\fc_assert.h"
#yy_movetop #include "fc_todebug\fc_mem.h"
#yy_movetop #include "asm_lex.h"
#yy_movetop #pragma warning( disable : 4068 4102 4244)
#yy_movetop //foreward
#yy_movetop class ASM_scanner;
%}
letter                  [A-Za-z]
alphanum                [A-Za-z0-9]
digit                   [0-9]
octal_digit             [0-7]
hex_digit               [0-9A-Fa-f]
bit                     [01]
identifierEx            ({letter}|_|@|§)(@|§|_|{alphanum})*
integer                 {digit}(_?{digit})*
lit_true                true
lit_false               false
binary_integer          2#{bit}(_?{bit})*
octal_integer           8#{octal_digit}(_?{octal_digit})*
hex_integer             16#{hex_digit}(_?{hex_digit})*
real                    {integer}\.{integer}([Ee][+-]?{integer})?
fixed_point             {integer}(\.{integer})?
any_unsigned            (0[Xx]{hex_digit}{hex_digit}*|{integer})
offset_lit              offsetof\([ \t\r]*({identifierEx}[ \t\r]*[,])?([ \t\r]*{identifierEx})[ \t\r]*\)
objid_lit               objid\([ \t\r]*{identifierEx}[ \t\r]*\)

year                    {integer}
month                   {integer}
day                     {integer}
date_literal            {year}-{month}-{day}

date                    ([Dd][Aa][Tt][Ee]|D|d)#{date_literal}

day_hour                {integer}
day_minute              {integer}
day_second              {fixed_point}
daytime                 {day_hour}:{day_minute}:{day_second}

date_and_time           (DATE_AND_TIME|date_and_time|[Dd][Tt])#{date_literal}-{daytime}
time_of_day             (TIME_OF_DAY|time_of_day|TOD|tod)#{daytime}

direct_field            (({alphanum}|_)+|["]([^"]*)["])
direct_variable         "%"{alphanum}*("*+"|"*."|"*")?({direct_field}(\.{direct_field})*)?(!{alphanum}+)*


days                    ({fixed_point}[dD]|{integer}[dD]_?{hours})
hours                   ({fixed_point}[hH]|{integer}[hH]_?{minutes})
minutes                 ({fixed_point}[mM]|{integer}[mM]_?{seconds})
seconds                 ({fixed_point}[sS]|{integer}[sS]_?{milliseconds})
milliseconds            {fixed_point}(ms|MS|Ms|mS)

interval                ({days}|{hours}|{minutes}|{seconds}|{milliseconds})

duration                ([Tt]|[Tt][Ii][Mm][Ee])#-?{interval}

int_type_name           ([Ss][Ii][Nn][Tt]|[Ii][Nn][Tt]|[Dd][Ii][Nn][Tt]|[Ll][Ii][Nn][Tt]|[Uu][Ss][Ii][Nn][Tt]|[Uu][Ii][Nn][Tt]|[Uu][Dd][Ii][Nn][Tt]|[Uu][Ll][Ii][Nn][Tt])
bit_type_name           ([Bb][Oo][Oo][Ll]|[Bb][Yy][Tt][Ee]|[Ww][Oo][Rr][Dd]|[Dd][Ww][Oo][Rr][Dd]|[Ll][Ww][Oo][Rr][Dd])
real_type_name          ([Rr][Ee][Aa][Ll]|[Ll][Rr][Ee][Aa][Ll])

int_or_bit_literal      ((({int_type_name}|{bit_type_name})#[+-]?)?({integer}|{binary_integer}|{octal_integer}|{hex_integer}))
real_literal            (({real_type_name}#[+-]?)?{real})

%%
"+"                     return '+';
"-"                     return '-';
"*"                     return '*';
"/"                     return '/';
"="                     return '=';
":"                     return ':';
":="                    return ASM_TOK_BECOMES;
";"                     return ';';
"."                     return '.';
","                     return ',';
"["                     return '[';
"]"                     return ']';
"("                     return '(';
")"                     return ')';
"{"                     return '{';
"}"                     return '}';
"|"                     return '|';
">"                     return '>';
"<"                     return '<';
["']                    return ASM_TOK_STRING_LITERAL_BEGIN;


^[ \t\r]*\.target          return ASM_TOK_TARGET;
^[ \t\r]*\.fun             return ASM_TOK_FUN;
^[ \t\r]*\.fb              return ASM_TOK_FB;
^[ \t\r]*\.prg             return ASM_TOK_PRG;
^[ \t\r]*\.struct          return ASM_TOK_STRUCT;
^[ \t\r]*\.resource        return ASM_TOK_RESOURCE;
^[ \t\r]*\.task            return ASM_TOK_TASK;
^[ \t\r]*\.global          return ASM_TOK_GLOBAL;
^[ \t\r]*\.global_retain   return ASM_TOK_GLOBAL_RETAIN;
^[ \t\r]*\.config          return ASM_TOK_CONFIG;
^[ \t\r]*\.copy            return ASM_TOK_COPY;
^[ \t\r]*\.channels        return ASM_TOK_CHANNELS;
^[ \t\r]*\.channel         return ASM_TOK_CHANNEL;
^[ \t\r]*\.end             return ASM_TOK_END_SECTION;

^[ \t\r]*clssid            return ASM_TOK_CLSSID;
^[ \t\r]*var               return ASM_TOK_VAR;
^[ \t\r]*in                return ASM_TOK_IN;
^[ \t\r]*inext             return ASM_TOK_INEXT;
^[ \t\r]*out               return ASM_TOK_OUT;
^[ \t\r]*inout             return ASM_TOK_INOUT;
^[ \t\r]*align8            return ASM_TOK_ALIGN8;
^[ \t\r]*bp                return ASM_TOK_BP;
^[ \t\r]*stmtbeg           return ASM_TOK_STMTBEG;
^[ \t\r]*"ST>"             return ASM_TOK_ST_INFO;
^[ \t\r]*jmp               return ASM_TOK_JMP;
^[ \t\r]*jmpc              return ASM_TOK_JMPC;
^[ \t\r]*jmpcn             return ASM_TOK_JMPCN;
^[ \t\r]*call              return ASM_TOK_CALL;
^[ \t\r]*read              return ASM_TOK_READ;
^[ \t\r]*write             return ASM_TOK_WRITE;
^[ \t\r]*{identifierEx}    return ASM_TOK_CALOP;



/* regular expressions */
{lit_true}              return ASM_TOK_LITERAL;
{lit_false}             return ASM_TOK_LITERAL;
{int_or_bit_literal}    return ASM_TOK_LITERAL;
{real_literal}          return ASM_TOK_LITERAL;
{date}                  return ASM_TOK_LITERAL;
{date_and_time}         return ASM_TOK_LITERAL;
{time_of_day}           return ASM_TOK_LITERAL;
{duration}              return ASM_TOK_LITERAL;
{offset_lit}            return ASM_TOK_LITERAL_OFFSET;
{objid_lit}             return ASM_TOK_LITERAL_OBJID;
{any_unsigned}          return ASM_TOK_ANY_UNSIGNED;
{direct_variable}       return ASM_TOK_DIRECT_VARIABLE; 
{identifierEx}          return ASM_TOK_IDENTIFIER_EX; 
[\n]                    return '\n';
[ \t\r]                 return -10;

.                        return ASM_TOK_SYNTAX_ERROR;
%%






class ASM_Scanner: public asm_lex_scan
{
public:    
    const TCHAR* m_pszInput;  //string buffer as input
    unsigned long m_ulLen;    //length buffer in bytes
    unsigned long m_ulRead;   //current read position in buffer
    long          m_lOffset;  //absolute file offset of last token
    long          m_yylengOld;//size in byte of last token
	int			  m_nColumn;  //current column

    ASM_Scanner() : asm_lex_scan(ASM_MAX_TOKEN_LENGTH)
    {
        m_pszInput=NULL; 
        m_ulLen=0; 
        m_ulRead =0;
        m_lOffset = 0;
        m_yylengOld = 0;
		m_nColumn = 1;
    }
    void setStartState(int i){this->yy_start = i;}
    virtual int	yygetc()
    { 
        return m_pszInput ? (m_ulRead<m_ulLen ? m_pszInput[m_ulRead++] : EOF) : getc(yyin);
    }

    virtual void yyerror(char *fmt, ...)
    {
        //the following call are made to yyerror:
        //yyerror("Bad space for scanner!");
        //yyerror("No space for scanner!");
        //yyerror("Token buffer overflow");
        //yyerror("Push-back buffer overflow");
        //do not printf error msg now, yylex will return -2
        //as for this fatal error, the caller (in jacc file must catch 
        //this retun code and print a good err msg, usually 
        //'token to long' (the other err msgs are very unlikely
    }
};


const int TEXT_RESIZE_CHAR_COUNT = 2048;
struct ASM_ScannerStruct
{
    TCHAR* pszTextBlock;
    int    iTextBlockLen;
    int    iTextBlockSize;
	ASM_Scanner ps;
};

static void ClearTextBlock(ASM_ScannerStruct* ps)
{
    ps->iTextBlockLen = 0;

    if(ps->iTextBlockSize>0)
    {
        assert(ps->pszTextBlock);
        ps->pszTextBlock[0] = 0;
    }
}
static void AppendCharToTextBlock(ASM_ScannerStruct* ps, TCHAR c)
{
    if(ps->iTextBlockLen >= ps->iTextBlockSize)
    {
        ps->iTextBlockSize += TEXT_RESIZE_CHAR_COUNT * sizeof(TCHAR);
        size_t sizeNew = ps->iTextBlockSize;


        int bRet;    
        if(ps->pszTextBlock != NULL)
            bRet = FC_ResizeMemory((void**) &(ps->pszTextBlock), sizeNew);
        else
            bRet = FC_NewMemory((void**) &(ps->pszTextBlock), sizeNew);
        
        assert(bRet);
    }
    if(ps->pszTextBlock)
    {
        
        //convert \r\n to \n, all internal strings are \n only:
        if(c==_T('\n') && ps->iTextBlockLen>0 && ps->pszTextBlock[ps->iTextBlockLen-1]==_T('\r'))
            ps->iTextBlockLen--;
        
        ps->pszTextBlock[ps->iTextBlockLen] = c;
        ps->iTextBlockLen++;
    }
}

ASM_ScannerStruct* ASM_CreateScanner()
{
	ASM_ScannerStruct* ps = new ASM_ScannerStruct;
    assert(ps);

    //init struct members:
    ps->pszTextBlock = NULL;
    ps->iTextBlockLen = 0;
    ps->iTextBlockSize = 0;
    
    return ps;
}
void ASM_DeleteScanner(ASM_ScannerStruct* ps)
{
	if(ps->pszTextBlock)
        FC_FreeMemory(ps->pszTextBlock);

    delete(ps);
}
void ASM_SetInputString(ASM_ScannerStruct* ps, 
                        const TCHAR*            pszInput, 
                        unsigned long           ulLen)
{
    assert(pszInput !=NULL);

	ps->ps.setinput(NULL);

    ps->ps.m_pszInput = pszInput;
    ps->ps.m_ulLen = ulLen;
    
    //reset other variables for next scan:
    ps->ps.m_ulRead =0;
    ps->ps.m_lOffset = 0;
    ps->ps.m_yylengOld = 0;
	ps->ps.m_nColumn = 1;

    ps->ps.YY_INIT();
}


int ASM_NextToken(ASM_ScannerStruct* ps, 
                  const TCHAR**      pszToken, 
                  int*               pnLine,
                  int*               pnColumn)
{
    int  ntok;
    bool bFirstInLine = ps->ps.m_nColumn==1; 

    nextLine:
    //skip white space:
    do
    {
        *pnColumn = ps->ps.m_nColumn;
        ntok = ps->ps.yylex();
        ps->ps.m_lOffset += ps->ps.m_yylengOld;
        ps->ps.m_yylengOld = ps->ps.yyleng; 
        ps->ps.m_nColumn +=ps->ps.yyleng;
    }
    while(-10==ntok);

    if(ntok=='\n')
    {
        ps->ps.m_nColumn = 1;
        if(bFirstInLine)
            goto nextLine;//skip empty lines
    }

    //comment
    if(ntok==';')
    {
        ps->ps.m_lOffset += ps->ps.m_yylengOld;
        ps->ps.m_yylengOld =0;
        TCHAR c;
        do
        {
            c=ps->ps.input();ps->ps.m_yylengOld++;ps->ps.m_nColumn++;
        }
	    while(c!='\n' && c!=EOF);

        if(c=='\n')
        {
            c=ps->ps.unput(c);ps->ps.m_yylengOld--;ps->ps.m_nColumn--;
        }
        goto nextLine;
    }



    *pszToken = ps->ps.yytext;
	*pnLine   = ps->ps.yylineno;
	return ntok;
}



int  ASM_SkipIECStringLiteral(ASM_ScannerStruct*  ps, 
                              TCHAR               delim, 
                              bool                bClearBuffer)
{
    int c;

    assert(delim==_T('"') || delim==_T('\''));
   
    if(bClearBuffer)
        ClearTextBlock(ps);

    AppendCharToTextBlock(ps, delim);

    for(;;)
    {
        c = ps->ps.input();ps->ps.m_yylengOld++;ps->ps.m_nColumn++;
        
        if(c==EOF)
        {
            return 0;
        }
        else if(c==_T('$'))
        {
            AppendCharToTextBlock(ps, (TCHAR)c);
            c = ps->ps.input();ps->ps.m_yylengOld++;ps->ps.m_nColumn++;
            if(c==EOF)
                return 0;
        }
        else if(c==delim)
        {
            break;
        }
        else if(c=='\n')
        {
            ps->ps.m_nColumn = 1;        
        }
        AppendCharToTextBlock(ps, (TCHAR)c);
    }

    AppendCharToTextBlock(ps, delim);
    return 1;
}

void ASM_SkipLine(ASM_ScannerStruct* ps, bool bStore)
{
	int c;

    ps->ps.m_lOffset += ps->ps.m_yylengOld;
    ps->ps.m_yylengOld =0;

    if(bStore)
        ClearTextBlock(ps);

    do
    {
        c=ps->ps.input();ps->ps.m_yylengOld++;ps->ps.m_nColumn++;
        if(bStore)
            AppendCharToTextBlock(ps, (TCHAR)c);
    }
	while(c!='\n' && c!=EOF);

    if(c=='\n')
        c=ps->ps.unput(c);ps->ps.m_yylengOld--;ps->ps.m_nColumn--;

    if(bStore)
    {
        //cut trailing \r\n:
        if(ps->iTextBlockLen>=1 && ps->pszTextBlock[ps->iTextBlockLen-1]=='\n')
            ps->pszTextBlock[--ps->iTextBlockLen] = 0;
        if(ps->iTextBlockLen>=1 && ps->pszTextBlock[ps->iTextBlockLen-1]=='\r')
            ps->pszTextBlock[--ps->iTextBlockLen] = 0;
    }
}


bool ASM_SkipBlock(ASM_ScannerStruct* ps, bool bStore, 
                   const TCHAR* pszEnd, bool bIgnoreCase)
{
	int	         c;
	const TCHAR* cp;
    bool          ret = 1;
    

    assert(bIgnoreCase==false);//ignore case not implemented....

    if(bStore)
        ClearTextBlock(ps);
        
    ps->ps.m_lOffset += ps->ps.m_yylengOld;
    ps->ps.m_yylengOld =0;

    for (cp = pszEnd; *cp != 0; ) 
    {
        c = ps->ps.input();ps->ps.m_yylengOld++;ps->ps.m_nColumn++;

        if (c == EOF) 
        {
			ret = 0;
			break;
		}
        else if(c == _T('\n'))
        {
            ps->ps.m_nColumn = 1;
        }

        if(bStore)
            AppendCharToTextBlock(ps, (TCHAR)c);

		if (c != *cp++) 
        {
			cp = pszEnd;
			if (c == *cp)
				cp++;
		}
    }

    if(bStore && ret)
    {
        //cut the block end marker e.g. "END_NATIVE":
        int ioff = ps->iTextBlockLen - _tcslen(pszEnd);
        assert(ioff>=0);
        if(ioff>=0)
            ps->pszTextBlock[ps->iTextBlockLen - _tcslen(pszEnd)] = 0;
    }

    return  ret;
}


const TCHAR* ASM_GetSkippedTextBlock(ASM_ScannerStruct* ps)
{
    //null terminate anyway:
    AppendCharToTextBlock(ps, 0);
    assert(ps->pszTextBlock);
    return ps->pszTextBlock;
}
