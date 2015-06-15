/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/CELibrary.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CELibrary.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/CELibrary.cpp $
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
#include "utilif.h"
#include "direct.h"
#include "resource.h"
#include "CELibrary.h"
#include "CELibraryException.h"
#include "CEFile.h"
#include "CELibraryList.h"
#include "CELibraryItem.h"
#include "CEPassWord.h"
#include "ConstList.h"
#include "ConstFileParser.h"
#include "CEL_Names.h"
//----  Static Initializations:   ----------------------------------*





#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CELibErrId CELibrary::ErrIdList[] = 
	{
		{Err_NotOk, IDS_CE_ERR},
		{Err_Ok, 0},
		{Err_NoWriteAccess, IDS_CE_ERR_NOWRITEACCESS},
		{Err_POUAlreadyExist, IDS_CE_ERR_POUALREADYEXIST},
		{Err_POUNotExist, IDS_CE_ERR_POUNOTEXIST},
		{Err_FileNotFound, IDS_CE_ERR_FILENOTFOUND},
		{Err_POUFileNotExist, IDS_CE_ERR_POUFILENOTEXIST},
		{Err_DirectoryNotExist, IDS_CE_ERR_DIRECTORYNOTEXIST},
		{Err_WrongPOUType, IDS_CE_ERR_WRONGPOUTYPE},
		{Err_WrongHeaderDest, IDS_CE_ERR_WRONGHEADERDEST},
		{Err_WrongUsesEntry, IDS_CE_ERR_WRONGUSEENTRY},
		{Err_CoCreateFailed, IDS_CE_ERR_COCREATEFAILED},
		{Err_ObjConvert, IDS_CE_ERR_OBJCONVERT}
	};

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CELibrary::CELibrary()
{
	
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	m_fileName="";
	//m_pWorkingStorage = NULL;
	//m_pOrigStorage = NULL;
	m_pLibList = new CELibraryList();
	m_readOnly = 0;
	m_modeRead=FALSE;
	m_modeWrite=FALSE;
	m_modeTransacted=FALSE;
	m_modeInteractive=FALSE;
	m_modeCreate=FALSE;
	
	SetModified(FALSE);

	m_pConstList = new ConstList();
	m_bConstParsed = FALSE;

	m_pObjConvert=NULL;
    m_pILibAcc = NULL;
}

CELibrary::~CELibrary()
{
	
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));
	
	//
	// release working library, file is deleted automatically
	//
	//if (m_pWorkingStorage != 0)
	//	delete m_pWorkingStorage;

	//if (m_pOrigStorage != 0)
	//	delete m_pOrigStorage;

	if (m_pLibList != 0)
		delete m_pLibList;

	if (m_pConstList != 0)
		delete m_pConstList;

	if (m_pObjConvert != NULL)
		m_pObjConvert->Release();
}

/////////////////////////////////////////////////////////////////////
/**
 * Open the library with the specified file
 * 
 * @param fileName  file name of ATCMControl library
 * @param openMode  mode for opening, "r" read, "rw" read/write,
 *				"rwt" read/write/transacted, "c" create
 * 
 * @return 0-1  Err_Ok, Err_NotOk
 * @exception   CELibraryException, if file not found or temporary 
 *				storage could not be allocated
 * @see         ...
 */
int CELibrary::Open(LPCTSTR fileName, LPCTSTR  popenMode)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
    
    HRESULT                   hr;
    BSTR                      bstr      = NULL;
    BSTR                      bstrErr   = NULL;
    CString                   strLibFullPath;
    bool           bWrite = false;
    I4CLOpen*      pILibOpen = NULL;
    //create lib access instance:
    hr = CoCreateInstance(
              CLSID_FC4CLAccess,
              NULL, 
              CLSCTX_INPROC_SERVER,
              IID_I4CLOpen, 
              (void**)&pILibOpen
         );    
    if(FAILED(hr))
    {
        return NULL;
    }
    //open for read access:
    strLibFullPath = fileName;
    ASSERT(!strLibFullPath.IsEmpty());
    bstr  = strLibFullPath.AllocSysString();
    hr    = pILibOpen->Open(bWrite, bstr, &bstrErr, &m_pILibAcc);
    if(FAILED(hr))
    {
        CString strMsg = bstrErr;
        AfxMessageBox(strMsg, MB_ICONEXCLAMATION|MB_OK);
        SysFreeString(bstr);
        SysFreeString(bstrErr);

        return NULL;
    }
    ASSERT(m_pILibAcc && !bstrErr);
    SysFreeString(bstr);
    CString           jot;
    if(!m_pILibAcc)
    {
        return 0;
    }
    m_pLibList->ReadFromLibAcc(m_pILibAcc);
    //hr = pILibOpen->Close(m_pILibAcc);
    return SetError(Err_Ok);
}

/////////////////////////////////////////////////////////////////////
/**
 * commit changes to a ATCMControl library, only valid in transacted mode
 * @return 0-1  Err_Ok ok; Err_NotOk not ok 
 * @exception   CELibraryException, if temporary lib coud not be written
 *				to original lib
 * @see         
 */
int CELibrary::Commit()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	/*if ( ! m_modeTransacted )
		return SetError(Err_NotOk);

	if (m_fileName == "")
		return SetError(Err_NotOk);

	//
	// close original storage, was only opened for reading
	//
	if ( m_pOrigStorage != NULL )
	{
		delete m_pOrigStorage;
		m_pOrigStorage = NULL;
	}

	//
	// if original storage does not exist, create it
	//
	if ( m_pOrigStorage == NULL )
	{
		m_pOrigStorage = new CEStorage();
		HRESULT rv = m_pOrigStorage->AssignFileToStorage(m_fileName,
			STGM_DIRECT | STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE);
		if ( rv != S_OK )
		{
			throw CELibraryException(CELibraryException::Err_Save);
			return 0;
		}
	}

	//
	// write contents of working lib into original lib
	//
	int rv = SetError(WriteWorkingLib2OrigLib());

	//
	// close original storage
	//
	if ( m_pOrigStorage != NULL )
	{
		delete m_pOrigStorage;
		m_pOrigStorage = NULL;
	}

	SetModified(FALSE);
	return rv;*/
    return 0;
}

/////////////////////////////////////////////////////////////////////
/**
 * save ATCMControl library in different file 
 * only valid in transacted mode
 * @param fileName file name to save to
 * @param bChangeName if true, name of library is set to fileName
 * @return 0-1  1 ok; 0 not ok 
 * @exception   CELibraryException, if temporary lib coud not be written
 *				to original lib
 * @see         
 */
int CELibrary::SaveAs(LPCTSTR fileName, BOOL bChangeName)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	/*if (! m_modeTransacted)
		return SetError(Err_NotOk);

	//
	//  create new original storage
	//
	CEStorage* m_pNewOrigStorage = new CEStorage();
	HRESULT rv = m_pNewOrigStorage->AssignFileToStorage(fileName,
	STGM_DIRECT | STGM_SHARE_EXCLUSIVE | STGM_READWRITE |STGM_CREATE);
	if ( rv != S_OK )
	{
		delete m_pNewOrigStorage;
		throw CELibraryException(CELibraryException::Err_Save);
		return 0;
	}
	//
	// delete old one and assign new storage to it
	//
	delete m_pOrigStorage;
	m_pOrigStorage = m_pNewOrigStorage;
	CString strOldName=m_fileName;
	m_fileName=fileName;
	//
	// save working storage
	//
	Commit();

	if (bChangeName==FALSE)
		m_fileName=strOldName;
	
	return SetError(Err_Ok);*/
    return 0;
}
	
/////////////////////////////////////////////////////////////////////
/**
 * close a ATCMControl library
 * @return 0-1  1 ok; 0 not ok 
 * @exception   CELibraryException, if temporary storage could not 
 *				be closed, is already closed
 * @see         
 */
int CELibrary::Close()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	/*if ( m_pWorkingStorage == NULL )
	{
		throw CELibraryException(CELibraryException::Err_CloseLib);
		return 0;
	}
	
	if ( m_modeWrite )
		WriteLibList(m_pWorkingStorage);
	
	delete m_pWorkingStorage;
	m_pWorkingStorage = NULL;

	if (m_pOrigStorage != NULL)
	{
		delete m_pOrigStorage;
		m_pOrigStorage=NULL;
	}*/

	return SetError(Err_Ok);
}

/////////////////////////////////////////////////////////////////////
/**
 * test if working lib is open
 * @return 0-1  1 open; 0 not open 
 * @exception    
 * @see         
 */
int CELibrary::IsOpen()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	/*if ( m_pWorkingStorage != NULL )
		return 1;
	else*/
		return 0;
}


/////////////////////////////////////////////////////////////////////
/**
 * Read the contents of the library, i.e. read the stream "lib.list"
 *
 * @return 0-1  1 ok; 0 not ok 
 * @exception   CELibraryException, if stream "lib.list" can't be
 *				opened.
 *				Assertion if original library is not opened.
 * @see         WriteLibList()
 */
/*int CELibrary::ReadLibList(CEStorage* pCeStorage)
{
	return m_pLibList->ReadFromStorage(pCeStorage);
}*/

/////////////////////////////////////////////////////////////////////
/**
 * Write the table contents to library, i.e. write the stream lib.list.
 *
 * @return 0-1  1 ok; 0 not ok 
 * @exception   CELibraryException, if stream lib.list can't be written
 * @see         ReadLibList()
 */
