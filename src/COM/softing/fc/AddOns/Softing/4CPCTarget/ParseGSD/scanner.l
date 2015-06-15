
%{
#pragma warning(disable: 4102)      // 'xxx' : unreferenced label
#pragma warning(disable: 4309)      // 'initializing' : truncation of constant value
#pragma warning(disable: 4305)      // 'initializing' : truncation from 'const unsigned int' to 'unsigned char'
#pragma hdrstop
#include "stdafx.h"
#include <afx.h>
#include <afxwin.h>

#include "gsd_scan.h"
#include "PrmTextDB.h"
#include "ExtParamDB.h"
#include "Parser.h" 
#include "Gsd_Keyw.h"
#include "gsdmessage.h"

//----  Global Variables:   --------------------------------------------------*
YYSTYPE     yylval;                 // union for tokens between LEX and YACC

%}

/*===========================================================================*/
/*===       LEX Definitions                                                  */
/*===========================================================================*/

whitespace          [ \t]
lf                  \n|\r\n

line_cont           {whitespace}*\\

Sign                [+-]
Digit               [0-9]
Hex                 [0-9a-fA-F]

/* NameChar            [A-Za-z_\.\-0-9] */
NameChar            [A-Za-z_\.0-9]
OtherChar           [\+\*\/<>\(\)\[\]\{\}\!\$%&\?\'\^\|=# ;\,\:´-]
StringChar          {NameChar}|{OtherChar}

Keyword             {NameChar}+


/* start conditions: */
%start              DP_text

%%
/*===========================================================================*/
/*===       LEX Rules                                                        */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/*--------   Standard start condition: overread till begin of DP description */

/*---   test for begin of DP text: */
<0>^{whitespace}*#{Keyword}             {
                                            if (stricmp(yytext, "#Profibus_DP") == 0)
                                            {
                                                /* start scanning in DP text body */
                                                BEGIN(DP_text);
                                                return _PROFIBUS_DP;
                                            }
                                        }

/*---   ignore rest of line: */
<0>"\n"                                 {   ; }

/*---   any characters not handled yet  */
<0>.                                    {   /* skip text until DP marker is found */
                                            ; 
                                        }

/*---------------------------------------------------------------------------*/
/*-----------   DP_text start condition: DP description we're interested in! */

/*---   test for end of DP text: */
<DP_text>^{whitespace}*#{Keyword}       {
                                            if (stricmp(yytext, "#Profibus_DP") != 0)
                                            {
                                                /* ignoring rest of text */
                                                BEGIN(0);
                                            }
                                        }
/*---   comments: */
<DP_text>{whitespace}*";"                            {   
                                            yycomment("\n");
                                            return _COMMENT;
                                        }
                                            /* just eat comment line */


/*---   visible strings: */
/* interpret vis.- string by strict GSD rules */
/*
<DP_text>\"{StringChar}*\"
*/
/* be more forgiving with visible strings, allow umlauts and other localized stuff */
<DP_text>\"(\\.|[^\\"])*\"              {
                                            // strip leading and trailing quotes!
                                            if (*yytext == _T('\"')) {
                                                yylval.pszText = yytext + 1;
                                            }
                                            else {
                                                yylval.pszText = yytext;
                                            }
                                            int len = _tcslen(yylval.pszText);
                                            if (yylval.pszText[len - 1] == _T('\"')) {
                                                yylval.pszText[len - 1] = _T('\0');
                                            }
                                            return _VISIBLE_STRING;
                                        }

<DP_text>@([^@\n])*                     {
											yylval.pszText = yytext;
                                            return _SUB_FAMILY;
                                        }

/*---   logical line continuation: */
<DP_text>{line_cont}                    { yycomment("\n"); }     /* eat to join with next line */

/*---   integer literals: */
<DP_text>{Digit}+                       {
                                            sscanf(yytext,"%d",&yylval.ulVal);
                                            return _INTEGER_LITERAL;
                                        }
<DP_text>0x{Hex}+                       {
                                            yylval.ulVal = strtol(yytext, NULL, 0);
                                            return _INTEGER_LITERAL;
                                        }

/*---   simple tokens: */
<DP_text>"="                            {   return '='; }
<DP_text>","                            {   return ','; }
<DP_text>"("                            {   return '('; }
<DP_text>")"                            {   return ')'; }
<DP_text>"-"                            {   return '-'; }
<DP_text>"@"                            {   return '@'; }

