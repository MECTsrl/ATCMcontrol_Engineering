/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/ItemStringList.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: ItemStringList.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/ItemStringList.h $
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

#ifndef __ITEMSTRINGLIST_H_
#define __ITEMSTRINGLIST_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "opcDA.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CItemStringList  
{
public:
	CItemStringList(const CString& strCommonPath, const CString& strItemList );
	virtual ~CItemStringList();
	int GetSize() const;
	CString GetAt( int nIndex ) const;
	CString GetAtSimple( int nIndex ) const;
	int GetGroupCount();
	BOOL GetGroup(UINT iGroup, CString& strItemList, CString& strCommonPath);

private:
	void Initialize(const CString& strItems);
	int InitializeGroups();

private:
	CString m_strCommonPath;
	CStringArray m_astrList;
	CStringArray m_astrValue;
	CUIntArray m_aiGroupNr;
};

#endif // __ITEMSTRINGLIST_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: ItemStringList.h $
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