/*int CELibrary::WriteLibList(CEStorage* pCeStorage)
{
	return m_pLibList->WriteToStorage(pCeStorage);
}*/

/////////////////////////////////////////////////////////////////////
/**
 * Restore the working library to the original library. The contents
 * of the original library ar overwritten.
 *
 * @return 0-1  1 ok; 0 not ok
 * @exception   CELibraryException if contents of working library can't
 *				be restored
 * @see         
 */
int CELibrary::WriteWorkingLib2OrigLib()
{
	//
	// delete information in original storage, i.e. all substorages
	//
	/*m_pOrigStorage->DestroySubStorages();
	
	//
	// delete lib.list
	//
	m_pOrigStorage->DeleteElement(CEL_LIBLIST);

	//
	// copy working lib to original storage
	//
	try
	{
		m_pWorkingStorage->CopySubStorages(m_pOrigStorage, STGTY_STORAGE);
	}
	catch ( CELibraryException ex )
	{
		throw ex;
		return 0;
	}

	//
	// write lib.list
	//
	WriteLibList(m_pOrigStorage);*/
	return SetError(Err_Ok);
}

/////////////////////////////////////////////////////////////////////
/**
 * Copy the file(s) of an library item into the library/storage.
 *
 * @param pCeStorage	pointer to cestorage
 * @param pouName		name of library item
 * @param fullFileName  file name with contents of library items
 * @param libName		name of the file in library, if different from fullFileName
 * @return 0-1  1 ok; 0 not ok 
 * @exception   CELibraryException, if storage can't be written or
				file can't be read
 * @see         AddPOU()
 */
/*int CELibrary::CopyFiles2Lib(CEStorage * pCeStorage, CString pouName, CString fullFileName, CString libName, unsigned long byteOffsetStart, unsigned long nBytes)
{
	//
	// Open file for reading
	//
	if ( ! CEFile::IsFile(fullFileName) )
	{
		throw CELibraryException(CELibraryException::Err_FileNotFound);
	} 
	try
	{
		CStdioFile file(fullFileName, CFile::modeRead|CFile::typeBinary);

		//
		// open or create new storage with name pouName below pCeStorage
		//
		CEStorage ceStorageNew;
		HRESULT rv = pCeStorage->OpenStorage(pouName, 
					STGM_READWRITE | STGM_SHARE_EXCLUSIVE,&ceStorageNew);
		if ( rv == STG_E_FILENOTFOUND )
		{
			HRESULT rv = pCeStorage->CreateStorage(pouName, 
						STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,
						&ceStorageNew);
			if ( rv != S_OK )
			{
				file.Close();
				throw CELibraryException(CELibraryException::Err_AddPOU);
			}
		}

		//
		// create new stream below new storage
		//
		CString fileName;
		if ( libName == "" )
			fileName = CEFile::GetFileName(fullFileName);
		else
			fileName = libName;
		CEStream ceStream;
		rv = ceStorageNew.CreateStream(fileName, 
				STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,
				&ceStream);
		if ( rv != S_OK )
		{
			file.Close();
			throw CELibraryException(CELibraryException::Err_AddPOU);
		}	

		//
		// Copy File to Stream 
		//
		ceStream.ImportFile(file, byteOffsetStart, nBytes);
		file.Close();
	}
	catch (CFileException e)
	{
		throw CELibraryException(CELibraryException::Err_FileNotFound);
	}
	return SetError(Err_Ok);
}*/

/////////////////////////////////////////////////////////////////////
/**
 * Copy the buffer to a file of an library item into the library/storage.
 *
 * @param pCeStorage	pointer to cestorage
 * @param pouName		name of library item
 * @param pszBuffer		the buffer
 * @param libName		name of the file in library
 * @byteOffsetStart		offset (in characters) in buffer, where to start copying
 * @nBytes				number of characters to copy (method does unicode handling for you)
 * @return 0-1  1 ok; 0 not ok 
 * @exception   CELibraryException, if storage can't be written or
				file can't be read
 * @see         AddPOU()
 */
/*int CELibrary::CopyBuffer2Lib(CEStorage * pCeStorage, CString pouName, LPCTSTR pszBuffer, CString libName, unsigned long byteOffsetStart, unsigned long nBytes)
{
	try
	{
		//
		// open or create new storage with name pouName below pCeStorage
		//
		CEStorage ceStorageNew;
		HRESULT rv = pCeStorage->OpenStorage(pouName, 
					STGM_READWRITE | STGM_SHARE_EXCLUSIVE,&ceStorageNew);
		if ( rv == STG_E_FILENOTFOUND )
		{
			HRESULT rv = pCeStorage->CreateStorage(pouName, 
						STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,
						&ceStorageNew);
			if ( rv != S_OK )
			{
				throw CELibraryException(CELibraryException::Err_AddPOU);
			}
		}

		//
		// create new stream below new storage
		//
		CEStream ceStream;
		rv = ceStorageNew.CreateStream(libName, 
				STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,
				&ceStream);
		if ( rv != S_OK )
		{
			throw CELibraryException(CELibraryException::Err_AddPOU);
		}	

		//
		// Copy File to Stream 
		//
		ceStream.ImportBuffer(pszBuffer, byteOffsetStart, nBytes);
	}
	catch (CFileException e)
	{
		throw CELibraryException(CELibraryException::Err_FileNotFound);
	}
	return SetError(Err_Ok);
}*/

/////////////////////////////////////////////////////////////////////
/**
 * return the (path) name of the library.
 *
 * @return		file name of library
 * @exception   
 * @see         
 */
LPCTSTR CELibrary::GetName()
{
	return m_fileName;
}

/////////////////////////////////////////////////////////////////////
/**
 * Add a new item to the (working) library.
 *
 * @param name			name of item
 * @param type			type of item
 * @return		Err_POUAlreadyExist: item with name exists already in lib;
 *              Err_Ok: ok
 * @exception   CELibraryException, if file was not found, file can't be added
 *				to working lib
 * @see         
 */
int CELibrary::AddPOU(LPCTSTR name, LPCTSTR type)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	if ( ExistPOU(name) )
		return SetError(Err_POUAlreadyExist,name);

	AddInternalPOU(name,type);
	SetModified(TRUE);

	return SetError(Err_Ok);
}

/////////////////////////////////////////////////////////////////////
/**
 * Add a new file to item in the (working) library.
 *
 * @param name			name of item
 * @param fullFileName  file belonging to the item
 * @return		Err_POUAlreadyExist: item with name exists already in lib;
 *              Err_Ok: ok
 * @exception   CELibraryException, if file was not found, file can't be added
 *				to working lib
 * @see         
 */
int CELibrary::AddPOUFile(LPCTSTR name, LPCTSTR fullFileName)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	if ( ! ExistPOU(name) )
		return SetError(Err_POUNotExist,name);

	//
	// make entry in working storage
	//
	try
	{
		//
		// copy the files of lib item into lib
		//
		//CopyFiles2Lib(m_pWorkingStorage, name, fullFileName);
		
		//
		// get new file name
		//
		CString pouLibFileName=GetFileNameInLib(name,fullFileName);
		
		//
		// add filename to internal list
		//
		AddInternalPOUFile(name,pouLibFileName);
		SetModified(TRUE);

	}
	catch ( CELibraryException ex )
	{
		throw ex;
	}

	return SetError(Err_Ok);
}

/////////////////////////////////////////////////////////////////////
/**
 * Add a new file to item in the (working) library.
 *
 * @param name			name of item
 * @param fullFileName  file belonging to the item
 * @param byteOffsetStart
 * @param nBytes
 * @return		Err_POUAlreadyExist: item with name exists already in lib;
 *              Err_Ok: ok
 * @exception   CELibraryException, if file was not found, file can't be added
 *				to working lib
 * @see         
 */
int CELibrary::AddPOUFile(LPCTSTR name, LPCTSTR fullFileName, LPCTSTR libName,unsigned long byteOffsetStart, unsigned long nBytes)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	if ( ! ExistPOU(name) )
		return SetError(Err_POUNotExist,name);

	//
	// make entry in working storage
	//
	try
	{
		//
		// copy the files of lib item into lib
		//
		//CopyFiles2Lib(m_pWorkingStorage, name, fullFileName, libName, byteOffsetStart,nBytes);
		
		//
		// get new file name
		//
		CString pouLibFileName;
		if (libName == "" )
			pouLibFileName=GetFileNameInLib(name,fullFileName);
		else
			pouLibFileName=libName;		
		//
		// add filename to internal list
		//
		AddInternalPOUFile(name,pouLibFileName);
		SetModified(TRUE);
	}
	catch ( CELibraryException ex )
	{
		throw ex;
	}

	return SetError(Err_Ok);
}

/////////////////////////////////////////////////////////////////////
/**
 * Add a new file to item in the (working) library.
 *
 * @param name			name of item
 * @param pszBuffer		buffer with contents to write to pou file
 * @param libName		name of pou file
 * @param byteOffsetStart
 * @param nBytes
 * @return		Err_POUAlreadyExist: item with name exists already in lib;
 *              Err_Ok: ok
 * @exception   CELibraryException, if file was not found, file can't be added
 *				to working lib
 * @see         
 */