/*---   keywords: */
<DP_text>{Keyword}                      {
                                            CGsdKeyword     keywords;  // should be a global instance, init/term when starting/stopping DLL!!!
                                            int             iRevision;
                                            yylval.pszText = yytext;
                                            int iToken = keywords.LookupKeyword(yytext, iRevision);
                                            if (iToken < 0) {
                                                // must be a user definition, forget this line
                                                yyerror("001"); //special keys for localization.
                                                yycomment("\n");
                                                return _USER_KEYWORD;
                                            }
                                            else {
                                                return iToken;
                                            }
                                        }

/*---   empty lines: */
<DP_text>^{whitespace}*$                {   
                                            yycomment("\n");
                                        }
                                            /* eat the empty line */

/*---   treat whitespaces between tokens: */
<DP_text>{whitespace}+                  {   ; }
                                            /* eat all whitespaces */

/*---   real newlines: */
<DP_text>{lf}                           {   
                                            return CRLF;
                                        }


<DP_text>{OtherChar}                    {   
                                            yylval.pszText = yytext;
                                            return _OTHER_CHAR;
                                        }


/*---   any characters not handled yet (must never reach this point!): */
<DP_text>.                              {
#ifdef _DEBUG
                                            yyerror("003");
#endif //_DEBUG
                                        }

%%

/*-------------------------- FUNCTION ---------------------------------------*/
CGsdScan::CGsdScan(LPCTSTR pszGsdFile, LPCTSTR pszLogFile /*= NULL*/)
/*>>>>
ctor

I   pszGsdFile:     name of GSD file
I   pszLogFile:     name of log file; if NULL, create temporary name

Result
  void
<<<<*/
  : yy_scan(2000),      // token buffer size
    m_hGsdFile(NULL),
    m_hLogFile(NULL),
    m_bUseTmpFile(FALSE),
    m_lErrorCount(0),
    eof_found(0),
	m_messages()
{
	CString str;

    if (pszLogFile == NULL) {
        // build temporary file name for log file:
        LPTSTR pszTmpName = _ttempnam(NULL, GSD_PARSE_LOGFILE);
        m_sLogFile = pszTmpName;
        free(pszTmpName);
        m_sLogFile += GSD_PARSE_LOGFILE_EXT;
        m_bUseTmpFile = TRUE;
    }
    else {
        m_sLogFile = pszLogFile;
        m_bUseTmpFile = FALSE;
    }
    
	m_hLogFile = ::_tfopen(m_sLogFile, _T("w"));
    ASSERT(m_hLogFile != NULL);

    m_sGsdFile = pszGsdFile;
    str.Format(_T("Parsing GSD file %s ...\n"), (LPCTSTR) m_sGsdFile);
    WriteToLog(str);

    // try opening GSD file:
    m_hGsdFile = ::_tfopen(pszGsdFile, _T("r"));
    if (m_hGsdFile == NULL) {
		//only for the log file.
		GSD_FormatMessage(str, E_GSD_FILE_NOT_FOUND,(LPCSTR)m_sGsdFile);
		WriteToLog(str);
    }
    setinput(m_hGsdFile);
}

/*-------------------------- FUNCTION ---------------------------------------*/
CGsdScan::~CGsdScan() 
/*>>>>
dtor

Result
  void
<<<<*/
{
    if (m_hGsdFile != NULL) {
        ::fclose(m_hGsdFile);
    }
    if (m_hLogFile != NULL) {
        ::fclose(m_hLogFile);
    }
    if (m_bUseTmpFile) {
        _tunlink(m_sLogFile);
    }

	m_messages.RemoveAll();
}

//------------------------------------------------------------------*
/**
 * Print error message, showing current line number if known.
 *
 * @param			fmt: format string (see wsprintf).
 * @param			...	 additional vargs - not used.
 * @see				Message()
*/
void  CGsdScan::yyerror(char *fmt, ...)
{
	try {
		CString strMsg;
		HRESULT hr = S_OK;

		strMsg = fmt;
		if(!strMsg.CompareNoCase("001")) {
			//skip user definition is a warning
			hr = W_GSD_SKIP_USERDEF;
			GSD_FormatMessage(strMsg,hr,m_sGsdFile, yytext);
		}

		if(!strMsg.CompareNoCase("002")) {
			//skip user definition is a warning
			hr = E_GSD_EOF_COMMENT;
			GSD_FormatMessage(strMsg,hr,m_sGsdFile);
		}
		
		if(!strMsg.CompareNoCase("003")) {
			//skip user definition is a warning
			hr = E_GSD_BAD_CHAR;
			GSD_FormatMessage(strMsg,hr,m_sGsdFile,yylineno,yytext);
		}

		if(hr == S_OK) {
			strMsg.Empty();
			if ((yylineno > 0) && (yyleng > 0)) {
				hr = E_GSD_YYERR_FMT1;
				GSD_FormatMessage(strMsg,hr,m_sGsdFile, yylineno, fmt, yytext);
			}
			else {
				hr = E_GSD_YYERR_FMT2;
				GSD_FormatMessage(strMsg,hr,m_sGsdFile, fmt);
			}
		}

		Message(hr,strMsg);
	}
	catch(CMemoryException *e) {
		e->Delete();
	}

}

