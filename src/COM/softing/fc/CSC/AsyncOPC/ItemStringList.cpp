/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/ItemStringList.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: ItemStringList.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/ItemStringList.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           AsyncOPC
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *
 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    administration of string lists seperated by '|' 
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.08.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "ItemStringList.h"
#include "SyncAccessGlobals.h"

//----  Static Initializations:   ----------------------------------*

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>CItemStrinList</B>
*
*	Description = constructor
*	<p>
*	Specification = 
*	<p>
*	@param strCommonPath prefix that is common to all items in strItemList
*	<p>
*	@param strItemList list of strings seperated by '|'
*	<p>
*
*/
CItemStringList::CItemStringList(const CString& strCommonPath, const CString& strItemList)
{
	m_strCommonPath=strCommonPath;
	CString strHelp=strItemList;
	Initialize(strHelp);
}

CItemStringList::~CItemStringList()
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
int CItemStringList::GetSize() const
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
CString CItemStringList::GetAt( int nIndex ) const
{
	return m_strCommonPath + m_astrList.GetAt(nIndex);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>GetAt</B>
*
*	Description = item at given position without common path
*	<p>
*	return	item
*	<p>
*
*/
CString CItemStringList::GetAtSimple( int nIndex ) const
{
	return m_astrList.GetAt(nIndex);
}

void CItemStringList::Initialize(const CString& strItems)
{
	TCHAR* pszItems = new TCHAR[strItems.GetLength()+1];
	//LPTSTR pszItems=(LPTSTR) (LPCTSTR) strItems;
	_tcscpy(pszItems,strItems);
   
	LPCTSTR pszItem = _tcstok(pszItems, gstrSeperator);
	CString strItem;
    while (pszItem != NULL)
    {		
		strItem = pszItem;
		m_astrList.Add(strItem);
        pszItem = _tcstok(NULL, gstrSeperator);
	}

	if (m_strCommonPath != "")
	{
		if ( m_strCommonPath[m_strCommonPath.GetLength()-1] != '.' )
			m_strCommonPath += '.';
	}

	delete pszItems;
}



/*
 *----------------------------------------------------------------------------*
 *  $History: ItemStringList.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 8.08.01    Time: 9:41
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * copied from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
