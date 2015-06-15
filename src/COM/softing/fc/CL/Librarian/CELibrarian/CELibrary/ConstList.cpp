/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/ConstList.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: ConstList.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/ConstList.cpp $
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
#include "ConstList.h"
//----  Static Initializations:   ----------------------------------*



// ConstList.cpp: Implementierung der Klasse ConstList.
//
//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

ConstList::ConstList()
{

}

ConstList::~ConstList()
{

}

void ConstList::AddConst(LPCTSTR pszConstName, LPCTSTR pszConstType, LPCTSTR pszConstValue)
{
	m_constNameArray.Add(pszConstName);
	m_constTypeArray.Add(pszConstType);
	m_constValueArray.Add(pszConstValue);
}

int ConstList::GetConstCount()
{
	return m_constNameArray.GetSize();
}

int ConstList::GetConst(int iIndex, LPTSTR pszConstName, LPTSTR pszConstType, LPTSTR pszConstValue)
{
	*pszConstName='\0';
	*pszConstType='\0';
	*pszConstValue='\0';
	if ( iIndex >= GetConstCount() )
		return 0;
	else
	{
		CString strHelp;
		strHelp=m_constNameArray.GetAt(iIndex);
		_tcscpy(pszConstName,strHelp);
		strHelp=m_constTypeArray.GetAt(iIndex);
		_tcscpy(pszConstType,strHelp);
		strHelp=m_constValueArray.GetAt(iIndex);
		_tcscpy(pszConstValue,strHelp);
		return 1;
	}
}

BOOL ConstList::ExistConst(LPCTSTR pszConstName)
{
	for (int ii=0; ii<GetConstCount(); ii++)
	{
		if ( (m_constNameArray.GetAt(ii)).CompareNoCase(pszConstName) == 0)
			return TRUE;
	}
	return FALSE;
}

void ConstList::DeleteAll()
{
	m_constNameArray.RemoveAll();
	m_constTypeArray.RemoveAll();
	m_constValueArray.RemoveAll();
};	

/*
 *----------------------------------------------------------------------------*
 *  $History: ConstList.cpp $
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
