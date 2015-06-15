/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/ConstFileParser.h 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: ConstFileParser.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/ConstFileParser.h $
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

#ifndef __CONSTFILEPARSER_H_
#define __CONSTFILEPARSER_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include <afxwin.h>
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#define ENDE 0
#define COMMENT 1
#define ASSIGN 2
#define COLON 3
#define SEMICOLON 4
#define IDENTIFIER 5
#define CONST_TOKEN 6
#define END_CONST_TOKEN 7
#define NOTOKEN 6
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



// ConstFileParser.h: Schnittstelle für die Klasse ConstFileParser.
//
//////////////////////////////////////////////////////////////////////

class ConstList;
class ConstFileParser  
{
public:
	ConstFileParser();
	virtual ~ConstFileParser();
	void SetText(const CString& text);
	int NextToken(CString& strToken);
	int Parse(ConstList* pConstList);
	int GetLastLineNo();


protected:
	int LexFail();
	TCHAR NextChar();
	void Retract(int iPos);
	int GetTokenString(int token, CString& strToken);
	int ParseFail();
	void MakeEntry(ConstList* pConstList,const CString& strVarName, const CString& strVarType, const CString& strVarValue);

private:
	//
	// scanner
	//
	int m_lexState;
	int m_lexStart;
	CString m_strParseText;
	int m_position;
	int m_tokenBegin;
	int m_lineNumber;

	//
	// parser
	//
	int m_parseState;
	int m_parseStart;

};

#endif // __CONSTFILEPARSER_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: ConstFileParser.h $
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