int CELibrary::AddPOUFileBuffer(LPCTSTR name, LPCTSTR pszBuffer, LPCTSTR libName,unsigned long byteOffsetStart, unsigned long nBytes)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	if ( ! ExistPOU(name) )
		return SetError(Err_POUNotExist,name);

	//
	// make entry in working storage
	//
	try
	{
		//
		// copy the files of lib item into lib
		//
		//CopyBuffer2Lib(m_pWorkingStorage, name, pszBuffer, libName, byteOffsetStart,nBytes);
		
		//
		// add filename to internal list
		//
		AddInternalPOUFile(name,libName);
		SetModified(TRUE);
	}
	catch ( CELibraryException ex )
	{
		throw ex;
	}

	return SetError(Err_Ok);
}

/////////////////////////////////////////////////////////////////////
/**
 * Delete an item in the (working) library.
 *
 * @param name			name of item
 * @return		Err_POUNotExist: item with name does not exist in lib;
 *              Err_Ok: ok
 * @exception   CELibraryException, if file was not found, file can't be deleted
 *				in working lib
 * @see         
 */
int CELibrary::DeletePOU(LPCTSTR name)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	if ( ! ExistPOU(name) )
		return SetError(Err_POUNotExist,name);

	int numFiles = GetPOUFileCount(name);
	//
	// destroy storage name
	//
	/*HRESULT rv = m_pWorkingStorage->DeleteElement(name);
	if ( rv != S_OK )
	{
		AnalyzeHRESULT(rv);
		if ( numFiles >0 || rv != STG_E_FILENOTFOUND)
		{
			throw CELibraryException(CELibraryException::Err_DeletePOU);
			return 0;
		}
	}*/

	//
	// delete name from internal list
	//
	int nn = DeleteInternalPOU(name);
	ASSERT(nn);
	SetModified(TRUE);

	return SetError(Err_Ok);
}

/////////////////////////////////////////////////////////////////////
/**
 * Delete an item in the (working) library.
 *
 * @param pouName			name of item
 * @param pouFileName		name of item file
 * @return		Err_POUNotExist: item with name does not exist in lib;
 *              Err_Ok: ok
 * @exception   CELibraryException, if file was not found, file can't be deleted
 *				in working lib
 * @see         
 */
int CELibrary::DeletePOUFile(LPCTSTR pouName, LPCTSTR pouFileName)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	if ( ! ExistPOUFile(pouName,pouFileName) )
		return SetError(Err_POUFileNotExist,pouFileName);

	//
	// Open storage pouName
	//
	/*CEStorage ceStorage;
	HRESULT rv = m_pWorkingStorage->OpenStorage(pouName
					,STGM_READWRITE | STGM_SHARE_EXCLUSIVE
					,&ceStorage);
	if ( rv != S_OK )
	{
		throw CELibraryException(CELibraryException::Err_DeletePOUFile);
		return 0;
	}

	//
	// Delete element pouFileName
	//
	rv = ceStorage.DeleteElement(pouFileName);
	AnalyzeHRESULT(rv);
	if ( rv != S_OK )
	{
		throw CELibraryException(CELibraryException::Err_DeletePOUFile);
		return 0;
	}*/

	//
	// delete name from internal list
	//
	int nn = DeleteInternalPOUFile(pouName,pouFileName);
	ASSERT(nn);
	SetModified(TRUE);

	//
	// storage closed automatically
	//
	return SetError(Err_Ok);
}

/////////////////////////////////////////////////////////////////////
/**
 * Get name, type and fileName of library item at spec. position
 *
 * @param pos		position of library item [in]
 * @param name		name of library item [out]
 * @param type		type of library item [out]
 * @return			void
 * @exception		-
 * @see         
 */
void CELibrary::GetPOU(int pos, LPTSTR name, LPTSTR type)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CELibraryItem* pCeItem=m_pLibList->Get(pos);
	CString name1 = pCeItem->GetName();
	CString type1 = pCeItem->GetType();
	_tcscpy(name,name1);
	_tcscpy(type,type1);
}

/////////////////////////////////////////////////////////////////////
/**
 * Get name of library item at spec. position
 *
 * @param pos		position of library item [in]
 * @return			name of library item
 * @exception		-
 * @see         
 */
LPCTSTR CELibrary::GetPOUName(int pos)
{
	static CString name="";
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CELibraryItem* pCeItem=m_pLibList->Get(pos);
	name = pCeItem->GetName();
	return name;
}

/////////////////////////////////////////////////////////////////////
/**
 * Get file name of specified library item
 *
 * @param pouName	name of library item [in]
 * @param fileName	file name of library item 'name' [out]
 * @return 0-1		1 item found; 0 item not found 
 * @exception   
 * @see         
 */
int CELibrary::GetPOUFile(LPCTSTR pouName, LPTSTR fileName, LPCTSTR fileExt/*=ST*/)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	fileName[0]='\0';
	CELibraryItem* pCeItem=m_pLibList->Get(pouName);
	if (pCeItem != NULL)
	{
		CString fileName1=pCeItem->GetFileName(fileExt);
		_tcscpy(fileName,fileName1);
		if (fileName1=="")
			return 0;
		else
			return 1;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////
/**
 * Get number of files of pou
 *
 * @param pouName	name of library item [in]
 * @return 			number of files, -1 if pou not found 
 * @exception   
 * @see         
 */
int CELibrary::GetPOUFileCount(LPCTSTR pouName)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CELibraryItem* pCeItem=m_pLibList->Get(pouName);
	if ( pCeItem == NULL )
		return -1;
	return pCeItem->GetFileCount();
}

/////////////////////////////////////////////////////////////////////
/**
 * Get file name of specified library item at spec. position
 *
 * @param pouName	name of library item [in]
 * @param position  position of file
 * @return 			name of library item file, "" if pou not found
 * @exception   
 * @see         
 */
LPCTSTR CELibrary::GetPOUFileName(LPCTSTR pouName, int position)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	static CString fileName="";
	CELibraryItem* pCeItem=m_pLibList->Get(pouName);
	if ( pCeItem == NULL )
		fileName="";
	else
		fileName = pCeItem->GetFileName(position);
	return fileName;
}

/////////////////////////////////////////////////////////////////////
/**
 * Get type of specified library item
 *
 * @param name		name of library item [in]
 * @param type		type of library item [out]
 * @return 0-1		1 item found; 0 item not found
 * @exception   
 * @see         
 */
int CELibrary::GetPOUType(LPCTSTR pouName, LPTSTR type)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	type[0]='\0';
	CELibraryItem* pCeItem=m_pLibList->Get(pouName);
	if (pCeItem != NULL)
	{
		CString type1 = pCeItem->GetType();
		_tcscpy(type,type1);
		return SetError(Err_Ok);
	}
	return SetError(Err_NotOk);
}

/////////////////////////////////////////////////////////////////////
/**
 * Get number of library items
 *
 * @return		number of library items
 * @exception   
 * @see         
 */
int CELibrary::GetPOUCount()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	return m_pLibList->GetSize();
}

/////////////////////////////////////////////////////////////////////
/**
 * Test if specified item exists in library
 *
 * @param pouName	name of item [in]
 * @return 0-1  1 item exists in lib; item does not exist in lib
 * @exception   
 * @see        
 */
BOOL CELibrary::ExistPOU(LPCTSTR pouName)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	return m_pLibList->Exist(pouName);
}

/////////////////////////////////////////////////////////////////////
/**
 * Test if specified item file exists in library
 *
 * @param pouName	name of item [in]
 * @param pouFileName name of item file
 * @return 0-1  1 item file exists in lib; item does not exist in lib
 * @exception   
 * @see        
 */
BOOL CELibrary::ExistPOUFile(LPCTSTR pouName, LPCTSTR pouFileName)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CELibraryItem* pCeItem = m_pLibList->Get(pouName);
	if ( pCeItem == NULL)
		return FALSE;

	return pCeItem->ExistFile(pouFileName);
}

/////////////////////////////////////////////////////////////////////
/**
 * Delete all library items in internal list
 *
 * @return		void
 * @exception   
 * @see         
 */
void CELibrary::DeleteAllPOU()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	m_pLibList->DeleteAll();
	m_pConstList->DeleteAll();
	SetModified(TRUE);
}

/////////////////////////////////////////////////////////////////////
/**
 * checkout/write all pou files into specified directory
 *
 * @param pouName	name of item [in]
 * @param directory name of directory [in]
 * @return 0-1  1 ok, !=1 not ok
 * @exception   
 * @see        
 */
int CELibrary::CheckoutPou(LPCTSTR ppouName, LPCTSTR pdirectory)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CString pouName=ppouName;
	CString directory=pdirectory;
	//
	// test directory
	//
	if ( ! CEFile::IsDirectory(directory) )
		return SetError(Err_DirectoryNotExist,directory);

	//
	// get pou
	//
	CELibraryItem* pCeItem=m_pLibList->Get(pouName);
	if ( pCeItem == NULL)
		return SetError(Err_POUNotExist,pouName);

	//
	// open pou storage
	//
	/*CEStorage ceStorage;
	HRESULT rv = m_pWorkingStorage->OpenStorage(pouName,
		STGM_READ | STGM_SHARE_EXCLUSIVE, 
		&ceStorage );
	AnalyzeHRESULT(rv);
	if ( rv != S_OK )
	{
		throw CELibraryException(CELibraryException::Err_ReadItem);
	}

	//
	// checkout files
	//
	CString fileName, fullFileName;
	CStdioFile file;
	for (int ii=0;ii< pCeItem->GetFileCount();ii++)
	{
		fileName = pCeItem->GetFileName(ii);
		//
		// open stream 
		//
		CEStream ceStream;
		rv = ceStorage.OpenStream(fileName, 
			STGM_READ|STGM_SHARE_EXCLUSIVE,
			&ceStream);
		if ( rv != S_OK )
		{
			throw CELibraryException(CELibraryException::Err_ReadItem);
		}

		//
		// construct new file name and export stream to it
		//
		fullFileName = directory + "\\" + fileName;
		if ( file.Open(fullFileName,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary ) )
		{
			ceStream.ExportFile(file);
			file.Close();
		}
	}*/

	return SetError(Err_Ok);
}

