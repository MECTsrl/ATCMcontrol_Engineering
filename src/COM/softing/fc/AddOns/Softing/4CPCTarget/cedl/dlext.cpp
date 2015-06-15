
//----  Includes:   -------------------------------------------*
#include "stdafx.h"
//#include "htmlhelp.h"
#include "resource.h"
#include "CSC_OnlineErr.h"
#include "dlext.h"
#include "utilif.h"

static CEDL_STAT_DESC StateDescTbl[] = {
	{CEDL_STAT_STRT,_T("CEDL_STAT_STRT")},
	{CEDL_STAT_IDLE,_T("CEDL_STAT_IDLE")},
	{CEDL_STAT_ADAPT_2,_T("CEDL_STAT_ADAPT_2")},
	{CEDL_STAT_WAIT_CONNECT,_T("CEDL_STAT_WAIT_CONNECT")},
	{CEDL_STAT_GETDV,_T("CEDL_STAT_GETDV")},
	{CEDL_STAT_ADAPT_1,_T("CEDL_STAT_ADAPT_1")},
    {CEDL_STAT_DLCC,_T("CEDL_STAT_DLCC")},
    {CEDL_STAT_UPCC,_T("CEDL_STAT_UPCC")},
    {CEDL_STAT_DELCC,_T("CEDL_STAT_DELCC")},
	{CEDL_STAT_ABORT,_T("CEDL_STAT_ABORT")}
};

/*	error description table for other error sources 
	than the CEDL. CEDL error messages will be managed with
	CEDLError.mc. Errors coming from the CSC_Online will be
	managed with this table.
*/
static CEDL_ERR_DESC ErrorDescTbl[] = {
	{E_CSC_TRANSFER_SINGLE_DOWNLOADER_ALLOWED,IDS_CSC_SINGLE_DL_ALLOWED,_T("Only a single download session at the same time allowed to ATCM control")},
	{E_CSC_NO_VALID_LICENSE_INFO,IDS_CSC_NO_VALID_LIC,_T("Cannot access license info for CSC_Online server")},
	{E_CSC_NO_REMOTE_DLOAD_FOR_BUNDLE_ALLOWED,IDS_CSC_NO_REMOTE_DLOAD_FOR_BUNDLE,_T("This bundled license allows no remote download")},
	{E_CSC_NO_CONNECT_TO_CONTROL,IDS_CSC_NO_CONNECT_TO_CONTROL,_T("COM Connection to control failed")},
	{E_CSC_NO_FTP_SESSION,IDS_CSC_NO_FTP_SESSION,_T("Connection to Web server for transfer of visualisation domains failed")}
};

HRESULT WaitObjectMsg(DWORD dwCount,HANDLE *pHandles,DWORD *pWaitResult,DWORD dwTimeOut)
//waits for a message or a object (like event) beeing signalled
{
	DWORD dwTO = dwTimeOut;
	DWORD dwSucc;
	DWORD dwTics;


	while(TRUE) {
		dwTics = ::GetTickCount();
		dwSucc = MsgWaitForMultipleObjects(dwCount,pHandles,FALSE,dwTO,
            QS_ALLINPUT | QS_ALLPOSTMESSAGE);
		//All events
		if(dwSucc >= WAIT_OBJECT_0 && dwSucc < (WAIT_OBJECT_0 + dwCount)) {
			//one of the user events signalled state
			*pWaitResult = dwSucc - WAIT_OBJECT_0;
			return S_OK; //1 means look at result
		}
		
		//must be a Message 
		if(dwSucc == (WAIT_OBJECT_0 + dwCount)) {//must be a Message 
            MSG msg;	
    		
            if(::PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
				//TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if(dwTimeOut != INFINITE) {
				dwTics = ::GetTickCount() - dwTics; //time elapsed
				if(dwTics > dwTO) { //Time out
					*pWaitResult = 	WAIT_TIMEOUT;
					return S_FALSE;
				}
				dwTO = dwTO - dwTics;
			}

		}
		else { //WAIT_TIMEOUT or WAIT_ABANDONED	...
			*pWaitResult = dwSucc;
			return S_FALSE;
		}
	}
}

