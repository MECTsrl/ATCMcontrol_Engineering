/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/ActiveX/Navigator/VariablePath.cpp 1     28.02.07 19:00 Ln $
*----------------------------------------------------------------------------*
*
* =FILENAME            $Workfile: VariablePath.cpp $
*                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/ActiveX/Navigator/VariablePath.cpp $
*
* =PROJECT             7VH102  ATCMControl
*
* =SWKE                4CSC
*
* =COMPONENT           simple sync OPC client
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
*
*----------------------------------------------------------------------------*
* =DESCRIPTION
* administration of string lists seperated by '|' 
*==
*----------------------------------------------------------------------------*
* =MODIFICATIONS
* $History: VariablePath.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP/ActiveX/Navigator
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CP/ActiveX/Navigator
 * 
 * *****************  Version 1  *****************
 * User: Scd          Date: 19.03.01   Time: 11:50
 * Created in $/4Control/COM/softing/fc/CP/ActiveX/Navigator
 * 
 * *****************  Version 1  *****************
 * User: So           Date: 28.09.99   Time: 9:41
 * Created in $/4Control/COM/softing/fc/CP/ActiveX/Navigator
 ******************************************************************************
H<<*/

#include "stdafx.h"
#include "VariablePath.h"

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>CItemStrinList</B>
*
*	Description = constructor
*	<p>
*	Specification = 
*	<p>
*	@param strPath list of strings seperated by '.'
*	<p>
*
*/
CVariablePath::CVariablePath( CString strPath)
{
	m_strPath=strPath;
	Initialize();
}

CVariablePath::~CVariablePath()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>GetSize</B>
*
*	Description = get number of items
*	<p>
*	return	number of items
*	<p>
*
*/
int CVariablePath::GetSize() const
{
	return m_astrList.GetSize();
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>GetAt</B>
*
*	Description = get common path (prefix) + item at given position
*	<p>
*	return	item
*	<p>
*
*/
CString CVariablePath::GetAt( int nIndex ) const
{
	return m_astrList.GetAt(nIndex);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>Pop</B>
*
*	Description = get first element
*	<p>
*	return	item
*	<p>
*
*/
CString CVariablePath::Pop()
{
	if (m_astrList.GetSize() <= 0 )
		return CString("");
	CString strFirst = m_astrList.GetAt(0);
	m_astrList.RemoveAt(0);
	return strFirst;
}

void CVariablePath::Initialize()
{
	TCHAR* pszItems = new TCHAR[m_strPath.GetLength()+1];
	//LPTSTR pszItems=(LPTSTR) (LPCTSTR) m_strPath;
	_tcscpy(pszItems, m_strPath);
   
	LPCTSTR pszItem = _tcstok(pszItems, gstrSeperator);
	CString strItem;
    while (pszItem != NULL)
    {		
		strItem = pszItem;
		m_astrList.Add(strItem);
        pszItem = _tcstok(NULL, gstrSeperator);
	}

	delete pszItems;
}