/////////////////////////////////////////////////////////////////////
/**
 * checkout/write a pou file into specified directory
 *
 * @param pouName	name of item [in]
 * @param pouFile	name of item file [in]
 * @param directory name of directory [in]
 * @return 0-1  1 ok, !=1 not ok
 * @exception   
 * @see        
 */
int CELibrary::CheckoutPouFile(LPCTSTR pouName,LPCTSTR ppouFile,LPCTSTR pdirectory)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CString pouFile = ppouFile;
	CString directory = pdirectory;
	//
	// test directory
	//
	if ( ! CEFile::IsDirectory(directory) )
		return SetError(Err_DirectoryNotExist,directory);

	//
	// get pou
	//
	CELibraryItem* pCeItem=m_pLibList->Get(pouName);
	if ( pCeItem == NULL)
		return SetError(Err_POUNotExist,pouName);

	//
	// test existance of pou file
	//
	if ( ! pCeItem->ExistFile(pouFile) )
		return SetError(Err_POUFileNotExist,pouFile);

	//
	// open pou storage
	//
	/*CEStorage ceStorage;
	HRESULT rv = m_pWorkingStorage->OpenStorage(pouName,
		STGM_READ | STGM_SHARE_EXCLUSIVE, 
		&ceStorage );
	AnalyzeHRESULT(rv);
	if ( rv != S_OK )
	{
		throw CELibraryException(CELibraryException::Err_ReadItem);
	}

	//
	// checkout files
	//
	CString fullFileName;
	CStdioFile file;

	//
	// open stream 
	//
	CEStream ceStream;
	rv = ceStorage.OpenStream(pouFile, 
		STGM_READ|STGM_SHARE_EXCLUSIVE,
		&ceStream);
	if ( rv != S_OK )
	{
		throw CELibraryException(CELibraryException::Err_ReadItem);
	}

	//
	// construct new file name and export stream to it
	//
	fullFileName = directory + "\\" + pouFile;
	if ( file.Open(fullFileName,CFile::modeCreate | CFile::modeWrite |CFile::typeBinary) )
	{
		ceStream.ExportFile(file);
		file.Close();
	}*/
	
	return SetError(Err_Ok);
}

/////////////////////////////////////////////////////////////////////
/**
 * set a property 
 *
 * @param name		name of pou
 * @param property	name of property
 * @param value		value of property
 * @return			Err_Ok property added, Err_POUNotExist, Err_NotOk
 * @exception   
 * @see         
 */
int CELibrary::AddPOUProperty(LPCTSTR  name,LPCTSTR property,
							  LPCTSTR value)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CELibraryItem* pCeItem=m_pLibList->Get(name);
	if ( pCeItem == NULL )
		return SetError(Err_POUNotExist,name);

	//
	// librarian is case insensitive
	//
	CString prop=property;
	prop.MakeLower();

	if ( pCeItem->SetProperty(prop,value ) )
	{
		SetModified(TRUE);
		return SetError(Err_Ok);
	}
	else
		return SetError(Err_NotOk);
}

/////////////////////////////////////////////////////////////////////
/**
 * delete a property 
 *
 * @param name		name of pou
 * @param property	name of property
 * @return			Err_Ok property deleted, Err_POUNotExist, Err_NotOk
 * @exception   
 * @see         
 */
int CELibrary::DeletePOUProperty(LPCTSTR name,LPCTSTR property)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CELibraryItem* pCeItem=m_pLibList->Get(name);
	if ( pCeItem == NULL )
		return SetError(Err_POUNotExist);

	//
	// librarian is case insensitive
	//
	CString prop=property;
	prop.MakeLower();

	if ( pCeItem->DeleteProperty(prop) )
	{
		SetModified(TRUE);
		return SetError(Err_Ok);
	}
	else
		return SetError(Err_NotOk);
}

/////////////////////////////////////////////////////////////////////
/**
 * get a property 
 *
 * @param name		name of pou
 * @param property	name of property
 * @param value		value of property
 * @return			Err_Ok ok, Err_POUNotExist, Err_NotOk property not
 *					found
 * @exception   
 * @see         
 */
int CELibrary::GetPOUProperty(LPCTSTR name, LPCTSTR property,
							  LPTSTR value)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CELibraryItem* pCeItem=m_pLibList->Get(name);
	if ( pCeItem == NULL )
		return SetError(Err_POUNotExist,name);

	//
	// librarian is case insensitive
	//
	CString prop=property;
	prop.MakeLower();
	CString val=value;

	if ( pCeItem->GetProperty(prop, val) )
	{
		_tcscpy(value,val);
		return SetError(Err_Ok);
	}
	else
		return SetError(Err_NotOk);
}

/////////////////////////////////////////////////////////////////////
/**
 * get a property at specified index 
 *
 * @param pouName	name of pou
 * @param index		index of property
 * @param property	name of property
 * @param value		value of property
 * @param bufferLenth  size of property and value parameters
 * @return			Err_Ok ok, Err_POUNotExist, Err_NotOk property not
 *					found
 * @exception   
 * @see         
 */
int CELibrary::GetPOUProperty(LPCTSTR pouName, int index, 
							  LPTSTR property,
							  LPTSTR value, int bufferLength)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CELibraryItem* pCeItem=m_pLibList->Get(pouName);
	if ( pCeItem == NULL )
		return SetError(Err_POUNotExist,pouName);

	//
	// librarian is case insensitive
	//
	CString prop=property;
	prop.MakeLower();
	CString val=value;

	if ( pCeItem->GetProperty(index, prop, val) )
	{
		if ( bufferLength > 0 )
		{
			_tcsncpy(property,prop,bufferLength-1);
			_tcsncpy(value,val,bufferLength-1);
			property[bufferLength-1] = '\0';
			value[bufferLength-1] = '\0';
		}
		else
		{
			_tcscpy(property,prop);
			_tcscpy(value,val);
		}
		return SetError(Err_Ok);
	}
	else
		return SetError(Err_NotOk);
}

/////////////////////////////////////////////////////////////////////
/**
 * get count of properties 
 *
 * @param pouName	name of pou
 * @return			number of properties, -1 if pou not found
 * @exception   
 * @see         
 */
int CELibrary::GetPOUPropertyCount(LPCTSTR pouName)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CELibraryItem* pCeItem=m_pLibList->Get(pouName);
	if ( pCeItem == NULL )
		return -1;

	return pCeItem->GetPropertyCount();
}

/////////////////////////////////////////////////////////////////////
/**
 * maps an error id to a string 
 *
 * @param errId		error id
 * @return			string id
 * @exception   
 * @see         
 */
LPCTSTR CELibrary::GetErrorMessage(long errId)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));
	
	static CString message="";
	int iListCnt = sizeof(ErrIdList) / sizeof(CELibErrId);
	for(int i = 0; i < iListCnt; i++) {
		if(ErrIdList[i].errId == errId) {
			message.LoadString(ErrIdList[i].errTextId);
			return message;
		}
	}
	message.LoadString(IDS_CE_ERR);
	return message;
}

LPCTSTR CELibrary::GetLastErrorMessage()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));
	
	static CString message="";
	if (m_lastErrorId == Err_Ok) 
		return message;
	int iListCnt = sizeof(ErrIdList) / sizeof(CELibErrId);
	message.LoadString(IDS_CE_ERR);
	for(int i = 0; i < iListCnt; i++) {
		if(ErrIdList[i].errId == m_lastErrorId) {
			message.LoadString(ErrIdList[i].errTextId);
		}
	}
	message = message + " " + m_lastErrorInfo;
	return message;
}

/////////////////////////////////////////////////////////////////////
/**
 * get the file name in the library of an item to be inserted into lib
 *
 * @param pouName		name of item
 * @param fullFileName  real full file name of item to be inserted
 * @return				file name of item in lib
 * @exception   
 * @see         
 */
LPCTSTR CELibrary::GetFileNameInLib(LPCTSTR  pouName, LPCTSTR fullFileName)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	static CString fileName = "";
	fileName = CEFile::GetFileName(fullFileName);
	//return pouName + "\\" + fileName;
	return fileName;
}

int CELibrary::GetPOUFileByteSize(LPCTSTR ppouName, LPCTSTR ppouFileName, unsigned long* pulFileByteSize)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CString pouName=ppouName;
	CString pouFileName=ppouFileName;

	//
	// get pou
	//
	CELibraryItem* pCeItem=m_pLibList->Get(pouName);
	if ( pCeItem == NULL)
		return SetError(Err_POUNotExist,pouName);

	//
	// test existance of pou file
	//
	if ( ! pCeItem->ExistFile(pouFileName) )
		return SetError(Err_POUFileNotExist,pouFileName);
	//
	// open storage pouName
	//
	/*CEStorage ceStorage;
	HRESULT rv = m_pWorkingStorage->OpenStorage(pouName,
		STGM_READ | STGM_SHARE_EXCLUSIVE, 
		&ceStorage );
	AnalyzeHRESULT(rv);
	if ( rv != S_OK )
	{
		throw CELibraryException(CELibraryException::Err_ReadItem);
	}
	//
	// open stream 
	//
	CEStream ceStream;
	rv = ceStorage.OpenStream(pouFileName, 
		STGM_READ|STGM_SHARE_EXCLUSIVE,
		&ceStream);
	if ( rv != S_OK )
	{
		throw CELibraryException(CELibraryException::Err_ReadItem);
	}

	//
	// get stream information
	//
	STATSTG statStg;
	rv = ceStream.Stat(&statStg,STATFLAG_NONAME);
	if ( rv != S_OK )
	{
		throw CELibraryException(CELibraryException::Err_ReadItem);
	}
	*pulFileByteSize = statStg.cbSize.LowPart;*/

	return SetError(Err_Ok);
}

