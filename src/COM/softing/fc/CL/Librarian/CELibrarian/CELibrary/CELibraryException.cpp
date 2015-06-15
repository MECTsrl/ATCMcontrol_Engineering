/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/CELibraryException.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CELibraryException.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/CELibraryException.cpp $
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
#include "resource.h"
#include "CELibraryException.h"
//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CELibraryExceptionId CELibraryException::ExIdList[] = 
	{
		{Err_Ok, 0},
		{Err_NoWriteAccess, IDS_CE_ERR_NOWRITEACCESS},
		{Err_POUAlreadyExist, IDS_CE_ERR_POUALREADYEXIST},
		{Err_POUNotExist, IDS_CE_ERR_POUNOTEXIST},
		{Err_FileNotFound, IDS_CE_ERR_FILENOTFOUND},
		{Err_CreateTempFile, IDS_CE_ERR_CREATETEMPFILE},
		{Err_OpenLib, IDS_CE_ERR_OPENLIB},
		{Err_FileFormat, IDS_CE_ERR_FILEFORMAT},
		{Err_WriteLibList, IDS_CE_ERR_WRITELIBLIST},
		{Err_AddPOU, IDS_CE_ERR_ADDPOU},
		{Err_Save, IDS_CE_ERR_SAVE},
		{Err_ReadItem, IDS_CE_ERR_READITEM},
		{Err_DeletePOU, IDS_CE_ERR_DELETEPOU},
		{Err_Unknown, IDS_CE_ERR_UNKNOWN},
		{Err_CloseLib, IDS_CE_ERR_CLOSELIB},
		{Err_InUse, IDS_CE_ERR_OPENLIB}
	};

/////////////////////////////////////////////////////////////////////
/**
 * constructor
 *
 * @param exId	exception identifier
 * @return		
 * @exception   
 * @see         
 */
CELibraryException::CELibraryException(int exId)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	m_cause = exId;
}

/////////////////////////////////////////////////////////////////////
/**
 * destructor
 *
 * @param
 * @return
 * @exception   
 * @see         
 */
CELibraryException::~CELibraryException()
{
 AFX_MANAGE_STATE(AfxGetAppModuleState( ));
}

/////////////////////////////////////////////////////////////////////
/**
 * get exception id
 *
 * @param 
 * @return		exception id 
 * @exception   
 * @see         
 */
int CELibraryException::GetCause()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));
	return m_cause;
}

/////////////////////////////////////////////////////////////////////
/**
 * get error message of exception
 *
 * @param 
 * @return		error message 
 * @exception   
 * @see         
 */
LPCTSTR CELibraryException::GetErrorMessage()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));
	
	static CString message = "";

	int errTextId = ExIdList[0].errTextId;

	for ( int ii = 0; ii< (sizeof(ExIdList)/sizeof(CELibraryExceptionId)) ; ii++)
	{
		if ( m_cause == ExIdList[ii].errId )
		{
			errTextId = ExIdList[ii].errTextId;
			break;
		}
	}

	message.LoadString(errTextId);
	return message;
}

/////////////////////////////////////////////////////////////////////
/**
 * open a message box with error message
 *
 * @param 
 * @return		void 
 * @exception   
 * @see         
 */
void CELibraryException::ReportError()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CString message = GetErrorMessage();
	AfxMessageBox( message, MB_ICONEXCLAMATION|MB_OK);
}

/*
 *----------------------------------------------------------------------------*
 *  $History: CELibraryException.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 3  *****************
 * User: Sis          Date: 24.05.02   Time: 10:22
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * bug fix 1615: replaced AfxGetStaticModuleState() by
 * AfxGetAppModuleState()
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:33p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 *==
 *----------------------------------------------------------------------------*
*/