/*-------------------------- FUNCTION ---------------------------------------*/
int CGsdScan::yygetc(void)
{
    if (eof_found) return EOF;
    int c = getc(yyin);
    if (c==EOF)
    {
        eof_found = 1;
        return '\n';
    }
    return c;
}

//------------------------------------------------------------------*
/**
 * Issues a error/warning message and adds the message to the 
 * error list.
 * @see	yyerror()
*/
void CGsdScan::Message(HRESULT hrMessage, LPCTSTR pszMessage)
{
	try {
		CString strMsg = pszMessage;
		VARIANT_BOOL bError;
		ParseMessage * pmsg = NULL;

		WriteToLog(pszMessage);

		pmsg = ::CreateParseMessage(hrMessage,pszMessage);
		if(!pmsg) {
			return;
		}

		m_messages.Add(pmsg);

		pmsg->IsError(&bError);
		if(bError == VARIANT_TRUE) {
			m_lErrorCount++;	
		}
	}
	catch(CMemoryException* e) {
		e->Delete();
	}
}

/*-------------------------- FUNCTION ---------------------------------------*/
void
    CGsdScan::WriteToLog(LPCTSTR pszLine)
/*>>>>
write line to log file

I   pszLine

Result
  void
<<<<*/
{
    if (m_hLogFile != NULL) {
        _fputts(pszLine, m_hLogFile);
        _fputtc(_T('\n'), m_hLogFile);
        fflush(m_hLogFile);
    }
}

void CGsdScan::SetMessages(CStationCallback *pCB)
{
	int iCount = m_messages.GetCount();
	for(int i = 0; i< iCount; i++) {
		//[] operator has implicit addref.
		ParseMessage * pMessage = m_messages[i]; 
		if(pMessage) {
			pCB->AddMessage(pMessage);
		}
	}

	pCB->SetErrorCount(m_lErrorCount);
} 

#ifdef _SHOW_TOKENS
/*-------------------------- FUNCTION ---------------------------------------*/
void
    CGsdScan::ShowToken(int token)
{
    static int iOldLine = -1;

    ASSERT(m_hLogFile != NULL);
    if (iOldLine != yylineno) {
        _ftprintf(m_hLogFile, _T("\n*TOK: line %d\n** TOK: "), yylineno);
        iOldLine = yylineno;
    }

    switch (token) {
    case _PROFIBUS_DP:
        _ftprintf(m_hLogFile, _T("<PROFIBUS_DP>\n"));
        break;
    case _INTEGER_LITERAL:
        _ftprintf(m_hLogFile, _T("<INTEGER_LITERAL=%d>"), yylval.ulVal);
        break;
    case CRLF:
        _ftprintf(m_hLogFile, _T("<CRLF>\n"));
        break;
    case _COMMENT:
        _ftprintf(m_hLogFile, _T("<COMMENT>\n"));
        break;
    case _VISIBLE_STRING:
        _ftprintf(m_hLogFile, _T("<VISIBLE_STRING=%s>"), yylval.pszText);
        break;
    case _USER_KEYWORD:
        _ftprintf(m_hLogFile, _T("<USER KEYWORD=%s>"), yylval.pszText);
        break;
    case '=':
    case ',':
    case '(':
    case ')':
    case '-':
        _ftprintf(m_hLogFile, _T("<\"%c\">"), token);
        break;
    default:
        {
        CGsdKeyword keywords;
        LPCSTR pszKeyword = keywords.KeywordFromToken(token);
        if (pszKeyword != NULL) {
            _ftprintf(m_hLogFile, _T("<%s (%d)>"), pszKeyword, token);
        }
        else {
            _ftprintf(m_hLogFile, _T("<!!ERROR!! Unknown Token\"%c\" (%d)>"), token, token);
        }
        }
        break;
    }
    fflush(m_hLogFile);
}
#endif // _SHOW_TOKENS


/*-------------------------- FUNCTION ---------------------------------------*/
void
    yy_scan::yycomment(char *const mat)
// skip comment input (see MKSlib)
{
    register int c;
    register char *cp;

    for (cp = mat; *cp != 0; ) {
        c = input();
        if (c == EOF) {
            yyerror("002");
            break;
        }
        if (c != *cp++) {
            cp = mat;
            if (c == *cp)
                cp++;
        }
    }
}