int CELibrary::GetPOUHeaderByteSize(LPCTSTR ppouName, unsigned long* pulFileByteSize)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));
	
	CString pouName=ppouName;
	
	//
	// get header info
	//
	TCHAR strheaderFileName[256];
	unsigned long byteOffsetStart, byteOffsetEnd;
	int rv=GetHeaderInfo(pouName, strheaderFileName, byteOffsetStart, byteOffsetEnd);
	if ( rv != Err_Ok )
		return SetError(rv);

	CString headerFileName = strheaderFileName;
	if (headerFileName == "")
		return SetError(Err_NotOk);

	if ( byteOffsetEnd != 0 )
	{
		*pulFileByteSize = byteOffsetEnd;
	}
	else
	{
		//
		// get header file into buffer
		//
		rv = GetPOUFileByteSize(pouName, headerFileName, pulFileByteSize);
		if ( rv != Err_Ok)
			return SetError(rv);
		*pulFileByteSize = *pulFileByteSize - byteOffsetStart;
	}


	return SetError(Err_Ok);
}

int CELibrary::GetPOUPublicHeaderByteSize(LPCTSTR ppouName, unsigned long* pulFileByteSize)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));
	
	CString pouName=ppouName;
	
	//
	// try to get display info. if failed get header info
	//
	TCHAR pszheaderFileName[256];
	unsigned long byteOffsetStart, byteOffsetEnd;
	int rv=GetDisplayInfo(pouName, pszheaderFileName, byteOffsetStart, byteOffsetEnd);
	if ( rv != Err_Ok || pszheaderFileName[0] == '\0')
	{
		rv=GetHeaderInfo(pouName, pszheaderFileName, byteOffsetStart, byteOffsetEnd);
		if ( rv != Err_Ok)
			return SetError(rv);
	}

	CString strHeaderFileName = pszheaderFileName;
	if (strHeaderFileName == "")
		return SetError(Err_NotOk);

	if ( byteOffsetEnd != 0 )
	{
		*pulFileByteSize = byteOffsetEnd;
	}
	else
	{
		//
		// get header file into buffer
		//
		rv = GetPOUFileByteSize(pouName, strHeaderFileName, pulFileByteSize);
		if ( rv != Err_Ok)
			return SetError(rv);
		*pulFileByteSize = *pulFileByteSize - byteOffsetStart;
	}


	return SetError(Err_Ok);
}

int CELibrary::GetPOUFileAsBuffer(LPCTSTR ppouName, LPCTSTR ppouFileName, void* pBuffer, unsigned long ulBuffSize,
								  unsigned long byteOffsetStart )
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CString pouName=ppouName;
	CString pouFileName=ppouFileName;

	//
	// get pou
	//
	CELibraryItem* pCeItem=m_pLibList->Get(pouName);
	if ( pCeItem == NULL)
		return SetError(Err_POUNotExist,pouName);
    BSTR spouName = pouName.AllocSysString();
    BSTR spouFileName = pouFileName.AllocSysString();
    HRESULT hr = m_pILibAcc->GetFileRaw(spouName, NULL, spouFileName, (byte*) pBuffer, ulBuffSize);
    ::SysFreeString(spouName);
    ::SysFreeString(spouFileName);
    if(hr != S_OK)
    {
        return SetError(Err_NotOk);
    }
    else
    {
        return SetError(Err_Ok);
    }
    
    //HRESULT GetFileRaw(
    //    [in, string] BSTR   sPOUName, 
    //    [in, string] BSTR   sTargetType,
    //    [in, string] BSTR   sFileName,
    //    [out, ptr]   byte*  pBytes,
    //    [in]         DWORD  nBytes
    //);

    //HRESULT hr = GetFileAsBSTR(
    //    [in, string] BSTR   sPOUName, 
    //    [in, string] BSTR   sTargetType,
    //    [in, string] BSTR   sFileName,
    //    [out]        BSTR*  sTextFile
    //);   

	//
	// test existance of pou file
	//
	//if ( ! pCeItem->ExistFile(pouFileName) )
	//	return SetError(Err_POUFileNotExist,pouFileName);
	//
	// open storage pouName
	//
	/*CEStorage ceStorage;
	HRESULT rv = m_pWorkingStorage->OpenStorage(pouName,
		STGM_READ | STGM_SHARE_EXCLUSIVE, 
		&ceStorage );
	AnalyzeHRESULT(rv);
	if ( rv != S_OK )
	{
		throw CELibraryException(CELibraryException::Err_ReadItem);
	}
	//
	// open stream 
	//
	CEStream ceStream;
	rv = ceStorage.OpenStream(pouFileName, 
		STGM_READ|STGM_SHARE_EXCLUSIVE,
		&ceStream);
	if ( rv != S_OK )
	{
		throw CELibraryException(CELibraryException::Err_ReadItem);
	}

	//
	// read stream into buffer
	//
	ULONG nBytesRead;
	rv = ceStream.Seek(byteOffsetStart,STREAM_SEEK_SET);
	if ( rv != S_OK )
		return SetError(Err_NotOk);
	rv = ceStream.Read(pBuffer, ulBuffSize, &nBytesRead);
	if ( nBytesRead == ulBuffSize && rv == S_OK)
		return SetError(Err_Ok);
	else
		return SetError(Err_NotOk);*/
}

/////////////////////////////////////////////////////////////////////
/**
 * Get the complete header of header into a buffer
 * is "\r\n"
 *
 * @param pouName		name of pou
 * @param pBuffer		the content of the header
 * @param ulBuffSize	size of pBuffer
 * @return 0-1			1 item was found in library
 *						0 item was not found in library
 * @exception   CELibraryException if stream belonging to item can't 
 *				read
 * @see         
 */
int CELibrary::GetPOUHeaderAsBuffer(LPCTSTR ppouName, void* pBuffer, unsigned long ulBuffSize)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CString pouName=ppouName;
	
	//
	// get header info
	//
	TCHAR headerFileName[256];
	unsigned long byteOffsetStart, byteOffsetEnd;
	int rv=GetHeaderInfo(pouName, headerFileName, byteOffsetStart, byteOffsetEnd);
	if ( rv != Err_Ok )
		return rv;

	//
	// get header file into buffer
	//
	rv = GetPOUFileAsBuffer(pouName, headerFileName, pBuffer, ulBuffSize, byteOffsetStart);

	if ( rv != Err_Ok)
		return rv;
	return SetError(Err_Ok);
}

/////////////////////////////////////////////////////////////////////
/**
 * Get the public/display part of header of header into a buffer
 * is "\r\n"
 *
 * @param pouName		name of pou
 * @param pBuffer		the content of the header
 * @param ulBuffSize	size of pBuffer
 * @return 0-1			1 item was found in library
 *						0 item was not found in library
 * @exception   CELibraryException if stream belonging to item can't 
 *				read
 * @see         
 */
int CELibrary::GetPOUPublicHeaderAsBuffer(LPCTSTR ppouName, void* pBuffer, unsigned long ulBuffSize)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CString pouName=ppouName;
	
	//
	// try to get display info. if failed get header info
	//
	TCHAR headerFileName[256];
	unsigned long byteOffsetStart, byteOffsetEnd;
	int rv=GetDisplayInfo(pouName, headerFileName, byteOffsetStart, byteOffsetEnd);
	if ( rv != Err_Ok || headerFileName[0] == '\0')
	{
		rv=GetHeaderInfo(pouName, headerFileName, byteOffsetStart, byteOffsetEnd);
		if (rv != Err_Ok )
			return rv;
	}

	//
	// get header file into buffer
	//
	rv = GetPOUFileAsBuffer(pouName, headerFileName, pBuffer, ulBuffSize, byteOffsetStart);

	if ( rv != Err_Ok)
		return rv;
	return SetError(Err_Ok);
}

/////////////////////////////////////////////////////////////////////
/**
 * Get the file of the specified item as a string. The line seperator
 * is "\r\n"
 *
 * @param pouName		name of item [in]
 * @param pouContent		the content of the file as string [out]
 * @return 0-1			1 item was found in library
 *						0 item was not found in library
 * @exception   CELibraryException if stream belonging to item can't 
 *				read
 * @see         
 */