HRESULT CompareFiles(LPCTSTR pszFile1,LPCTSTR pszFile2)
// S_OK = identical, S_FALSE = not identical
{
	CFile File1,File2;
	CFileException expt;
	LPBYTE pbBuffer1,pbBuffer2;
	DWORD dwLen;
	HRESULT hr = S_OK;


	if(!File1.Open(pszFile1,CFile::modeRead,&expt)) {
		hr = HRESULT_FROM_WIN32(expt.m_lOsError);
		return hr;
	}
	
	if(!File2.Open(pszFile2,CFile::modeRead,&expt)) {
		hr = HRESULT_FROM_WIN32(expt.m_lOsError);
		return hr;
	}

	/*don't pass CFileException outside the function because
	  maybe we want to make a utility function with DLL interface from it
	*/
	try{
		
		dwLen = File1.GetLength();
		if(dwLen != File2.GetLength()) {
			File2.Close();
			File1.Close();
			return S_FALSE; //not equal
		}

		pbBuffer1 = new BYTE[dwLen];
		pbBuffer2 = new BYTE[dwLen];


		if(!pbBuffer1 || !pbBuffer2){
			File2.Close();
			File1.Close();
			return E_OUTOFMEMORY;
		}

		File1.Read(pbBuffer1,dwLen);
		File2.Read(pbBuffer2,dwLen);

		if(memcmp(pbBuffer1,pbBuffer2,dwLen))
			hr = S_FALSE;
			
			
		delete [] pbBuffer1;
		delete [] pbBuffer2;					

	}
	catch(CFileException *e) {
		hr = HRESULT_FROM_WIN32(e->m_lOsError);
		e->Delete();
	}
	File2.Close();
	File1.Close();
	return hr;
}

LPCTSTR GetStateDescription(CEDL_STAT State)
{
    static TCHAR szUnknown[] = _T("Unknown state");

    int nEntries  = sizeof(StateDescTbl)/sizeof(CEDL_STAT_DESC);

    for(int i = 0;i<nEntries;i++) {
        if(StateDescTbl[i].State == State)
            return StateDescTbl[i].szDescr;
    }

    return szUnknown;
}


void ClearFile(LPCTSTR pszFile)
//throws CFileException
{
    CFile File(pszFile,CFile::modeCreate | CFile::modeReadWrite);
    File.Close();
}

void FormatMsg(LPTSTR pszOut,HRESULT hr,...)
{
    va_list arg_ptr;
    va_start(arg_ptr,hr);      
    FormatMsgArg(pszOut,hr,arg_ptr);    
    va_end(arg_ptr);
}

void FormatMsgArg(LPTSTR pszOut,HRESULT hr,va_list pArg)
{
    HINSTANCE hInst;
    DWORD dwRes;
    LPVOID lpMsgBuf = NULL;
    LANGID langId,deflangId;

    hInst = AfxGetResourceHandle( );

    langId = UTIL_GetInstallLanguage();
    deflangId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
    *pszOut = 0;

    //try with hmodule message table and installed language
    dwRes = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE ,
        hInst,hr,
        langId, // installed language
        (LPTSTR) &lpMsgBuf,0,&pArg);

    if(dwRes) {
        lstrcpy(pszOut,(LPCTSTR) lpMsgBuf);
    }

    if(lpMsgBuf) {
        ::LocalFree(lpMsgBuf);
        lpMsgBuf = NULL;
    }
    
    if(dwRes) {
        return;
    }

    //try with hmodule message table default language
    dwRes = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE ,
        hInst,hr,
        deflangId, // Default language
        (LPTSTR) &lpMsgBuf,0,&pArg);

    if(dwRes) {
        lstrcpy(pszOut,(LPCTSTR) lpMsgBuf);
    }

    if(lpMsgBuf) {
        ::LocalFree(lpMsgBuf);
        lpMsgBuf = NULL;
    }

    if(dwRes) {
        return;
    }

    //try with system message table alone
    dwRes = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,hr,
        deflangId, // Default language
        (LPTSTR) &lpMsgBuf,0,NULL);

    if(dwRes) {
        lstrcpy(pszOut,(LPCTSTR) lpMsgBuf);
    }

    if(lpMsgBuf) {
        ::LocalFree(lpMsgBuf);
    }
}


