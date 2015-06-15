/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/iecsyntax.l 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: iecsyntax.l $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/iecsyntax.l $
 *
 * =PROJECT             7VH102  ATCMControl
 *
 * =SWKE                CMN
 *
 * =COMPONENT           UTIL
 *
 * =CURRENT             $Date: 28.02.07 19:00 $
 *                      $Revision: 1 $
 *
 * =ENVIRONMENT         Win32:
 *                      MS Windows NT 4.0
 *                      MSVC++ V5.0 / MFC 4.2 / ATL 2.0 / DAO 3.5
 *                      MS Java 2.0
 *                      MS Visual SourceSafe 5.0
 *
 * =REFERENCES
 *==
 *----------------------------------------------------------------------------*
 *

 

 

 *==
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *== Lex script with basic IEC syntax.
 *   Note that things like ID or typed literlas 
 *   are not checked in detail. e.g. UTIL_STS_BASIC_ID may be a Keyword
 *   or could also be just a '_' which is an invalid identifier, use 
 *   UTIL_CheckIdentifierEx to check for keyword and exakt syntax.
 *----------------------------------------------------------------------------*
 * =MODIFICATIONS
 * $History: iecsyntax.l $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 22.08.01   Time: 14:27
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * call yy_reset() to reset the lineno
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 9.03.01    Time: 20:43
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * some new function 
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 22.02.01   Time: 21:04
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * inserted first ..Ex function
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 19.02.01   Time: 9:32
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * made skip block ignore case aware
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 23.01.01   Time: 17:10
 * Created in $/4Control/COM/softing/fc/CMN/fc_util
 * utils and baselib from V1.3
 * 
 * *****************  Version 9  *****************
 * User: Ef           Date: 19.10.99   Time: 11:59
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * fixed d-1618
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 5.08.99    Time: 15:29
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * direct addr syntax $const
 * 
 * *****************  Version 7  *****************
 * User: Jd           Date: 30.07.99   Time: 13:35
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * IEC-Syntax: case-insensitive
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 8.06.99    Time: 9:34
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * fixed bug
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 2.06.99    Time: 20:12
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * more st syntax
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 1.06.99    Time: 22:19
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 1.06.99    Time: 21:50
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 1.06.99    Time: 19:16
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * started to integrate 4ccg sts_scan to util.dll
 *==
 ******************************************************************************
H<<*/

 
 /* Structured Text Grammar according to IEC 1131-3 (corrigenda 3 included) */