int CELibrary::GetPOUFileAsString(LPCTSTR ppouName, LPTSTR pouContent)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	CString pouName=ppouName;
	TCHAR ppouFileName[256];
	pouContent[0] = '\0';

	//
	// get relative file name in lib
	//
	GetPOUFile(pouName,ppouFileName);
	CString pouFileName=ppouFileName;
	
	//
	// get file name
	//
	pouFileName = CEFile::GetFileName(pouFileName);
	if (pouFileName == "")
		return 0;
	//
	// open storage pouName
	//
	/*CEStorage ceStorage;
	HRESULT rv = m_pWorkingStorage->OpenStorage(pouName,
		STGM_READ | STGM_SHARE_EXCLUSIVE, 
		&ceStorage );
	AnalyzeHRESULT(rv);
	if ( rv != S_OK )
	{
		throw CELibraryException(CELibraryException::Err_ReadItem);
	}
	//
	// open stream 
	//
	CEStream ceStream;
	rv = ceStorage.OpenStream(pouFileName, 
		STGM_READ|STGM_SHARE_EXCLUSIVE,
		&ceStream);
	if ( rv != S_OK )
	{
		throw CELibraryException(CELibraryException::Err_ReadItem);
	}

	//
	// read stream int string
	//
	ULONG nBytesRead;
	char buf[2];
	while (ceStream.Read(buf, 1, &nBytesRead) == S_OK)
	{
		if ( buf[0] == '\n' ) _tcscat(pouContent,"\r");
		buf[1] = '\0' ;
		_tcscat(pouContent,(const char*) buf);
	}*/
	return SetError(Err_Ok);
}

/////////////////////////////////////////////////////////////////////
/**
 * Add a item to the internal list
 *
 * @param name		name of item
 * @param type		type of item
 * @param fileName	fileName of item
 * @return			void
 * @exception  
 * @see        
 */
void CELibrary::AddInternalPOU(CString name, CString type, CString fileName)
{
	CELibraryItem* pCeItem=m_pLibList->Add(name);
	if (fileName != "")
		pCeItem->AddFile(fileName);
	pCeItem->SetProperty(CEL_ATTR_TYPE,type);
}

/////////////////////////////////////////////////////////////////////
/**
 * Add a file of an item to the internal list
 *
 * @param name		name of item
 * @param fileName	fileName of item
 * @return			void
 * @exception  
 * @see        
 */
void CELibrary::AddInternalPOUFile(CString name, CString fileName)
{
	CELibraryItem* pCeItem=m_pLibList->Get(name);
	ASSERT(pCeItem);
	if (pCeItem != NULL)
		pCeItem->AddFile(fileName);
}

/////////////////////////////////////////////////////////////////////
/**
 * Delete item from the internal list
 *
 * @param pouName	name of item
 * @return			1 ok; 0 item not found
 * @exception  
 * @see        
 */
int CELibrary::DeleteInternalPOU(const CString& pouName)
{
	return m_pLibList->Delete(pouName);
}

/////////////////////////////////////////////////////////////////////
/**
 * Delete item file from the internal list
 *
 * @param pouName	name of item
 * @param pouFileName name of item file
 * @return			1 ok; 0 item/item file not found
 * @exception  
 * @see        
 */
int CELibrary::DeleteInternalPOUFile(const CString& pouName, 
									const CString& pouFileName)
{
	CELibraryItem* pCeItem = m_pLibList->Get(pouName);
	if ( pCeItem == NULL )
		return 0;
	return pCeItem->DeleteFile(pouFileName);
}

int CELibrary::SetHeaderInfo(const CString& pouName, const CString& headerFile, const CString& byteOffsetStart, const CString& byteOffsetEnd)
{
	CELibraryItem* pCeItem=m_pLibList->Get(pouName);
	if (pCeItem != NULL)
	{
		pCeItem->SetProperty(CEL_ATTR_HEADER_FILNAME,headerFile);
		pCeItem->SetProperty(CEL_ATTR_HEADER_OFFSET_START,byteOffsetStart);
		pCeItem->SetProperty(CEL_ATTR_HEADER_OFFSET_END,byteOffsetEnd);
		return SetError(Err_Ok);
	}
	return SetError(Err_NotOk);
}

int CELibrary::GetHeaderInfo(LPCTSTR pouName, LPTSTR pheaderFile, unsigned long& byteOffsetStart, unsigned long& byteOffsetEnd)
{
	*pheaderFile = '\0';
	byteOffsetStart = 0;
	byteOffsetEnd = 0;
	CString headerFile;

	CELibraryItem* pCeItem=m_pLibList->Get(pouName);
	if (pCeItem != NULL)
	{
		CString helpString;
		long offset;
		pCeItem->GetProperty(CEL_ATTR_HEADER_FILNAME,headerFile);
		_tcscpy(pheaderFile,headerFile);
		pCeItem->GetProperty(CEL_ATTR_HEADER_OFFSET_START, helpString);
		offset = atol(helpString);
		if (offset>=0) byteOffsetStart=offset;
		helpString = "";
		pCeItem->GetProperty(CEL_ATTR_HEADER_OFFSET_END, helpString);
		offset = atol(helpString);
		if (offset>=0) byteOffsetEnd=offset;
		return SetError(Err_Ok);
	}
	return SetError(Err_NotOk);
}

int CELibrary::SetDisplayInfo(const CString& pouName, const CString& strDisplayFile, const CString& strByteOffsetStart, const CString& strByteOffsetEnd)
{
	CELibraryItem* pCeItem=m_pLibList->Get(pouName);
	if (pCeItem != NULL)
	{
		pCeItem->SetProperty(CEL_ATTR_DISPLAY_FILNAME,strDisplayFile);
		pCeItem->SetProperty(CEL_ATTR_DISPLAY_OFFSET_START,strByteOffsetStart);
		pCeItem->SetProperty(CEL_ATTR_DISPLAY_OFFSET_END,strByteOffsetEnd);
		return SetError(Err_Ok);
	}
	return SetError(Err_NotOk);
}

int CELibrary::GetDisplayInfo(LPCTSTR pouName, LPTSTR pszDisplayFile, unsigned long& byteOffsetStart, unsigned long& byteOffsetEnd)
{
	*pszDisplayFile = '\0';
	byteOffsetStart = 0;
	byteOffsetEnd = 0;
	CString headerFile;

	CELibraryItem* pCeItem=m_pLibList->Get(pouName);
	if (pCeItem != NULL)
	{
		CString helpString;
		long offset;
		pCeItem->GetProperty(CEL_ATTR_DISPLAY_FILNAME,headerFile);
		_tcscpy(pszDisplayFile,headerFile);
		pCeItem->GetProperty(CEL_ATTR_DISPLAY_OFFSET_START, helpString);
		offset = atol(helpString);
		if (offset>=0) byteOffsetStart=offset;
		helpString = "";
		pCeItem->GetProperty(CEL_ATTR_DISPLAY_OFFSET_END, helpString);
		offset = atol(helpString);
		if (offset>=0) byteOffsetEnd=offset;
		return SetError(Err_Ok);
	}
	return SetError(Err_NotOk);
}

/////////////////////////////////////////////////////////////////////
/**
 * test the read only flag of original library
 *
 * @return 0-1  1 lib is read only; 
 *              0 else
 * @exception   
 * @see         
 */
BOOL CELibrary::IsReadOnly()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	return m_readOnly;
}

/////////////////////////////////////////////////////////////////////
/**
 * analyze error code of IStream and IStore interfaces
 *
 * @param hres  error code
 * @return		void 
 * @exception   
 * @see         
 */
void CELibrary::AnalyzeHRESULT(HRESULT hres)
{
	switch (hres)
	{
	case S_OK:
		int rv;
		break;
	case S_FALSE:
		break;
	case STG_E_FILENOTFOUND:
		break;
	case STG_E_ACCESSDENIED:
		break;
	case STG_E_LOCKVIOLATION: 
		break;
	case STG_E_FILEALREADYEXISTS: 
		break;
	case STG_E_TOOMANYOPENFILES: 
		break;
	case STG_E_INSUFFICIENTMEMORY: 
		break;
	case STG_E_INVALIDNAME: 
		break;
	case STG_E_INVALIDPOINTER: 
		break;
	case STG_E_INVALIDFLAG: 
		break;
	case STG_E_INVALIDFUNCTION: 
		break;
	case STG_E_OLDFORMAT: 
		break;
	case STG_E_OLDDLL: 
		break;
	case STG_E_PATHNOTFOUND:
		rv = 1;
		break;
	case E_PENDING: 
		break;
	case STG_E_INVALIDPARAMETER: 
		break;
	case STG_E_REVERTED:
		break;
	default:
		rv=1;
		break;
	}
}

/////////////////////////////////////////////////////////////////////
/**
 * import library from directory structure
 *
 * @param	fileName	the file name of the lib.list file
 * @return	void
 * @exception   
 * @see         
 */
int CELibrary::Import(LPCTSTR fullFileName)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	//
	// read lib.list file and add files to working storage
	//
	try 
	{
		CString line;
		CString name="";
		CString key,value;
		CString fileName;
		int len,eq;

		CStdioFile file(fullFileName,CFile::modeRead);
		while (file.ReadString(line))
		{
			if ((len = line.GetLength()) < 1) continue;
			if ( line[0] == '[')
			{
				name = line.Mid(1,len-2);
				AddPOU(name,"");
			}
			else if (line.Mid(0,5) == "file=")
			{
				fileName = line.Mid(5);
				if (name != "")
					AddPOUFile(name,fileName);
			}
			else
			{
				//
				// 'parse' property
				//
				eq=line.Find('=');
				if ( eq > 1 )
				{
					key = line.Mid(0,eq);
					value = line.Mid(eq+1);
					if (name!="")
						AddPOUProperty(name,key,value);
				}
			}
		}
	}
	catch (CFileException ex)
	{
		throw new CELibraryException(CELibraryException::Err_FileNotFound);
	}
	return SetError(Err_Ok);
}