//------------------------------------------------------------------*
/**
 * throws a _com_error exception with a created error information
 * object supporting the interface IErrorInfo. The error information
 * object can be used to determine the source of the error and the
 * cause of the error. If the HRESULT of the error passed is E_OUT-
 * OFMEMORY indicating an out of memory exception only a _com_error
 * exception will be generated without an error information object.
 * Further if the passed CLSID reference is CLSID_NULL only a _com_error
 * exception without a error information object will be generated.
 * The error description (IErrorInfo::SetDescription etc.) will be
 * generated through a call to ::FormatMessage with the HRESULT.
 * @param   clsid - CLSID indicating the source of the error.
 * @param   riid - Interface context of the error source.
 * @param   hr - HRESULT error code of the error
 * @param   ... additional parameters passed to FormatMessage to
 *  generate the error description.<br>
 * @return  -
 * @exception   _com_error (with error info object).
 * @see SDK Documentation on ::FormatMessage, interface IErrorInfo.
*/
void ComThrow(const CLSID& clsid, REFIID riid,HRESULT hr,...) throw()
{
    IErrorInfo* pInfo;

    LPTSTR pszOut;
    //special handling for E_OUTOFMEMORY
    if(hr == E_OUTOFMEMORY) {
        _com_raise_error(hr); //no error info object
    }
    
    {
        CString strOut;
        va_list arg_ptr;
    
        va_start(arg_ptr,hr);
        pszOut = strOut.GetBuffer(_MAX_PATH);
        FormatMsgArg(pszOut,hr,arg_ptr);
        strOut.ReleaseBuffer();
        va_end(arg_ptr);

        ComErrorInfo(clsid,riid,&pInfo,strOut);
    }
    _com_raise_error(hr,pInfo);
}

//------------------------------------------------------------------*
/**
 * Used in the catch block for a catched _com_error exception to
 * set the error information object with ::SetErrorInfo and returning
 * the HRESULT error code the corresponds with the exception.
 * If the exception _com_error object holds a error information object
 * exposing the interface IErrorInfo, ::SetErrorInfo with that object
 * will be called.
 * @param   _com_error exceptional object.           
 * @return  error code associated with the _com_error object.        
 * @exception       -
 * @see ::SetErrorInfo.
*/
HRESULT ComCatch(_com_error & err)
//catches thrown _com_error and sets error info for interface method
//call if available
{
    IErrorInfo* pei;
    pei = err.ErrorInfo();
    if(pei) {
        ::SetErrorInfo(0,pei);
        //do not release it, that is the task of the _com_error class
    }

    return err.Error();
}


HRESULT ComErrorInfo(const CLSID& clsid, REFIID riid, IErrorInfo ** ppErrInfo, LPCTSTR pszDesc) 
{
    USES_CONVERSION;
    ICreateErrorInfo* pcei = NULL;
    IErrorInfo * pei = NULL;
    LPOLESTR lpsz;

    if(ppErrInfo == NULL) {
        return E_POINTER;
    }

    *ppErrInfo = NULL;

    if(clsid == CLSID_NULL) {
        return S_FALSE;
    }

	HRESULT hr = CreateErrorInfo(&pcei);
    if(FAILED(hr)) {
        return hr;
    }

    ProgIDFromCLSID(clsid, &lpsz);
    if (lpsz != NULL) {
        pcei->SetSource(lpsz);    
        ::CoTaskMemFree(lpsz);
    }

    lpsz = T2OLE(pszDesc);
    pcei->SetDescription(lpsz);
	//Interface ID of the interface whose method is throwing the Except.
    pcei->SetGUID(riid);
    
    hr = pcei->QueryInterface(IID_IErrorInfo, (void**) &pei);
	if(SUCCEEDED(hr)) {
        *ppErrInfo = pei;
    }
        
    pcei->Release();	    
	return hr;
}


