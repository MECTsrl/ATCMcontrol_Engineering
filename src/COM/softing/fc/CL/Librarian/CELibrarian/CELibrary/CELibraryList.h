/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/CELibraryList.h 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CELibraryList.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/CELibraryList.h $
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

#ifndef __CELIBRARYLIST_H_
#define __CELIBRARYLIST_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "CG_4CLAccess.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



// CELibraryList.h: Schnittstelle für die Klasse CELibraryList.
//
//////////////////////////////////////////////////////////////////////


class CELibraryItem;
//class CEStorage;

class CELibraryList  
{
public:
	CELibraryList();
	virtual ~CELibraryList();
	CELibraryItem* Add(const CString& itemName);
	int Delete(const CString& itemName);
	void SetAt(int iIndex, CELibraryItem* pCeItem);		// 13.12.06 SIS
	BOOL Exist(const CString& itemName);
	int GetSize();
	CELibraryItem* Get(int position);
	CELibraryItem* Get(const CString& itemName, int* position = NULL);
	void DeleteAll();
	//int WriteToStorage(CEStorage* pCeStorage);
	int ReadFromLibAcc(I4CLAccess* pILibAcc);
    CString GetPOUType(CString strLinkFile);
    void GetPOUHeaderFileNameAndByteCount(CString strLinkFile, CString & strHeaderFileName, CString & strByteCount);
private:
	CTypedPtrArray<CPtrArray, CELibraryItem*> m_itemArray;
	static CString      m_libList;
};

#endif // __CELIBRARYLIST_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: CELibraryList.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 13.12.06   Time: 15:22
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/celibrary
 * 13.12.06 SIS sort by (1) type and (2) name
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
