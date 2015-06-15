/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/CELibraryItem.h 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CELibraryItem.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/CELibraryItem.h $
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

#ifndef __CELIBRARYITEM_H_
#define __CELIBRARYITEM_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



// CELibraryItem.h: Schnittstelle für die Klasse CELibraryItem.
//
//////////////////////////////////////////////////////////////////////


//class CEStream;

class CELibraryItem  
{
public:

	CELibraryItem(const CString& itemName);
	virtual ~CELibraryItem();
	
	int AddFile(const CString& fileName);
	int DeleteFile(const CString& fileName);
	BOOL ExistFile(const CString& fileName);
	CString GetName();
	CString GetType();
	CString GetFileName(const CString& fileType);
	CString GetFileName(int position);
	int GetFileCount();
	//void Write(CEStream* pCeStream);

	BOOL SetProperty(const CString& key, const CString& value);
	BOOL GetProperty(const CString& key, CString& value);
	int GetProperty(int position, CString& key, CString& value);
	BOOL DeleteProperty(const CString& key);
	int GetPropertyCount();

private:
	CString m_itemName;
	CString m_itemType;
	CStringArray m_fileNames;
	CMapStringToString m_properties;
};


#endif // __CELIBRARYITEM_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: CELibraryItem.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 3  *****************
 * User: Sis          Date: 13.12.06   Time: 15:21
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/celibrary
 * 13.12.06 SIS roll back
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 13.12.06   Time: 15:19
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
