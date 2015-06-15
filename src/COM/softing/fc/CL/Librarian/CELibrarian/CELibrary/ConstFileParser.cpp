/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/ConstFileParser.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: ConstFileParser.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/ConstFileParser.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CL
 *
 * =COMPONENT           CELibrarian
 *
 * =CURRENT      $Date: 28.02.07 18:59 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  6/25/2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include <iostream.h>
#include "ConstFileParser.h"
#include "ConstList.h"
//----  Static Initializations:   ----------------------------------*



// ConstFileParser.cpp: Implementierung der Klasse ConstFileParser.
//
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

ConstFileParser::ConstFileParser()
{
	
	m_lexState=0;
	m_lexStart=0;
	m_strParseText="";
	m_position=-1;
	m_tokenBegin=0;
	m_parseState=0;
	m_parseStart=0;
	m_lineNumber=0;
}

ConstFileParser::~ConstFileParser()
{

}

void ConstFileParser::SetText(const CString& strText)
{
	m_strParseText=strText;
	m_lexState=0;
	m_lexStart=0;
	m_position=-1;
	m_tokenBegin=0;
	m_parseState=0;
	m_parseStart=0;
	m_lineNumber=0;
}

TCHAR ConstFileParser::NextChar()
{
	int it=m_strParseText.GetLength();
	if ( m_position+2 >= m_strParseText.GetLength() )
	{
		return 0;
	}
	else
	{
		return m_strParseText[++m_position];
	}
}

void ConstFileParser::Retract(int iPos)
{
	m_position-=iPos;
	ASSERT(m_position >=-1);
}


int ConstFileParser::NextToken(CString& strToken)
{
	strToken="";
	TCHAR ch;
	for (;;)
	{
		switch(m_lexState)
		{
		case 0: 
			ch = NextChar();
			m_tokenBegin=m_position;
			if (ch==' ' || ch=='\t' || ch=='\n' || ch=='\r')
			{
				if (ch == '\n')
					m_lineNumber++;
			}
			else if ( ch == '(' ) 
				m_lexState = 11;
			else if ( ch == ':' )
				m_lexState = 21;
			else if ( ch == ';' )
				m_lexState = 31;
			else if ( ch == '\0' )
				return 0;
			else if ( ch == '"' )
				m_lexState = 61;
			else 
				m_lexState = 41;
			break;
		case 11:
			ch = NextChar();
			if ( ch == '*' ) 
				m_lexState = 12;
			else
				m_lexState = LexFail();
			break;
		case 12:
			ch = NextChar();
			if ( ch == '*' ) 
				m_lexState = 13;
			else if ( ch == '\0' )
				m_lexState = LexFail();
			else
				m_lexState = 12;
			break;
		case 13:
			ch = NextChar();
			if ( ch == ')' )
			{
				m_lexState = 0;
				return GetTokenString(COMMENT,strToken);
			}
			else if ( ch == '\0' )
				m_lexState = LexFail();
			else
				m_lexState = 12;
			break;
		case 21:
			ch = NextChar();
			if ( ch == '=' )
			{
				m_lexState=0;
				return GetTokenString(ASSIGN,strToken);
			}
			else
			{
				m_lexState=0;
				Retract(1);
				return GetTokenString(COLON,strToken);
			}
			break;
		case 31:
			m_lexState=0;
			return GetTokenString(SEMICOLON,strToken);
			break;
		case 41: //identifier
			ch=NextChar();
			if (ch == '#' )
			{
				m_lexState = 51; //special initial value
			}
			else if ( ch == ':' || ch == ';' || ch == ' ' || ch == '\t' || ch == '\n' || ch =='\r' )
			{
				m_lexState=0;
				Retract(1);
				return GetTokenString(IDENTIFIER,strToken);
			}
			else if (ch == '\0')
			{
				m_lexState=0;
				return GetTokenString(IDENTIFIER,strToken);
			}
			else
				m_lexState=41;
			break;
		case 51: //special inital value e.g. dt#0001-01-01-00:00:00
			ch=NextChar();
			if ( ch == ';' || ch == ' ' || ch == '\t' || ch == '\n' || ch =='\r' )
			{
				m_lexState=0;
				Retract(1);
				return GetTokenString(IDENTIFIER,strToken);
			}
			else if (ch == '\0')
			{
				m_lexState=0;
				return GetTokenString(IDENTIFIER,strToken);
			}
			else
				m_lexState=51;
			break;
		case 61: //string constant
			ch=NextChar();
			if ( ch == '\n' || ch =='\r' )
			{
				m_lexState=0;
				Retract(1);
				return GetTokenString(IDENTIFIER,strToken);
			}
			else if (ch == '\0')
			{
				m_lexState=0;
				return GetTokenString(IDENTIFIER,strToken);
			}
			else if ( ch == '"')
			{
				m_lexState=0;
				return GetTokenString(IDENTIFIER,strToken);
			}
			else
				m_lexState=61;
			break;
		default:
			ASSERT(0);
		}
	}
}