/////////////////////////////////////////////////////////////////////
/**
 * Add a link file to the (working) library.
 *
 * @param linkFileName		name of link File
 * @param pPouName			name of pou (out)
 * @param bDeleteDouble     TRUE: double entries are deleted,
 *							FALSE: return error code if entry already ex.
 * @return
 *              Err_Ok: ok
 * @exception   CELibraryException, if file was not found, file can't be added
 *				to working lib
 * @see         
 */
int CELibrary::AddPOULinkFile(LPCTSTR linkFileName, LPTSTR pPouName, BOOL bDeleteDouble)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

#define CEL_NUMTOK 5

	CString pouName, pouType;
	CString headerSourceFile, headerDestFile, headerByteOffsetStart,headerByteOffsetEnd;
	CStringArray usesEntries, sourceEntries, putEntries;
	CString strDisplayFile, strDisplayByteOffsetStart, strDisplayByteOffsetEnd;

	//
	// get path of link file, all paths in link file are relative to it
	//
	CString linkFilePath = CEFile::GetFilePath(linkFileName);
	if (linkFilePath != "")
	{
		if (linkFilePath[linkFilePath.GetLength()-1] != '\\')
			linkFilePath = linkFilePath + "\\";
	}

	//
	// test if link file exist 
	//
	if ( ! CEFile::IsFile(linkFileName) )
	{
		return SetError(CELibrary::Err_FileNotFound,linkFileName);
	}

	//
	// scan link file
	//
	try 
	{
		CString line,token[CEL_NUMTOK];
		CStdioFile file(linkFileName,CFile::modeRead);
		//
		// skip first line
		//
		file.ReadString(line);
		while (file.ReadString(line))
		{
			//
			// get tokens
			//
			for (int ii=0;ii<CEL_NUMTOK;ii++)
			{
				int eq=line.Find(';');
				if (eq < 0)
				{
					token[ii] = line;
					line = "";
				}
				else
				{
					token[ii]=line.Mid(0,eq);
					line=line.Mid(eq+1);
				}
				token[ii].TrimLeft();
				token[ii].TrimRight();
			}
			//
			// parse lines according to keyword
			//
			if ( token[0].CompareNoCase(CEL_LINK_DEST) == 0)
			{
				pouName=token[1];
				if ( pPouName != NULL )
					_tcscpy(pPouName,pouName);
			}
			else if (token[0].CompareNoCase(_T(CEL_ATTR_TYPE)) == 0)
			{
				pouType=token[1];
			}
			else if (token[0].CompareNoCase(CEL_LINK_HEADER) == 0)
			{
				if (token[1] != "")
					headerSourceFile=linkFilePath + token[1];
				headerDestFile=token[2];
				headerByteOffsetStart=token[3];
				headerByteOffsetEnd=token[4];
			}
			else if (token[0].CompareNoCase(CEL_LINK_USES) == 0)
			{
				if (token[1] != "")
					usesEntries.Add(token[1]);
			}
			else if (token[0].CompareNoCase(CEL_LINK_SOURCE) == 0)
			{
				if (token[1] != "")
					sourceEntries.Add(linkFilePath+token[1]);
			}
			else if (token[0].CompareNoCase(CEL_LINK_PUT) == 0)
			{
				if (token[1] != "")
					putEntries.Add(linkFilePath+token[1]);
			}
			else if (token[0].CompareNoCase(CEL_LINK_DISPLAY) == 0)
			{
				strDisplayFile=token[1];
				strDisplayByteOffsetStart=token[2];
				strDisplayByteOffsetEnd=token[3];
			}
		}
	}
	catch (CFileException ex)
	{
		throw new CELibraryException(CELibraryException::Err_FileNotFound);
	}

	//
	// test input data pou type: function, function_block, struct, const 
	//
	pouType.MakeUpper();
	if (	pouType != CEL_TYPE_FUNCTION 
		&&	pouType != CEL_TYPE_FUNCTION_BLOCK
		&&	pouType != CEL_TYPE_CONST
		&&	pouType != CEL_TYPE_STRUCT
		&&  pouType != CEL_TYPE_COMMON_FILES
		)
	{
		return SetError(Err_WrongPOUType,pouType);
	}

	//
	// test input data: headerFile must be empty or must exist
	//
	BSTR sHeaderSourceBuffer;
	CString strHeaderSourceBuffer;
	CString strHeaderExt = CEFile::GetFileExt(headerSourceFile);
	BOOL bPlainFile=TRUE;
	if (headerSourceFile.Find(_T("::")) >=0 || strHeaderExt.CompareNoCase(_T("4gr")) == 0)
		bPlainFile=FALSE;

	if ( headerSourceFile != "" && bPlainFile == FALSE)
	{
		//
		// object converter
		//
		if (m_pObjConvert == NULL)
		{
			HRESULT hres = CoCreateInstance(CLSID_ObjConvert, NULL, CLSCTX_ALL,
				IID_IObjConvert, (void**)&m_pObjConvert);
			if ( hres != S_OK )
			{
			//	AfxMessageBox(_T("Can't create object converter control!"));
				m_pObjConvert=NULL;
				return SetError(Err_CoCreateFailed, _T("Object Converter"));
			}
		}

		//
		// convert header to plain st
		//
		BSTR sHeaderSourceFile=headerSourceFile.AllocSysString();
		HRESULT hr=m_pObjConvert->GetFileInBuffer(sHeaderSourceFile, &sHeaderSourceBuffer);
		::SysFreeString(sHeaderSourceFile);
		if (hr != S_OK)
		{
			::SysFreeString(sHeaderSourceBuffer);
			return SetError(Err_ObjConvert, headerSourceFile);
		}
		else
		{
			strHeaderSourceBuffer=sHeaderSourceBuffer;
			::SysFreeString(sHeaderSourceBuffer);
		}
	}
	else if ( headerSourceFile != "" && bPlainFile == TRUE)
	{
		if ( ! CEFile::IsFile( headerSourceFile ) )
		{
			return SetError(Err_FileNotFound,headerSourceFile);
		}
	}

	//
	// test input data: header file in lib must not be empty
	//
	if ( headerDestFile == "")
	{
		return SetError(Err_WrongHeaderDest,"");
	}

	//
	// test input data: source files must exist
	//
	for (int ii=0; ii< sourceEntries.GetSize(); ii++)
	{
		if ( ! CEFile::IsFile(sourceEntries.GetAt(ii)) )
		{
			return SetError(Err_FileNotFound,sourceEntries.GetAt(ii));
		}
	}

	//
	// test input data: uses Entries not yet allowed
	//
	if ( usesEntries.GetSize() > 0 )
	{
		CString useEntry;
		for (int ii = 0; ii<usesEntries.GetSize(); ii++)
		{
			useEntry = usesEntries.GetAt(ii);
			if (useEntry.CompareNoCase(CEL_VAR_EXTERNAL) == 0)
				return SetError(Err_WrongUsesEntry,"");
		}
	}

	//
	// test input data: put files must exist
	//
	for (ii=0; ii< putEntries.GetSize(); ii++)
	{
		if ( ! CEFile::IsFile(putEntries.GetAt(ii)) )
		{
			return SetError(Err_FileNotFound,putEntries.GetAt(ii));
		}
	}

	int rv;

	if ( ExistPOU(pouName) )
	{
		//
		// delete pou if it already exists
		//
		if (bDeleteDouble == TRUE)
			rv = DeletePOU(pouName);
		else
			return SetError(Err_POUAlreadyExist,"");
	}

	//
	// add pou and all files to library
	//
	rv = AddPOU(pouName,pouType);
	for (ii=0;ii<putEntries.GetSize();ii++)
	{
		rv = AddPOUFile(pouName,putEntries.GetAt(ii));
	}

	//
	// copy header information to library
	//
	if (headerSourceFile == "")
	{
		rv = SetHeaderInfo(pouName,headerDestFile,headerByteOffsetStart,headerByteOffsetEnd);
	}
	else
	{
		//
		// if headerSourceFile not empty copy headerSourceFile
		//
		unsigned long offsetStart=0, offsetEnd=0;
		long offset = atol(headerByteOffsetStart);
		if (offset >0)
			offsetStart=offset;
		offset = atol(headerByteOffsetEnd);
		if (offset >0)
			offsetEnd=offset;
		CString strTest=strHeaderSourceBuffer.Left(offsetEnd);
		if (bPlainFile == FALSE)
		{
			rv = AddPOUFileBuffer(pouName, strHeaderSourceBuffer, headerDestFile, offsetStart, offsetEnd);
		}
		else
		{
			rv = AddPOUFile(pouName, headerSourceFile, headerDestFile, offsetStart, offsetEnd);
		}
		rv = SetHeaderInfo(pouName,headerDestFile, _T("0"), _T("0"));
	}

	//
	// set header display information
	//
	if ( strDisplayFile != "" )
	{
		rv = SetDisplayInfo(pouName, strDisplayFile, strDisplayByteOffsetStart, strDisplayByteOffsetEnd);
	}

	return SetError(Err_Ok);
#undef CEL_NUMTOK
}

/////////////////////////////////////////////////////////////////////
/**
 * set the password of the library. 
 * the password is encrypted. the propterty "password" is added to
 * the pou "."
 *
 * @param	passWord the password ( not encrypted )	
 * @return		Err_NotOk: not ok
 *              Err_Ok: ok
 * @exception
 * @see         
 */
