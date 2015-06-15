/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/ItemStringList.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: ItemStringList.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/ItemStringList.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           simple sync OPC client
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

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>GetGroupCount</B>
*
*	Description = get number of groups, i.e. number of items that have the same machine name
*				  the common path is ignoread, format of items is [machine name].XXXXX
*	<p>
*	return	number of groups
*	<p>
*
*/
int CItemStringList::GetGroupCount()
{
	int siGroupCount=InitializeGroups();

	return siGroupCount;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>GetGroupCount</B>
*
*	Description = get items of given group number as list seperated by '|',
*	<p>
*	
*	return	number of groups
*	<p>
*
*/
BOOL CItemStringList::GetGroup(UINT iGroup, CString& strItemList, CString& strCommonPath)
{
	strItemList = "";
	strCommonPath = "";

	if ( ((UINT) GetGroupCount()) <iGroup)
	{
		return FALSE;
	}

	CString strConfig, strPureName;
	for (int ii=0; ii<GetSize(); ii++)
	{
		if (m_aiGroupNr.GetAt(ii) == iGroup)
		{
			strConfig=GetConfigName(m_astrList.GetAt(ii), strPureName);
			strItemList = strPureName;
			break;
		}
	}

	for (int jj=ii+1; jj<GetSize(); jj++)
	{
		if (m_aiGroupNr.GetAt(jj) == iGroup)
		{
			strConfig=GetConfigName(m_astrList.GetAt(jj), strPureName);
			strItemList += gstrSeperator + strPureName;
		}
	}

	strCommonPath=strConfig + ".";

	return TRUE;
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

int CItemStringList::InitializeGroups()
{
	
	CString strPureMachineName, strRemainder;
	CStringArray astrGroupName;
	
	m_aiGroupNr.RemoveAll();
	//
	// get machine names (group names)
	//
	for (int ii=0; ii<GetSize(); ii++)
	{
		SplitMachineName(m_astrList.GetAt(ii), strPureMachineName, strRemainder);
		strPureMachineName.MakeLower();
		astrGroupName.Add(strPureMachineName);
		m_aiGroupNr.Add(0);
	}


	//
	// get group numbers
	//
	int iGroupNr=0;
	for (ii=0; ii<GetSize(); ii++)
	{
		CString strGroup=astrGroupName.GetAt(ii);
		if (m_aiGroupNr.GetAt(ii)==0)
		{
			iGroupNr++;
			m_aiGroupNr.SetAt(ii,iGroupNr);
			for (int jj=ii+1; jj<GetSize(); jj++)
			{
				if (strGroup == astrGroupName.GetAt(jj))
				{
					m_aiGroupNr.SetAt(jj, iGroupNr);
				}
			}
		}
	}

	//
	// normalize Group numbers
	//
	for (ii=0; ii<GetSize(); ii++)
	{
		int iTemp=m_aiGroupNr.GetAt(ii);
		_ASSERTE(iTemp >0);
		m_aiGroupNr.SetAt(ii, iTemp-1);
	}

	return iGroupNr;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: ItemStringList.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 8.08.01    Time: 9:59
 * Created in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * copied from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