int ConstFileParser::LexFail()
{
	return 0;
}

int ConstFileParser::GetTokenString(int token, CString& strToken)
{
	strToken=m_strParseText.Mid(m_tokenBegin,m_position-m_tokenBegin+1);
	if (token == IDENTIFIER)
	{
		if ( strToken == "CONST" )
			return CONST_TOKEN;
		else if ( strToken== "END_CONST" )
			return END_CONST_TOKEN;
		else
			return token;
	}
	else
	{
		return token;
	}
}

int ConstFileParser::Parse(ConstList* pConstList)
{
	CString strToken,strVarName,strVarType,strVarValue;
	int token;
	for (;;)
	{
		token=NextToken(strToken);
		if (token==ENDE)
		{
			if (m_parseState==0)
				return 1;
			else 
				return 0;
		}

		switch(m_parseState)
		{
		case 0:
			if (token == 0)
				return 0;
			else if (token == COMMENT)
				m_parseState=0;
			else if (token == CONST_TOKEN)
				m_parseState=11;
			else
				m_parseState = ParseFail();
			break;
		case 11:
			if (token == IDENTIFIER)
			{
				strVarName=strToken;
				m_parseState = 12;
			}
			else if (token == COMMENT)
				m_parseState=11;
			else if (token == SEMICOLON )
				m_parseState=11;
			else if (token == END_CONST_TOKEN)
				m_parseState=0;
			else
				m_parseState=ParseFail();
			break;
		case 12:
			if (token == COLON)
				m_parseState=13;
			else if (token == COMMENT)
				m_parseState=12;
			else
				m_parseState=ParseFail();
			break;
		case 13:
			if (token == IDENTIFIER)
			{
				strVarType=strToken;
				m_parseState=14;
			}
			else
				m_parseState=ParseFail();
			break;
		case 14:
			if (token == ASSIGN)
				m_parseState=15;
			else
				m_parseState=ParseFail();
			break;
		case 15:
			if (token == IDENTIFIER)
			{
				strVarValue=strToken;
				MakeEntry(pConstList,strVarName,strVarType,strVarValue);
				m_parseState=16;
			}
			else
				m_parseState=ParseFail();
			break;
		case 16:
			if (token == SEMICOLON)
				m_parseState=11;
			else
				m_parseState=ParseFail();
			break;
		default:
			ASSERT(0);
		}
	}
};

int ConstFileParser::ParseFail()
{
	if (m_parseState == 0)
		return 0;
	else 
		return 11;
}

void ConstFileParser::MakeEntry(ConstList* pConstList,const CString& strVarName, const CString& strVarType, const CString& strVarValue)
{
	//cout << strVarName << " " << strVarType << " " << strVarValue << endl;
	pConstList->AddConst(strVarName, strVarType, strVarValue);
}

int ConstFileParser::GetLastLineNo()
{
	return m_lineNumber;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: ConstFileParser.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:33p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 *==
 *----------------------------------------------------------------------------*
*/
