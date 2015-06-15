/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/inc/CELibraryException.h 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CELibraryException.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/inc/CELibraryException.h $
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

#ifndef __CELIBRARYEXCEPTION_H_
#define __CELIBRARYEXCEPTION_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



typedef struct 
{
	int errId;
	int errTextId;
} CELibraryExceptionId;

class __declspec(dllexport) CELibraryException  
{
public:

	typedef	enum errorID
	{		//error codes
		Err_Unknown=0,
		Err_Ok,
		Err_NoWriteAccess,
		Err_POUAlreadyExist,
		Err_POUNotExist,
		Err_FileNotFound,
		Err_CreateTempFile,
		Err_OpenLib,
		Err_FileFormat,
		Err_WriteLibList,
		Err_AddPOU,
		Err_Save,
		Err_ReadItem,
		Err_DeletePOU,
		Err_CloseLib,
		Err_DeletePOUFile,
		Err_InUse
	};

public:
	void ReportError();
	LPCTSTR GetErrorMessage();
	int GetCause();
	CELibraryException(int exId);
	virtual ~CELibraryException();

protected:
	int m_cause;
	static CELibraryExceptionId ExIdList[];
};


#endif // __CELIBRARYEXCEPTION_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: CELibraryException.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/inc
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:33p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/inc
 *==
 *----------------------------------------------------------------------------*
*/
