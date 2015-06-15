/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/Items.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: Items.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/Items.h $
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
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.08.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __ITEMS_H_
#define __ITEMS_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "opcDA.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
typedef CMap<CString, LPCTSTR, OPCHANDLE, OPCHANDLE&>   handleMap;
typedef CMap<OPCHANDLE, OPCHANDLE, OPCITEMSTATE, OPCITEMSTATE&> itemMap;

class CItems  
{
public:
	CItems();
	virtual ~CItems();
	void SetHandle(const CString& strItem, OPCHANDLE hItem);
	BOOL LookupHandle(const CString& strItem, OPCHANDLE& hItem);
	BOOL Remove(const CString& strItem);
	void SetItemState(OPCHANDLE hItem, OPCITEMSTATE& itemState);
	void ClearItemState(OPCHANDLE hItem);
	BOOL LookupItemState(const CString& strItem, OPCITEMSTATE& itemState);
	BOOL LookupItemState(OPCHANDLE hItem, OPCITEMSTATE& itemState);
	
	int GetCount();

protected:
	BOOL RemoveItemState(OPCHANDLE hItem);
	static void	CopyItemState(OPCITEMSTATE& destItem, OPCITEMSTATE& sourceItem);

private:
	handleMap m_handleMap;
	itemMap m_itemMap;
};

#endif // __ITEMS_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: Items.h $
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