int CELibrary::SetPassWord(LPCTSTR passWord)
{
	//
	// if pou . does not exist add it
	//
	int rv = 1;
	BOOL exist = ExistPOU(".");
	if ( exist != TRUE )
		rv = AddPOU(".","");
	
	CString encPassWord;
	rv = CEPassWord::Encode(passWord,encPassWord);
	if ( rv )
	{
		rv=AddPOUProperty(".",CEL_ATTR_PASSWORD,encPassWord);
		return SetError(rv);
	}
	else
	{
		return SetError(Err_NotOk);
	}
}

/////////////////////////////////////////////////////////////////////
/**
 * test the password of the library. 
 * 
 *
 * @param	passWord the password ( not encrypted )	
 * @return		Err_NotOk: password does not match or lib not 
 *							password protected
 *              Err_Ok: ok password correct
 * @exception
 * @see         
 */
int CELibrary::TestPassWord(LPCTSTR passWord)
{
	BOOL exist = ExistPOU(".");
	if ( exist != TRUE )
		return Err_NotOk;

	TCHAR buffer[CEL_MAX_PWLEN+1000];
	int rv = GetPOUProperty(".", CEL_ATTR_PASSWORD, buffer);
	if ( rv != Err_Ok )
		return SetError(rv);
	CString libPassWord = buffer;
	CString encPassWord;
	rv = CEPassWord::Encode(passWord,encPassWord);
	if ( libPassWord == encPassWord )
		return SetError(Err_Ok);
	else
		return SetError(Err_NotOk);
}

/////////////////////////////////////////////////////////////////////
/**
 * test if library is password protected
 * 
 *
 * @return		TRUE: password protected
 *				FALSE: not password protected
 * @exception
 * @see         
 */
BOOL CELibrary::IsPassWordProtected()
{
	TCHAR buffer[CEL_MAX_PWLEN+1000];
	int rv = GetPOUProperty(".",CEL_ATTR_PASSWORD,buffer);
	if ( rv != Err_Ok )
		return FALSE;
	else
		return TRUE;
}

int CELibrary::SetError(int errId, const CString& errorInfo)
{
	m_lastErrorInfo = errorInfo;
	m_lastErrorId = errId;
	return errId;
}

/////////////////////////////////////////////////////////////////////
/**
 * Set the time stamp (last modification) of lib
 * 
 * @param timeStamp  last modification of lib file
 * @exception   
 * @see         ...
 */
void CELibrary::SetTimeStamp(FILETIME timeStamp)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));
	m_timeStamp = CTime(timeStamp);
}

/////////////////////////////////////////////////////////////////////
/**
 * Get the time stamp (last modification) of lib as ANSI time_t
 * 
 * @return timeStamp  last modification of lib file
 * @exception   
 * @see         ...
 */
time_t CELibrary::GetTimeStamp()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));
	return m_timeStamp.GetTime();
}

/////////////////////////////////////////////////////////////////////
/**
 * Set the modified flag of lib
 * 
 * @param bModified: TRUE modified, FALSE not modified
 * @exception   
 * @see         ...
 */
void CELibrary::SetModified(BOOL bModified)
{
	m_bModified=bModified;
}

/////////////////////////////////////////////////////////////////////
/**
 * get the modified flag of lib
 * 
 * @return TRUE modified, FALSE not modified
 * @exception   
 * @see         ...
 */
BOOL CELibrary::IsModified()
{
	return m_bModified;
}


/////////////////////////////////////////////////////////////////////
/**
 * Parse all constant files
 * 
 * @return 1 ok, 0 not ok
 * @exception   
 * @see         ...
 */
int CELibrary::ParseConstFiles()
{
	CString strConstFileName;
	LPCTSTR pszConst = CEL_TYPE_CONST;
	ConstFileParser parser;
	unsigned long ulFileSize;
	HRESULT rv;

	m_pConstList->DeleteAll();

	int nn = GetPOUFileCount(pszConst);
	for (int ii=0; ii<nn; ii++)
	{
		strConstFileName=GetPOUFileName(pszConst, ii);
		
		//
		// exclude .link file
		//
		if (strConstFileName.Right(5).CompareNoCase(_T(".link")) == 0)
			continue;

		rv=GetPOUFileByteSize(pszConst,strConstFileName,&ulFileSize);
		ASSERT(rv==Err_Ok);
		if ( ulFileSize > 0)
		{
			LPTSTR pszBuffer = (LPTSTR) new TCHAR[ulFileSize+1];
			rv=GetPOUFileAsBuffer(pszConst,strConstFileName, (void*) pszBuffer, ulFileSize);
			pszBuffer[ulFileSize] = '\0';
			ASSERT(rv==Err_Ok);
			parser.SetText(pszBuffer);
			int iPa=parser.Parse(m_pConstList);
			if ( ! iPa )
			{
				//
				// parse error
				//
				CString strMessage, strFormat;
				strFormat.LoadString(IDS_CE_ERR_PARSE);
				strMessage.Format(strFormat, strConstFileName, parser.GetLastLineNo());
			//	AfxMessageBox(strMessage, MB_OK|MB_ICONEXCLAMATION);
			}
			delete[] pszBuffer;
		}
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////
/**
 * get count of constant files in library
 * 
 * @return count of constant files
 * @exception   
 * @see         ...
 */
int CELibrary::GetConstCount()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	if (m_bConstParsed==FALSE )
	{
		int rv = ParseConstFiles();
		m_bConstParsed = TRUE;
	}
	return m_pConstList->GetConstCount();
}

/////////////////////////////////////////////////////////////////////
/**
 * get constant information at specified position
 * (method available after GetConstCount)
 *
 * @param iIndex position of constant in list
 * @param pszConstName name of constant
 * @param pszConstType type of constant
 * @param pszConstValue value of constant (string representation in source code)
 * @return 1 ok, 0 not ok
 * @exception   
 * @see         ...
 */
int CELibrary::GetConst(int iIndex, LPTSTR pszConstName, LPTSTR pszConstType, LPTSTR pszConstValue)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState( ));

	return m_pConstList->GetConst(iIndex, pszConstName, pszConstType, pszConstValue);
}

/////////////////////////////////////////////////////////////////////
/**
 * test if given constant exists in library
 * 
 * @return TRUE constant exists, else constant does not exist
 * @exception   
 * @see         ...
 */
BOOL CELibrary::ExistConst(LPCTSTR pszConstName)
{
	if (m_bConstParsed==FALSE )
	{
		int rv = ParseConstFiles();
		m_bConstParsed = TRUE;
	}
	return m_pConstList->ExistConst(pszConstName);
}

/////////////////////////////////////////////////////////////////////
/**
 * test if given pou name is not an internal pou name (CONST, __COMMON, .)
 * 
 * @return TRUE regular POU name, FALSE internal pou name
 * @param pszName name of POU
 * @exception   
 * @see         ...
 */
BOOL CELibrary::IsPOUName(LPCTSTR pszName)
{
	CString strConst=CEL_TYPE_CONST, strCommon=CEL_SEC_NAME_COMMON, strLib=_T("."), strAdd=CEL_SEC_NAME_ADDFILES;
	
	if ( strConst.CompareNoCase(pszName) == 0 ||
		strCommon.CompareNoCase(pszName) == 0 ||
		strLib.CompareNoCase(pszName) == 0    ||
        strAdd.CompareNoCase(pszName) == 0)
		return FALSE;
	else
		return TRUE;
}

/////////////////////////////////////////////////////////////////////
/**
 * make (4c) object path of a POU or constant
 * 
 * @return ErrOk ok, ErrNotOk pou does not exist in library, path is
 *	build 'pro forma' as if POU would exist in lib and is not a constant
 * @param pszName name of POU
 * @param pszObjectPath object path of POU
 * @exception   
 * @see         ...
 */
int CELibrary::GetObjectPath(LPCTSTR pszName, LPTSTR pszObjectPath)
{
	CString strPath;
	int rv=Err_NotOk;

	if ( ExistPOU(pszName) )
	{
		strPath=GetName() + CString(_T("::")) + CString(pszName);
		rv=Err_Ok;
	}
	else if ( ExistConst(pszName) )
	{
		// get constants file
		TCHAR pszConstFileName[MAX_PATH];
		int rv=GetPOUFile(CEL_TYPE_CONST, pszConstFileName, _T("cst"));
		if (rv!=Err_Ok)
			return Err_NotOk;

		strPath=GetName() + CString(pszConstFileName) + CString(_T("::CONST\\")) + CString(pszName);
		rv=Err_Ok;
	}
	else 
	{
		strPath=GetName() + CString(_T("::")) + CString(pszName);
		rv=Err_NotOk;
	}	
	
	_tcscpy(pszObjectPath, strPath);
	return rv;
}


/*
 *----------------------------------------------------------------------------*
 *  $History: CELibrary.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 6  *****************
 * User: Sis          Date: 24.05.02   Time: 10:22
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * bug fix 1615: replaced AfxGetStaticModuleState() by
 * AfxGetAppModuleState()
 * 
 * *****************  Version 5  *****************
 * User: Oh           Date: 10/23/01   Time: 6:47p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * adapted to changes in cg_4claccess.idl
 * 
 * *****************  Version 4  *****************
 * User: Oh           Date: 7/05/01    Time: 5:27p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 3  *****************
 * User: Oh           Date: 6/26/01    Time: 9:35a
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:33p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 *==
 *----------------------------------------------------------------------------*
*/
