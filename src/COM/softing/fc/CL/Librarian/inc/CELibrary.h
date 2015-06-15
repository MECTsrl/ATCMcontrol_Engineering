/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/inc/CELibrary.h 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CELibrary.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/inc/CELibrary.h $
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

#ifndef __CELIBRARY_H_
#define __CELIBRARY_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "time.h"
#include "objconvert.h"
#include "CG_4CLAccess.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CELibraryList;
class ConstList;

typedef struct 
{
	long errId;
	long errTextId;
} CELibErrId;

class __declspec(dllexport) CELibrary  
{
public:
	typedef	enum errorID
	{		//error codes
		Err_NotOk=0,
		Err_Ok,
		Err_NoWriteAccess,
		Err_POUAlreadyExist,
		Err_POUNotExist,
		Err_FileNotFound,
		Err_POUFileNotExist,
		Err_DirectoryNotExist,
		Err_WrongPOUType,
		Err_WrongUsesEntry,
		Err_WrongHeaderDest,
		Err_CoCreateFailed,
		Err_ObjConvert,
	};

public:
	//
	// construction
	//
	CELibrary();
	virtual ~CELibrary();

	//
	// open, close ...
	//
	int Open(LPCTSTR fileName, LPCTSTR openMode);
	int Close();
	int IsOpen();
	int Commit();
	int SaveAs(LPCTSTR fileName, BOOL bChangeName=TRUE);
	int Import(LPCTSTR fileName);
	BOOL IsReadOnly();

	//
	// manipulation, information  
	//
	int AddPOU(LPCTSTR name, LPCTSTR type);
	int AddPOUFile(LPCTSTR name, LPCTSTR fullFileName);
	int AddPOUProperty(LPCTSTR name, LPCTSTR property, LPCTSTR value);
	int AddPOULinkFile(LPCTSTR linkFileName, LPTSTR pouName=NULL, BOOL bDeleteDouble=TRUE); // Layer 2
	
	int DeletePOU(LPCTSTR pouName);
	int DeletePOUFile(LPCTSTR pouName, LPCTSTR pouFileName);
	int DeletePOUProperty(LPCTSTR name, LPCTSTR property);
	void DeleteAllPOU();
	
	BOOL ExistPOU(LPCTSTR pouName);
	BOOL ExistPOUFile(LPCTSTR pouName, LPCTSTR pouFileName);

	int GetPOUCount();
	LPCTSTR GetPOUName(int position);
	int GetPOUType(LPCTSTR pouName, LPTSTR type); // Layer 2
	void GetPOU(int pos, LPTSTR name, LPTSTR type);
	int GetPOUFileAsBuffer(LPCTSTR ppouName, LPCTSTR ppouFileName, void* pBuffer, unsigned long ulBuffSize, unsigned long byteOffsetStart=0);
	int GetPOUHeaderAsBuffer(LPCTSTR pouName, void* pBuffer, unsigned long ulBuffSize); //Layer 2
	int GetPOUPublicHeaderAsBuffer(LPCTSTR pouName, void* pBuffer, unsigned long ulBuffSize); //Layer 2
	int GetPOUFileAsString(LPCTSTR pouName, LPTSTR pouContent);
	int GetPOUFile(LPCTSTR pouName, LPTSTR fileName, LPCTSTR fileExt="ST");
	int GetPOUFileCount(LPCTSTR pouName);
	int GetPOUFileByteSize(LPCTSTR pouName, LPCTSTR fileName, unsigned long* pulFileByteSize);
	int GetPOUHeaderByteSize(LPCTSTR pouName, unsigned long* pulFileByteSize); //Layer 2
	int GetPOUPublicHeaderByteSize(LPCTSTR pouName, unsigned long* pulFileByteSize); //Layer 2
	LPCTSTR GetPOUFileName(LPCTSTR pouName, int position);
	int GetPOUProperty(LPCTSTR name, LPCTSTR property, LPTSTR value);
	int GetPOUPropertyCount(LPCTSTR name);
	int GetPOUProperty(LPCTSTR pouName, int index, LPTSTR property, LPTSTR value, int bufferLength=0);

	int CheckoutPou(LPCTSTR pouName, LPCTSTR directory);
	int CheckoutPouFile(LPCTSTR pouName, LPCTSTR pouFile, LPCTSTR directory);
	