BOOL GetErrorDescription(HRESULT hr,LPTSTR pszBuffer,size_t sizeBuffer)
{
	CString str;
    int nEntries  = sizeof(ErrorDescTbl)/sizeof(CEDL_ERR_DESC);

	assert(pszBuffer);
	assert(sizeBuffer);

	*pszBuffer = _T('\0');

    for(int i = 0;i<nEntries;i++) {
        if(ErrorDescTbl[i].hr == hr) {
			if(!ErrorDescTbl[i].iIDS) {
				lstrcpyn(pszBuffer,ErrorDescTbl[i].szDescr,sizeBuffer);
				return TRUE;
			}

			str.LoadString(ErrorDescTbl[i].iIDS);
			lstrcpyn(pszBuffer,str,sizeBuffer);
			return TRUE;
		}
    }

    return FALSE;
}

void DisplayDaoException(CDaoException* e) 
{
	CString strMsg;
	if (e->m_pErrorInfo!=NULL)
	{
		strMsg.Format(
			_T("%s   (%d)\n\n")
			_T("Would you like to see help?"),
			(LPCTSTR)e->m_pErrorInfo->m_strDescription,
			e->m_pErrorInfo->m_lErrorCode);

		if (AfxMessageBox(strMsg, MB_YESNO) == IDYES)
		{
			WinHelp(GetDesktopWindow(),
					e->m_pErrorInfo->m_strHelpFile,
					HELP_CONTEXT,
					e->m_pErrorInfo->m_lHelpContext);
		}
	}
	else
	{
		strMsg.Format(
			_T("ERROR:CDaoException\n\n")
			_T("SCODE_CODE		=%d\n")	
			_T("SCODE_FACILITY	=%d\n")	
			_T("SCODE_SEVERITY	=%d\n")	
			_T("ResultFromScode	=%d\n"),
			SCODE_CODE		(e->m_scode),
			SCODE_FACILITY	(e->m_scode),
			SCODE_SEVERITY	(e->m_scode),
			ResultFromScode (e->m_scode));
		AfxMessageBox(strMsg);
	}
}

int GetMaxStringId(LPCTSTR pszHeader,int nArrIds[], size_t sizeArray)
//throws possibly CMemoryException
{
    int iLen,iID,iMaxLen = 0;
    CString str;

    for(unsigned i=0; i<sizeArray; i++) {
        str.LoadString(nArrIds[i]);
        iLen = str.GetLength();
        if(iLen > iMaxLen) {
            iMaxLen = iLen;
            iID = nArrIds[i];
        }
    }

    if(pszHeader) {
        iLen = _tcslen(pszHeader);
        if(iLen > iMaxLen) {
            iID = 0;
        }
    }

    return iID;
}

BOOL ShowHelp(HWND hWnd,DWORD dwData)
{
    BOOL bRet=TRUE;    
    CString strManualPath;
    UINT nStrLen = _MAX_PATH;

/*
    HRESULT hr = UTIL_GetInstallPath(strManualPath.GetBuffer(_MAX_PATH), &nStrLen);
    strManualPath.ReleaseBuffer();
    strManualPath += CE_MANUAL_FILENAME;

    bRet = (HtmlHelp(hWnd, 
        strManualPath, HH_HELP_CONTEXT, dwData) != NULL);
*/
    return bRet;
}

HRESULT CompareFilesForOnlineChange(LPCTSTR pszFile1, LPCTSTR pszFile2)
// Forbid changes in task structure
// S_OK = identical, S_FALSE = not identical 
{

	TCHAR* tmp;
	int    AllowChangesInField = 1;
	
	tmp = _tcsrchr(pszFile1, _T('\\'));
	if(_tcscmp(tmp, _T("\\retain.4pl")) == 0)
		return CompareFiles(pszFile1, pszFile2);

	if(_tcscmp(tmp, _T("\\__custdl.bin")) == 0)
		return CompareFiles(pszFile1, pszFile2);
    
    // TODO: check here all the conditions for online change
	//		 pszFile2 is old version of __common.hdr 
	//		 pszFile1 is new version of __common.hdr 
	//		 binary compare is not enough because the file contains also 
	//		 the number of objects (which is allowed to change)

	return S_OK;
}