%n 1000
%e 2000
%p 5000
%{
/* #yy_movetop will be processed by yyreplace.pl, all these lines are
 * moved to top of the lex generated cpp file.
 */
#yy_movetop #include <ctype.h>
#yy_movetop #include "assert.h"
#yy_movetop #ifndef UTIL_IMP
#yy_movetop #define UTIL_IMP    // declare as dllexports
#yy_movetop #endif
#yy_movetop #include "utilsts.h"
#yy_movetop #define FC_LIB_USE_CRT
#yy_movetop #include "fc_tools\fc_libmem.h"

#yy_movetop #pragma warning( disable : 4068 4102 4244)
#yy_movetop //foreward
#yy_movetop class UTIL_STS_scanner;
%}
letter                  [A-Za-z]
alphanum                [A-Za-z0-9]
digit                   [0-9]
octal_digit             [0-7]
hex_digit               [0-9A-Fa-f]
bit                     [01]
identifierEx            ({letter}|_)(_|{alphanum})*
integer                 {digit}(_?{digit})*
lit_true                ([Bb][Oo][Oo][Ll]#)?[Tt][Rr][Uu][Ee]
lit_false               ([Bb][Oo][Oo][Ll]#)?[Ff][Aa][Ll][Ss][Ee]
binary_integer          2#{bit}(_?{bit})*
octal_integer           8#{octal_digit}(_?{octal_digit})*
hex_integer             16#{hex_digit}(_?{hex_digit})*
real                    {integer}\.{integer}([Ee][+-]?{integer})?
fixed_point             {integer}(\.{integer})?

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
"**"                    return UTIL_STS_POW;
"&"                     return UTIL_STS_AND;
"="                     return UTIL_STS_EQ;
"<>"                    return UTIL_STS_NE;
"<"                     return UTIL_STS_LT;
"<="                    return UTIL_STS_LE;
">"                     return UTIL_STS_GT;
">="                    return UTIL_STS_GE;
":"                     return ':';
":="                    return UTIL_STS_BECOMES;
"=>"                    return UTIL_STS_RIGHT_ARROW;
";"                     return ';';
"."                     return '.';
".."                    return UTIL_STS_DOTDOT;
","                     return ',';
"["                     return '[';
"]"                     return ']';
"("                     return '(';
")"                     return ')';
"{"                     return '{';
"}"                     return '}';
"|"                     return '|';
"..."                   return UTIL_STS_PERIOD;
["']                    return UTIL_STS_STRING_LITERAL_BEGIN;

/* comment styles */
"]]*)"                   return UTIL_STS_SYS_COMMENT_END;
"(*[["                   return UTIL_STS_SYS_COMMENT_BEGIN;
"(*"                     return UTIL_STS_ST_COMMENT_BEGIN;

/* compiler directives */
#[Ii][Mm][Pp][Oo][Rr][Tt]                  return UTIL_STS_IMPORT_DIRECTIVE;
#[Pp][Rr][Aa][Gg][Mm][Aa]                  return UTIL_STS_PRAGMA_DIRECTIVE;

/* regular expressions */
{lit_true}               return UTIL_STS_TRUE_LITERAL;
{lit_false}              return UTIL_STS_FALSE_LITERAL;
{int_or_bit_literal}     return UTIL_STS_INT_LITERAL;
{real_literal}           return UTIL_STS_FLOAT_LITERAL;
{date}                   return UTIL_STS_DATE_LITERAL;
{date_and_time}          return UTIL_STS_DT_LITERAL;
{time_of_day}            return UTIL_STS_TOD_LITERAL;
{duration}               return UTIL_STS_TIME_LITERAL;
{direct_variable}        return UTIL_STS_DIRECT_VARIABLE; 
{identifierEx}           return UTIL_STS_IDENTIFIER_EX; 


[\n]                     return '\n';
[ \t\r]                  return -10;

.                        return UTIL_STS_SYNTAX_ERROR;
%%


//returns 0 if EOF in comment
//        1 all ok
//        2 if comment is nested
static int STcomment2(
    const TCHAR   szText[/*nChars*/],
    long          nChars, 
    const TCHAR*  mat,        //see TODO in function...
    int           nNest,      //must be 1 on starting call
    long*         pnCharsRead //output: receives the number of chars 
                              //read until the end of comment
                              // szText[*pnCharsRead] id the char
                              //right after the closing *)
);
//returns 0 if EOF in comment
//        1 all ok
//        2 if comment is nested
static int STcomment(
    UTIL_STS_scanner* ps,   
    const TCHAR*      mat,  //see TODO in function...
    int               nNest //must be 1 on starting call
);





class UTIL_STS_scanner: public iecsyntax_scan
{
public:    
    const TCHAR* m_pszInput;  //string buffer as input
    unsigned long m_ulLen;    //length buffer in bytes
    unsigned long m_ulRead;   //current read position in buffer
    long          m_lOffset;  //absolute file offset of last token
    long          m_yylengOld;//size in byte of last token
	int			  m_nColumn;  //current column

    UTIL_STS_scanner() : iecsyntax_scan(UTIL_STS_MAX_TOKEN_LENGTH)
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
    {                                           //##d-1618 cast unsigned!
        return m_pszInput ? (m_ulRead<m_ulLen ? (unsigned char)m_pszInput[m_ulRead++] : EOF) : getc(yyin);
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
struct UTIL_STS_ScannerStruct
{
    TCHAR* pszTextBlock;
    int    iTextBlockLen;
    int    iTextBlockSize;
	UTIL_STS_scanner ps;
};

static void ClearTextBlock(UTIL_STS_ScannerStruct* ps)
{
    ps->iTextBlockLen = 0;

    if(ps->iTextBlockSize>0)
    {
        assert(ps->pszTextBlock);
        ps->pszTextBlock[0] = 0;
    }
}
static void AppendCharToTextBlock(UTIL_STS_ScannerStruct* ps, TCHAR c)
{
    if(ps->iTextBlockLen >= ps->iTextBlockSize)
    {
        ps->iTextBlockSize += TEXT_RESIZE_CHAR_COUNT * sizeof(TCHAR);
        size_t sizeNew = ps->iTextBlockSize;


        int bRet;    
        if(ps->pszTextBlock != NULL)
            bRet = FC_LibResizeMemory((void**) &(ps->pszTextBlock), sizeNew);
        else
            bRet = FC_LibNewMemory((void**) &(ps->pszTextBlock), sizeNew);
        
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



UTILIF UTIL_STS_ScannerStruct* UTIL_STS_CreateScanner()
{
	UTIL_STS_ScannerStruct* ps = new UTIL_STS_ScannerStruct;
    assert(ps);

    //init struct members:
    ps->pszTextBlock = NULL;
    ps->iTextBlockLen = 0;
    ps->iTextBlockSize = 0;
    
    return ps;
}
UTILIF void UTIL_STS_DeleteScanner(UTIL_STS_ScannerStruct* ps)
{
	if(ps->pszTextBlock)
        FC_LibFreeMemory(ps->pszTextBlock);

    delete(ps);
}
UTILIF void UTIL_STS_SetInputString(UTIL_STS_ScannerStruct* ps, 
                                    const TCHAR*            pszInput, 
                                    unsigned long           ulLen)
{
    assert(pszInput !=NULL);

    ps->ps.yy_reset();
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

UTILIF void UTIL_STS_SetInputStringEx(
    UTIL_STS_ScannerStruct* ps, 
    const TCHAR*            pszInput, 
    unsigned long           ulLen,
	unsigned long           ulBytePos
)
{
    unsigned long ul;
    char          c;

    UTIL_STS_SetInputString(ps, pszInput, ulLen);

    for(ul=0; ul<ulBytePos;ul++) 
    {
        c = ps->ps.input();ps->ps.m_nColumn++;
        if(c == EOF) 
            return;
        else if(c == _T('\n'))
            ps->ps.m_nColumn = 1;
    }	
}




UTILIF int UTIL_STS_NextToken(UTIL_STS_ScannerStruct* ps, 
                              const TCHAR**           pszToken, 
                              int*                    pnLine,
                              int*                    pnColumn)
{
	int ntok;

    do
    {
		*pnColumn = ps->ps.m_nColumn;
        ntok = ps->ps.yylex();
        ps->ps.m_lOffset += ps->ps.m_yylengOld;
        ps->ps.m_yylengOld = ntok==0 ? 0: ps->ps.yyleng; //HACK 
        ps->ps.m_nColumn +=ps->ps.yyleng;
    }
    while(-10==ntok);

    if(ntok == '\n')
        ps->ps.m_nColumn = 1;

    *pszToken = ps->ps.yytext;
	*pnLine   = ps->ps.yylineno;
	return ntok;
}

UTILIF long UTIL_STS_GetFPosOfNextChar(
    const UTIL_STS_ScannerStruct* ps
)
{
	return ps->ps.m_lOffset + ps->ps.m_yylengOld;
}


UTILIF int UTIL_STS_GetLineColNextChar(
    const  UTIL_STS_ScannerStruct* ps,
    int*                          pCol           
)
{
    *pCol = ps->ps.m_nColumn;
	return ps->ps.yylineno;
}

UTILIF long UTIL_STS_GetOffsetOfLastToken(const UTIL_STS_ScannerStruct* ps)
{
	return ps->ps.m_lOffset;
}
UTILIF long UTIL_STS_GetLengthOfLastToken(const UTIL_STS_ScannerStruct* ps)
{
	return ps->ps.m_yylengOld;
}

UTILIF unsigned long UTIL_STS_GetLengthOfText(
    const UTIL_STS_ScannerStruct* ps
)
{
    return ps->ps.m_ulLen;
}

UTILIF int UTIL_STS_GetFPOSFromLineCol(
    const TCHAR*   pszFile,
    int            line,
    int            col,
    unsigned long* pulFpos
)
{
    const TCHAR* psz;
    
    assert(line>0 && col>0);
    if(!(line>0 && col>0))
        return 0;

    psz = pszFile; 
    
    while(line>1 && *psz)
    {
        if(*psz=='\n')
            line--;
        psz++;
    }        
    if(line!=1)
        return 0;

    while(--col>0 && *psz!=0)
        psz++;

    if(col!=0)
        return 0;

    *pulFpos = psz - pszFile;

    return 1;
}


UTILIF const TCHAR* UTIL_STS_GetText(
    const UTIL_STS_ScannerStruct* ps
)
{
    return ps->ps.m_pszInput;
}






//returns 0 if EOF in comment 
//        1 all ok
UTILIF int UTIL_STS_BufferSkipComment(
    const TCHAR**                 ppsz,
    long*                         pnChars
)
{
    const TCHAR* psz;
    long         nCharsLeft;
    long         nCharsRead;

    psz = *ppsz;
    nCharsLeft = *pnChars;

    assert(nCharsLeft>=0);


NextComment:
    while(nCharsLeft>0 && isspace(*psz))
    {
        psz++;
        nCharsLeft--;
    }
    if(nCharsLeft>=2 && psz[0]=='(' && psz[1]=='*')
    {
        nCharsLeft -= 2;
        psz += 2;
        if(STcomment2(psz, nCharsLeft, "*)", 1, &nCharsRead)==0)
        {
            *ppsz = NULL;
            *pnChars = 0;
            return 0;
        }

        nCharsLeft -= nCharsRead;
        assert(nCharsLeft>=0);
        psz += nCharsRead;
        goto NextComment;
    }

Ende:
    if(nCharsLeft>0)
    {
        *ppsz = psz;
        *pnChars = nCharsLeft;
    }
    else
    {
        assert(nCharsLeft==0);
        *ppsz = NULL;
        *pnChars = 0;
    }
    return 1;       
}




UTILIF const TCHAR* UTIL_STS_GetTextAfterLastToken(
    const UTIL_STS_ScannerStruct* ps,
    long*                         pnChars
)
{
    const TCHAR* psz;

    *pnChars = ps->ps.m_ulLen - ps->ps.m_lOffset - ps->ps.m_yylengOld;
    assert(*pnChars>=0);
    if(!ps->ps.m_pszInput || *pnChars<=0)
    {
        *pnChars = 0;
        return NULL;
    }
    
    psz =  ps->ps.m_pszInput + ps->ps.m_lOffset + ps->ps.m_yylengOld;
    assert(psz <= ps->ps.m_pszInput + ps->ps.m_ulRead);
    
    return psz;
}

UTILIF void UTIL_STS_SkipLine(UTIL_STS_ScannerStruct* ps)
{
	int c;

    ps->ps.m_lOffset += ps->ps.m_yylengOld;
    ps->ps.m_yylengOld =0;

    do
    {
        c=ps->ps.input();ps->ps.m_yylengOld++;ps->ps.m_nColumn++;
    }
	while(c!='\n' && c!=EOF);

    if(c=='\n')
    {
        ps->ps.unput(c);ps->ps.m_yylengOld--;ps->ps.m_nColumn--;
    }
}




//returns 0 if EOF in comment
//        1 all ok
//        2 if comment is nested
static int STcomment2(
    const TCHAR   szText[/*nChars*/],
    long          nChars, 
    const TCHAR*  mat, 
    int           nNest,
    long*         pnCharsRead //output: receives the number of chars 
                              //read until the end of comment
                              // szText[*pnCharsRead] id the char
                              //right after the closing *)
)
{
	int	          c;
	const TCHAR*  cp;
    char          l1=0,l2=0;
    int           ret = 1;
    long          nRead;
    
    assert(!strcmp(mat,"*)"));//sorry not really implemented a general mat
                              //see TODO below

    *pnCharsRead = 0;

    for (cp = mat; *cp != 0; ) 
    {
        if(*pnCharsRead>=nChars) 
        {
			ret = 0;
			break;
		}
        
        c = szText[*pnCharsRead];
        (*pnCharsRead)++;

        l1 = l2;
        l2 = c;
        
        if(l1=='(' && l2=='*') //TODO is not a mat as on input
        {
			if(nNest==1)
                ret = 2;

            if(0 == STcomment2(szText + *pnCharsRead, nChars - *pnCharsRead, 
                    mat, ++nNest, &nRead)
            )
            {
                *pnCharsRead += nRead;
                ret = 0;
                break;
            }
            *pnCharsRead += nRead;

            //reset:
            l1 = l2 = 0;
			cp = mat;
            continue;
        }
		if (c != *cp++) 
        {
			cp = mat;
			if (c == *cp)
				cp++;
		}
	}

    return ret;
}

//return: see STcomment2
static int STcomment(
    UTIL_STS_scanner* ps, 
    const TCHAR*      mat,  //see TODO in function...
    int               nNest //must be 1 on starting call
)
{
	int	c;
	const TCHAR* cp;
    char l1=0,l2=0;
    int ret = 1;
    
    ps->m_lOffset += ps->m_yylengOld;
    ps->m_yylengOld =0;

    assert(!strcmp(mat,"*)"));//sorry not really implemented a general mat
                              //see TODO below

    for (cp = mat; *cp != 0; ) 
    {
        c = ps->input();ps->m_yylengOld++;ps->m_nColumn++;
        
        l1 = l2;
        l2 = c;
        if (c == EOF) 
        {
			ret = 0;
			break;
		}
        else if(c == _T('\n'))
        {
            ps->m_nColumn = 1;
        }

        if(l1=='(' && l2=='*') //TODO generall mat?
        {
			if(nNest==1)
                ret = 2;

            if(0 == STcomment(ps, mat, ++nNest))
            {
                ret = 0;
                break;
            }

            //reset:
            l1 = l2 = 0;
			cp = mat;
            continue;
        }
		if (c != *cp++) 
        {
			cp = mat;
			if (c == *cp)
				cp++;
		}
	}

    return ret;
}




UTILIF int UTIL_STS_SkipComment(UTIL_STS_ScannerStruct* ps, const TCHAR* pszEnd)
{
	return STcomment(&ps->ps, pszEnd, 1);
}

UTILIF void UTIL_STS_GetTokenUpToEOL(UTIL_STS_ScannerStruct* ps)
{
    TCHAR c;

    ClearTextBlock(ps);

    ps->ps.m_lOffset += ps->ps.m_yylengOld;
    ps->ps.m_yylengOld =0;

    do
    {
        c = ps->ps.input();ps->ps.m_yylengOld++;ps->ps.m_nColumn++;

        if(c=='\n')
        {
            ps->ps.unput(c);ps->ps.m_yylengOld--;ps->ps.m_nColumn--;
            return;
        }
    }
    while(isspace(c) && c!=EOF);

    int i=0;
    while(c!='\r' &&  c!='\n' && c!=EOF)
    {
        AppendCharToTextBlock(ps, c);
        c = ps->ps.input();ps->ps.m_yylengOld++;ps->ps.m_nColumn++;
    }

    if(c=='\r' || c=='\n')
    {
        ps->ps.unput(c);ps->ps.m_yylengOld--;ps->ps.m_nColumn--;
    }

    for(i=ps->iTextBlockLen-1; i>=0 && isspace(ps->pszTextBlock[i]); )
    {
        ps->pszTextBlock[i]=0;
        ps->iTextBlockLen--;
    }
}


UTILIF int  UTIL_STS_SkipIECStringLiteral(UTIL_STS_ScannerStruct* ps, 
                                          TCHAR                   delim, 
                                          bool                    bClearBuffer)
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

UTILIF int UTIL_STS_SkipBlock(UTIL_STS_ScannerStruct* ps, 
                              bool                    bStore, 
                              const TCHAR*            pszEnd, 
                              bool                    bIgnoreCase)
{
    int          c;
    const TCHAR* cp;
    TCHAR        pszInput[2];
    int          ret = 1;
    
    pszInput[1] = 0;

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

		if(bIgnoreCase)
        {
            pszInput[0] = c;
            if(!strnicmp(pszInput, cp, 1))
            {
                cp++;
            }
            else
            {
                cp = pszEnd;
                if (!strnicmp(pszInput, cp, 1))
                    cp++;
            }
        }
        else
        {
            if (c != *cp++) 
            {
                 cp = pszEnd;
                 if (c == *cp)
                 cp++;
	    }
        }
    }

    if(bStore && ret==1)
    {
        //cut the block end marker e.g. "END_NATIVE":
        int ioff = ps->iTextBlockLen - _tcslen(pszEnd);
        assert(ioff>=0);
        if(ioff>=0)
            ps->pszTextBlock[ps->iTextBlockLen - _tcslen(pszEnd)] = 0;
    }

    return  ret;
}



UTILIF int UTIL_STS_SkipMatchingBrace(
    UTIL_STS_ScannerStruct* ps, 
    bool                    bStore, 
    TCHAR                   cBrace
)
{
	int	   c;
    TCHAR  endChar;
    int    iOpenBlock = 1;
    

    if(cBrace==_T('('))
        endChar = _T('(');
    else if(cBrace==_T('{'))     
        endChar = _T('}');
    else if(cBrace==_T('['))     
        endChar = _T(']');
    else if(cBrace==_T('<'))     
        endChar = _T('>');
    else 
    {
        assert(!"bad brace");
        return -1;
    }

    if(bStore)
        ClearTextBlock(ps);
        
    ps->ps.m_lOffset += ps->ps.m_yylengOld;
    ps->ps.m_yylengOld =0;

    for (;;) 
    {
        c = ps->ps.input();ps->ps.m_yylengOld++;ps->ps.m_nColumn++;

        if (c == EOF) 
        {
			return 0;
		}
        else if(c == _T('\n'))
        {
            ps->ps.m_nColumn = 1;
        }
        else if(c==cBrace)
        {
            iOpenBlock++;
        }
        else if(c==endChar)
        {
            iOpenBlock--;
            if(iOpenBlock==0)
                return 1;
        }

        if(bStore)
            AppendCharToTextBlock(ps, (TCHAR)c);

    }

    return 0;
}




UTILIF const TCHAR* UTIL_STS_GetSkippedTextBlock(UTIL_STS_ScannerStruct* ps)
{
    //null terminate anyway:
    AppendCharToTextBlock(ps, 0);
    assert(ps->pszTextBlock);
    return ps->pszTextBlock;
}

/* end of lex file */