	int GetHeaderInfo(LPCTSTR pouName, LPTSTR headerFile, unsigned long& byteOffsetStart, unsigned long& byteOffsetEnd); // Layer 2
	int GetDisplayInfo(LPCTSTR pouName, LPTSTR displayFile, unsigned long& byteOffsetStart, unsigned long& byteOffsetEnd); // Layer 2

	//
	// password protection (Layer 2)
	//
	int SetPassWord(LPCTSTR passWord);
	int TestPassWord(LPCTSTR passWord);
	BOOL IsPassWordProtected();
	
	//
	// constant file parsing and constant information (layer 2)
	//
	int GetConstCount();
	int GetConst(int iIndex, LPTSTR pszConstName, LPTSTR pszConstType, LPTSTR pszConstValue);
	BOOL ExistConst(LPCTSTR pszConstName);
	
	//
	// general information
	//
	LPCTSTR GetName();
	time_t GetTimeStamp();
	static BOOL IsPOUName(LPCTSTR pszName);
	int GetObjectPath(LPCTSTR pszName, LPTSTR pszObjectPath); //Layer 2


	//
	// error handling
	//
	static LPCTSTR GetErrorMessage(long errId);
	LPCTSTR GetLastErrorMessage();

	//
	// helpers
	//
	static LPCTSTR GetFileNameInLib(LPCTSTR pouName, LPCTSTR fileName);
    I4CLAccess*         m_pILibAcc;

protected:
	//
	// manipulation, internal
	//
	int AddPOUFile(LPCTSTR name, LPCTSTR fullFileName, LPCTSTR libName, unsigned long byteOffsetStart, unsigned long nbytes);
	int AddPOUFileBuffer(LPCTSTR name, LPCTSTR pszBuffer, LPCTSTR libName, unsigned long byteOffsetStart, unsigned long nbytes);
	void AddInternalPOU(CString name, CString type, CString fileName="");
	void AddInternalPOUFile(CString name, CString fileName);
	int DeleteInternalPOU(const CString& pouName);
	int DeleteInternalPOUFile(const CString& pouName, const CString& pouFileName);
	int SetHeaderInfo(const CString& pouName, const CString& headerFile, const CString& byteOffsetStart, const CString& byteOffsetEnd);
	int SetDisplayInfo(const CString& pouName, const CString& displayFile, const CString& byteOffsetStart, const CString& byteOffsetEnd);

	//
	// other internals
	//
	//int CopyFiles2Lib(CEStorage* pCeStorage, CString pouName, CString fullFileName, CString libName = "", unsigned long byteOffsetStart=0, unsigned long nbytes=0);
	//int CopyBuffer2Lib(CEStorage* pCeStorage, CString pouName, LPCTSTR pszBufferName, CString libName, unsigned long byteOffsetStart=0, unsigned long nbytes=0);
	//int WriteLibList(CEStorage* pCeStorage);
	//int ReadLibList(CEStorage* pCeStorage);
	void SetTimeStamp(FILETIME timeStamp);
	void SetModified(BOOL bModified);
	BOOL IsModified();

	//
	// internals const file parsing
	//
	int ParseConstFiles();

	//
	// integration into document class
	//
	int AttachWorking2OrigStorage();
	int WriteWorkingLib2OrigLib();

	//
	// helpers
	//
	
	//
	// error handling
	//
	static void AnalyzeHRESULT(HRESULT hres);
	int SetError(int errId, const CString& errorInfo="");

	
	int m_readOnly;					// read only flag	
	CELibraryList* m_pLibList;		// lib 'directory'

	//CEStorage* m_pWorkingStorage;	//
	//CEStorage* m_pOrigStorage;		// original storage, for transacted mode only

	BOOL m_modeRead, m_modeWrite, m_modeTransacted, m_modeInteractive;
	BOOL m_modeCreate;

	ConstList* m_pConstList; // layer 2, list of constants

	static CELibErrId ErrIdList[];

#pragma warning(disable : 4251)
	private:
		CString m_fileName;
		int m_lastErrorId;
		CString m_lastErrorInfo;
		CString m_lastErrorText;
		CTime m_timeStamp;
		BOOL m_bModified;
		BOOL m_bConstParsed; // Layer 2
		IObjConvert* m_pObjConvert; // Layer 2
        
};

#endif // __CELIBRARY_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: CELibrary.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/inc
 * 
 * *****************  Version 3  *****************
 * User: Oh           Date: 7/05/01    Time: 5:27p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/inc
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:33p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/inc
 *==
 *----------------------------------------------------------------------------*
*/
