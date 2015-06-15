/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/Items.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: Items.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/Items.cpp $
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
 *    administration of opc items, their handles and their values (OPCITEMSTATES)
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
#include "Items.h"

//----  Static Initializations:   ----------------------------------*

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItems::CItems()
{

}

CItems::~CItems()
{
	//
	// Free Variants
	//
	OPCHANDLE hItem;
	OPCITEMSTATE itemState;
	::VariantInit(&itemState.vDataValue);
	POSITION pos = m_itemMap.GetStartPosition();
	while ( pos != NULL )
	{
		m_itemMap.GetNextAssoc( pos, hItem, itemState );
		::VariantClear(&itemState.vDataValue);
	}	
}

void CItems::SetHandle(const CString& strItem, OPCHANDLE hItem)
{
	m_handleMap.SetAt(strItem, hItem);
}

BOOL CItems::LookupHandle(const CString& strItem, OPCHANDLE& hItem)
{
	return m_handleMap.Lookup(strItem, hItem);
}

BOOL CItems::Remove(const CString& strItem)
{
	OPCHANDLE hItem;
	BOOL bItemfound = m_handleMap.Lookup(strItem, hItem);
	m_handleMap.RemoveKey(strItem);
	if ( bItemfound )
	{
		RemoveItemState(hItem);
	}
	return bItemfound;
}

void CItems::SetItemState(OPCHANDLE hItem, OPCITEMSTATE& itemState)
{
    // check params
    if (hItem == 0)
        return ;

	//
	// Remove old item state
	//
	BOOL bFound=RemoveItemState(hItem);
	//
	// 
	//
	OPCITEMSTATE newState;
	::VariantInit(&newState.vDataValue);
	CopyItemState(newState, itemState);
	m_itemMap.SetAt(hItem, newState);
}

void CItems::ClearItemState(OPCHANDLE hItem)
{
	//
	// Remove old item state
	//
	BOOL bFound=RemoveItemState(hItem);
	if (bFound)
		bFound=bFound;
}

BOOL CItems::LookupItemState(OPCHANDLE hItem, OPCITEMSTATE& itemState)
{
	OPCITEMSTATE newState;
	//::VariantInit(&newState.vDataValue); not needed since Lookup 
    //works on OPCITEMSTATE&newState.  
    //Lookup does not VariantCopy into newState, therefore 
    //we don't need VariantClear(&newState) either!
    //In other words: newState gets overwritten via memcpy!
	BOOL bFoundItem=m_itemMap.Lookup(hItem, newState);
	if ( bFoundItem )
	{
		CopyItemState(itemState, newState);
	}
	return bFoundItem;
}

BOOL CItems::LookupItemState(const CString& strItem, OPCITEMSTATE& itemState)
{
	OPCHANDLE hItem;
	BOOL bFoundHandle = m_handleMap.Lookup(strItem, hItem);
	if (bFoundHandle)
	{
		return LookupItemState(hItem, itemState);
	}

	return bFoundHandle;
}

int CItems::GetCount()
{
	return m_handleMap.GetCount();
}

BOOL CItems::RemoveItemState(OPCHANDLE hItem)
{

	OPCITEMSTATE itemState;
	//::VariantInit(&itemState.vDataValue); not needed since Lookup 
    //works on OPCITEMSTATE&itemState.  
    //Lookup does not VariantCopy into itemState, therefore 
    //we don't need VariantClear(&itemState) either unless bFound!
    //In other words: itemState gets overwritten via memcpy!
	BOOL bFound = m_itemMap.Lookup(hItem, itemState);

	if (bFound)
	{
		::VariantClear(&(itemState.vDataValue));
		m_itemMap.RemoveKey(hItem);
	}
	return bFound;
}

void CItems::CopyItemState(OPCITEMSTATE& destItem, OPCITEMSTATE& sourceItem)
{
	destItem.hClient = sourceItem.hClient;
	destItem.ftTimeStamp = sourceItem.ftTimeStamp;
	destItem.wQuality = sourceItem.wQuality;
	destItem.wReserved = sourceItem.wReserved;
	::VariantClear(&destItem.vDataValue);
	::VariantCopy(&destItem.vDataValue, &sourceItem.vDataValue);
}


/*
 *----------------------------------------------------------------------------*
 *  $History: Items.cpp $
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
