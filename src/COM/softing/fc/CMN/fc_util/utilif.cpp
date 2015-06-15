/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/utilif.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: utilif.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/utilif.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           FC_UTIL
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
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  10.04.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

//----  Local Defines:   -------------------------------------------*
#include "stdafx.h"
//implementation file
#define UTIL_IMP
#include "assert.h"
#include "util.h"
#include "utilif.h"
#include "buildnr.h"
#include "product.h"
#include "Resource.h"
#include "CeSysDef.h"
#include "md5.h"
#include "ice.h"

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


static bool FormatMsgArg(HINSTANCE hResHandle,LPTSTR pszOut, size_t sizeBuf, HRESULT hr,va_list pArg);

static BYTE staStdUTILCryptKey[UTIL_CRYPTKEYSIZE+1] = "jT4OeRrs82vI7dNY";


#define ISHEXDIGIT(ch)   ((ch >= 'a' && ch <= 'f') || \
                          (ch >= 'A' && ch <= 'F') || \
                          (ch >= '0' && ch <= '9'))

#define BUFFER_LEN  20      // used in UTIL_String2UTF8()

UTILIF HRESULT UTIL_GetInstallPath(IN LPTSTR pszPath,IN OUT size_t *pSizeChars)
/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>UTIL_GetInstallPath</B>.
*
*	Description = retrieves ATCMControl install path from the registry
*	The function stores the path in the passed @param pszPath.
*	<p>
*	Specification = 
*	<p>
*	@param	pszPath - string pointer where the install path will be
*			copied to
*			pSizeChars - pointer to size of the buffer in characters
*	<p>
*	@return hr = E_INVALIDARG if the param is Null
*	@return hr = E_OUTOFMEMORY a memory allocation operation fails
*	@return hr = S_FALSE registry key or value not found
*	@return hr = E_FAIL	- memory size to small *pSizeChars contains 
*				 necessary size.
*	@return hr = S_OK - Success
*
*	@exception No
*	<p>
*	@see
*	<p>
*/
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HKEY hKey;
	LONG lResult;
	DWORD dwBufSize,dwType;
	CString strPath;
	LPBYTE pBuffer;

	assert(pszPath);
	if(!pszPath)
		return E_INVALIDARG;

	*pszPath = _T('\0');

	//TODO define in cesysdef.h
	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				UTIL_INSTALL_KEY,
				0,
				KEY_READ,
				&hKey);

	if(lResult != ERROR_SUCCESS)
		return S_FALSE;
	
	lResult = RegQueryValueEx(hKey,UTIL_INSTALL_PATH,NULL,NULL,NULL,&dwBufSize);

	if(lResult != ERROR_SUCCESS) {
		//close the key
		RegCloseKey(hKey);
		return S_FALSE;
	}

	assert(dwBufSize);
	pBuffer = new BYTE[dwBufSize];
	if(!pBuffer) {
		//close the key
		RegCloseKey(hKey);
		return E_OUTOFMEMORY;
	}	

	//TODO define in cesysdef.h
	lResult = RegQueryValueEx(hKey,UTIL_INSTALL_PATH,NULL,&dwType,pBuffer,&dwBufSize);

	if(lResult != ERROR_SUCCESS) {
		//close the key
		RegCloseKey(hKey);
		delete [] pBuffer;
		return S_FALSE;
	}

	assert(dwType == REG_SZ);
	RegCloseKey(hKey);								
	strPath = pBuffer; //conversion
	delete [] pBuffer;

	if(*pSizeChars <= (size_t) strPath.GetLength()) {
		*pSizeChars = strPath.GetLength() + 1;
		return E_FAIL;
	}

	_tcscpy(pszPath,strPath);
	return S_OK;
}



UTILIF HRESULT UTIL_GetFileDates(LPCTSTR pszFilePath, VARIANT & vtCreate, VARIANT & vtModify) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	COleVariant vtFileDate;
	COleDateTime dtFile;
	CFileStatus FileStatus;

	VariantInit(&vtCreate);
	VariantInit(&vtModify);	    

	if(!CFile::GetStatus(pszFilePath,FileStatus)) {
		return(E_FAIL);
	}
	dtFile = FileStatus.m_mtime.GetTime();
	vtFileDate = dtFile;

	vtModify = vtFileDate;

	dtFile = FileStatus.m_ctime.GetTime();
	vtFileDate = dtFile;

	vtCreate = vtFileDate;
	return(S_OK);
}


UTILIF HRESULT UTIL_DisplaySysError(HRESULT hrError,LPCTSTR pszTitle,LPCTSTR pszOpt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    CString strMsg;
	CString strTitle;
    CString str;

    hr = UTIL_FormatMessage(NULL,str.GetBuffer(_MAX_PATH),_MAX_PATH - 1,hrError);
    str.ReleaseBuffer();

    if(hr == S_FALSE) {
        str.LoadString(IDS_NOTRESOLVE);
    }

    strMsg.LoadString(IDS_E_SYSERR);
    strMsg += _T(": ");
    strMsg += str;
    strMsg += _T('\n');
    str.Format(IDS_FATAL_ERROR2,hrError);
    strMsg += str;

    if(pszOpt) {
        strMsg += _T('\n');
        strMsg += CString(pszOpt);
    }

	if(pszTitle) {
		strTitle = pszTitle;
	}
	else {
		strTitle.LoadString(IDS_DEF_TITLE);
	}

	::MessageBox(NULL,strMsg,strTitle,MB_OK | MB_ICONEXCLAMATION);
	return S_OK;
}


UTILIF LANGID UTIL_GetInstallLanguage(void)
{
    /*  TODO retrieve the installed language from registry
        for now return english
    */
    return MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT);
}


UTILIF void UTIL_FatalError (LPCTSTR pszDescription, HRESULT hr)
{
	CString strMessage;

	// Do not "manage state" for call of AfxMessageBox.
	// Title will be taken from the calling context then.

	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		CString strTemp;

		strMessage.LoadString (IDS_FATAL_ERROR1);
		strMessage += _T ("\n");

		if (pszDescription != NULL)
		{
			strMessage += pszDescription;
			strMessage += _T ("\n");
		};

		strMessage += _T ("\n");

		strTemp.Format (IDS_FATAL_ERROR2, hr);
		strMessage += strTemp;
		strMessage += _T ("\n\n");

		strTemp.LoadString (IDS_FATAL_ERROR3);
		strMessage += strTemp;
	};

	AfxMessageBox (strMessage);
}

UTILIF HRESULT UTIL_GetStdLibPath(IN LPTSTR pszPath,IN size_t SizeChars)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HKEY hKey;
	LONG lResult;
	DWORD dwBufSize,dwType;
	CString strPath;
	LPBYTE pBuffer;

	assert(pszPath);
    if(!pszPath || SizeChars == 0) {
		return E_INVALIDARG;
    }

    try {
	    *pszPath = _T('\0');

	    lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				    UTIL_INSTALL_KEY,
				    0,
				    KEY_READ,
				    &hKey);

	    if(lResult != ERROR_SUCCESS)
		    return S_FALSE;
	    
	    lResult = RegQueryValueEx(hKey,UTIL_INSTALL_LIBPATH,NULL,NULL,NULL,&dwBufSize);
	    if(lResult != ERROR_SUCCESS) {
		    //close the key
		    RegCloseKey(hKey);
		    return S_FALSE;
	    }

	    assert(dwBufSize);
	    pBuffer = new BYTE[dwBufSize];
	    if(!pBuffer) {
		    //close the key
		    RegCloseKey(hKey);
		    return E_OUTOFMEMORY;
	    }	

	    lResult = RegQueryValueEx(hKey,UTIL_INSTALL_LIBPATH,NULL,&dwType,pBuffer,&dwBufSize);

	    if(lResult != ERROR_SUCCESS) {
		    //close the key
		    RegCloseKey(hKey);
		    delete [] pBuffer;
		    return S_FALSE;
	    }

	    assert(dwType == REG_SZ);
	    RegCloseKey(hKey);								
	    strPath = pBuffer; //conversion
	    delete [] pBuffer;

	    if(SizeChars <= (size_t) strPath.GetLength()) {
		    return E_FAIL;
	    }
	    _tcscpy(pszPath,strPath);
    }
    catch(CMemoryException* e) {
        e->Delete();
        return E_OUTOFMEMORY;
    }
	return S_OK;
}

UTILIF HRESULT UTIL_GetStdProjectPath(IN LPTSTR pszPath,IN size_t SizeChars)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT hr;
	CString strPath;

    hr = UTIL_GetInstallPath(pszPath, &SizeChars);
    if(hr != S_OK)
    {
        return hr;
    }
    try
    {
        CString strPath = pszPath;
        strPath += UTIL_STD_PRJ_PATH;
	    if(SizeChars <= (size_t) strPath.GetLength())
        {
		    return E_FAIL;
	    }
	    _tcscpy(pszPath,strPath);
    }
    catch(CMemoryException* e) 
    {
        e->Delete();
        return E_OUTOFMEMORY;
    }
    return S_OK;
}

UTILIF HRESULT UTIL_GetVersionString(UTIL_VERSION_SPEC spec, LPTSTR pszVersion,size_t SizeChars)
{
    HRESULT hr = S_OK;

    try {
        CString str;

        if(!pszVersion || !SizeChars) {
            throw E_INVALIDARG;
        }

        switch(spec) {
            case UTIL_FULL_VERSION:
                str = PROD_FULL_VERSION_STR;
                break;
            case UTIL_SHORT_VERSION:
                str = PROD_SHORT_VERSION_STR;
                break;
            default:
                throw E_INVALIDARG;
        }

	    if(SizeChars <= (size_t) str.GetLength()) {
		    throw E_FAIL;
	    }
        
        _tcscpy(pszVersion,str);
    }
    catch(CMemoryException * e) {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }
    catch(HRESULT hrRes) {
        hr = hrRes;
    }

    return hr;
}

UTILIF HRESULT UTIL_FormatMessage(HINSTANCE hResHandle,LPTSTR pszOut,size_t sizeBuf,HRESULT hr,...)
{
    va_list arg_ptr;
    bool bRes = false;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    FC_PCHECK_HR(pszOut,E_POINTER);
    if(sizeBuf == 0) {
        return E_INVALIDARG;
    }
    
    *pszOut = _T('\0'); //initialize string

    va_start(arg_ptr,hr);      
    bRes = FormatMsgArg(hResHandle,pszOut,sizeBuf,hr,arg_ptr);    
    va_end(arg_ptr);
    return bRes == true ? S_OK : S_FALSE;
}


bool FormatMsgArg(HINSTANCE hResHandle,LPTSTR pszOut, size_t sizeBuf, HRESULT hr,va_list pArg)
{
    DWORD dwRes;
    LPVOID lpMsgBuf = NULL;
    LANGID langId,deflangId;

    langId = UTIL_GetInstallLanguage();
    deflangId = ::GetSystemDefaultLangID();
    *pszOut = 0;

    //try with hmodule message table and installed language
    dwRes = ::FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE ,
        hResHandle,hr,
        langId, // installed language
        (LPTSTR) &lpMsgBuf,0,&pArg);

    if(dwRes) {
        lstrcpyn(pszOut,(LPCTSTR) lpMsgBuf,sizeBuf);
    }

    if(lpMsgBuf) {
        ::LocalFree(lpMsgBuf);
        lpMsgBuf = NULL;
    }
    
    if(dwRes) {
        return true;
    }

    //try with hmodule message table default language
    dwRes = ::FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE ,
        hResHandle,hr,
        deflangId, // Default language
        (LPTSTR) &lpMsgBuf,0,&pArg);

    if(dwRes) {
        lstrcpyn(pszOut,(LPCTSTR) lpMsgBuf,sizeBuf);
    }

    if(lpMsgBuf) {
        ::LocalFree(lpMsgBuf);
        lpMsgBuf = NULL;
    }

    if(dwRes) {
        return true;
    }

    //try with system message table alone and default language
    dwRes = ::FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,hr,
        deflangId, // Default language
        (LPTSTR) &lpMsgBuf,0,NULL);

    if(dwRes) {
        lstrcpyn(pszOut,(LPCTSTR) lpMsgBuf,sizeBuf);
    }

    if(lpMsgBuf) {
        ::LocalFree(lpMsgBuf);
    }

    if(dwRes) {
        return true;
    }

    return false;
}


UTILIF LPCTSTR UTIL_GetMasterPassword(void) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    
    static TCHAR szBuffer[_MAX_PATH];
	CString strWhoFile;
	CString strHelp;
    HRESULT hr;
	CStdioFile file;
    size_t  sSizeChars = _MAX_PATH - 1;
    
    hr = UTIL_GetInstallPath(strHelp.GetBuffer(_MAX_PATH - 1), &sSizeChars);
    strHelp.ReleaseBuffer();
    if(hr == S_OK) {
        strWhoFile = strHelp + WHO_FILE_NAME;
		strHelp.Empty();
	    if(file.Open (strWhoFile, CFile::modeRead | CFile::shareDenyWrite)) {
	        try{
                CString strLine;
                while(file.ReadString(strLine)) {
                    if(strLine.Find(WHO_ENGINEERING) != -1) {
                        strHelp = strLine.Right(strLine.GetLength() - (strLine.FindOneOf(_T("=")) + 1));
                        break;
                    }
                }
            }
	        catch(CException *e) {
		        e->Delete();
                strHelp.Empty();
	        }
	        file.Close ();
        }
    }
	assert(strHelp.GetLength() <= _MAX_PATH);
	lstrcpy(szBuffer,strHelp);
    return szBuffer;
}



//------------------------------------------------------------------*
/**
 * UTIL_BuildControlString()
 *
 *  Build a control string used for communication purposes
 *  control string is built up like this:
 *  "<OPCServer>,<CustomConnectGuid>,<CommChannel>(<ConnectParameters>)[,VERSION(<FirmwareVersion>)]"
 *      where:
 *      <OPCServer>: OPC-Server address
 *      <CustomConnectGuid>: GUID of custom connect interface
 *      <CommChannel>: communication channel "TCP" or "RS232" ...
 *      <ConnectParameters>: connect parameter string depending on communication channel
 *      <FirmwareVersion>: internal firmware version string (optional)
 *
 *  ****************** caution *******************
 *  * minimum buffer sizes are noted in          *
 *  * square brackets after each parameter !!    *
 *  **********************************************
 *  
 * @param           pszOPCServer: OPC-Server address
 * @param           pszCustomConnectGuid: custom connect guid
 * @param           pszOPCInstName: OPC-Server instance name used for OPC server identification
 * @param           pszCommChannel: communication channel
 * @param           pszConnectPar: connect parameter string
 * @param           pszProtocol: protocol string
 * @param           pszProtocolPar: protocol parameter string
 * @param           pszFirmwareVersion: firmware version string (may be NULL)
 * @param           pszControlString: builded control string (output) [UTIL_MAX_CONTROL_STRING_LEN]
 * @return          S_OK: ok
 *                  E_INVALIDARG: required argument was invalid (NULL)
 *                  E_FAIL: buffer too small
 * @exception       -
 * @see             -
*/
UTILIF HRESULT UTIL_BuildControlString(
    IN LPCTSTR pszOPCServer, 
    IN LPCTSTR pszCustomConnectGuid, 
    IN LPCTSTR pszOPCInstName,
    IN LPCTSTR pszCommChannel, 
    IN LPCTSTR pszConnectPar,
    IN LPCTSTR pszProtocol,
    IN LPCTSTR pszProtocolPar,
    IN LPCTSTR pszFirmwareVersion,
    LPTSTR pszControlString)
{
    CString strControl;
    CString strConnectString;
    CString strConnectFormatString;
    CString strProtocol;
    CString strFirmwareVersion;

    size_t SizeChars = UTIL_MAX_CONTROL_STRING_LEN;

    if( pszOPCServer == NULL || 
        pszCustomConnectGuid == NULL || 
        pszOPCInstName == NULL ||
        pszCommChannel == NULL ||
        pszConnectPar == NULL || 
        pszProtocol == NULL ||
        pszProtocolPar == NULL ||
        pszControlString == NULL)
    {
        return E_INVALIDARG;
    }

    strProtocol.Format("%s(%s)", pszProtocol, pszProtocolPar);

    if(pszFirmwareVersion)
    {
        strFirmwareVersion.Format("%s(%s)", UTIL_VERSION_STRING, pszFirmwareVersion);
    }

    strConnectString.Format("%s(%s)", pszCommChannel, pszConnectPar);

    if(strFirmwareVersion.IsEmpty())
    {
        strControl.Format("%s,%s,%s,%s,%s", pszOPCServer, pszCustomConnectGuid, pszOPCInstName, strConnectString, strProtocol);
    }
    else
    {
        strControl.Format("%s,%s,%s,%s,%s,%s", pszOPCServer, pszCustomConnectGuid, pszOPCInstName, strConnectString, strProtocol, strFirmwareVersion);
    }

    // check buffer size
    if((UINT)strControl.GetLength() >= SizeChars)
    {
        return E_FAIL;
    }

    _tcscpy(pszControlString, strControl);
 
    return S_OK;
}


// Help function for UTIL_ParseControlString()
// and UTIL_ParseTargetControlString()
//------------------------------------------------------------------*
/**
 * evaluate bracketed term.
 *
 *  bracketed term: XXXX(YYYY)
 *  XXXX: prefix
 *  YYYY: contents
 *
 * @param           rstrInput: input string (in/out)
 * @param           rstrPrefix: prefix of term
 * @param           rstrContents: contents of term
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL EvaluateBracketedTerm(CString& rstrInput, CString& rstrPrefix, CString& rstrContents)
{
    CString strExtracted;   // extracted string
    int     iStart = -1;
    int     iStop = -1;
    int     iBracketsOpen = 0;
    int     iNumChars = rstrInput.GetLength();
    int     iChar;
    TCHAR   cCurrent;

    // search for first occurrence of bracket open
    for(iChar = 0; iChar < iNumChars; ++iChar)
    {
        if(rstrInput[iChar] == UTIL_BRACKET_OPEN)
        {
            ++iBracketsOpen;
            iStart = iChar + 1;
            break;
        }
    }

    if(!iBracketsOpen)
    {
        return FALSE;    // no bracket open found -> return FALSE
    }

    // search for corresponding bracket close
    for(iChar = iStart; iChar < iNumChars; ++iChar)
    {
        cCurrent = rstrInput[iChar];
        if(cCurrent == UTIL_BRACKET_OPEN)
        {
            ++iBracketsOpen;
        }
        if(cCurrent == UTIL_BRACKET_CLOSE)
        {
            --iBracketsOpen;
            if(iBracketsOpen == 0)
            {
                iStop = iChar;
                break;
            }
        }
    }

    if(iStop >= iStart)
    {
        strExtracted = rstrInput.Mid(iStart, iStop - iStart);
        rstrPrefix = rstrInput.Left(iStart - 1);
        rstrInput = rstrInput.Mid(iStop + 1);
        rstrContents = strExtracted;
        return TRUE;
    }

    return FALSE;
}


//------------------------------------------------------------------*
/**
 * UTIL_ParseControlString()
 *
 *  Parse a control string used for communication purposes
 *  Uses UTIL_ParseTargetControlString()
 *  Any parameter may be NULL if not required
 *  ****************** caution *******************
 *  * minimum buffer sizes are noted in          *
 *  * square brackets after each parameter !!    *
 *  **********************************************
 *
 * @param           pszControlString: control string to be parsed (input)
 * @param           pszOPCServer: OPC-Server address (output) [UTIL_MAX_OPC_SERVER_LEN]
 * @param           pszCustomConnectGuid: custom connect guid [UTIL_MAX_GUID_LEN]
 * @param           pszOPCInstName: OPC-Server instance name [UTIL_MAX_OPC_INST_NAME_LEN]
 * @param           pszCommChannel: communication channel [UTIL_MAX_COMM_CHANNEL_LEN]
 * @param           pszConnectPar: connect parameter string [UTIL_MAX_CONNECT_PAR_LEN]
 * @param           pszProtocol: protocol string [UTIL_MAX_PROTOCOL_LEN]
 * @param           pszProtocolPar: protocol parameter string [UTIL_MAX_PROTOCOL_PAR_LEN]
 * @param           pszFirmwareVersion: firmware version string [UTIL_MAX_FIRMWARE_VERSION_LEN]
 * @return          S_OK: ok
 *                  E_FAIL: syntax error or missing fields
 *                  E_INVALIDARG: required argument was invalid (NULL)
 * @exception       -
 * @see             UTIL_BuildControlString()
*/
UTILIF HRESULT UTIL_ParseControlString(
    IN LPCTSTR pszControlString,
    LPTSTR pszOPCServer, 
    LPTSTR pszCustomConnectGuid, 
    LPTSTR pszOPCInstName,
    LPTSTR pszCommChannel, 
    LPTSTR pszConnectPar,
    LPTSTR pszProtocol,
    LPTSTR pszProtocolPar,
    LPTSTR pszFirmwareVersion)
{
    CString strControl;
    CString strOPCServer;
    CString strCustomConnectGuid;
    CString strRest;
    HRESULT hr = S_OK;

    size_t SizeOPCServer = UTIL_MAX_OPC_SERVER_LEN;
    size_t SizeCustomConnectGuid = UTIL_MAX_GUID_LEN;

    if(pszControlString == NULL)
    {
        return S_OK;    // nothing to do
    }

    strControl = pszControlString;
    int iLength = strControl.GetLength();

    // strip '{' and '}' if existing
    if(iLength > 1)
    {
        if(strControl[0] == _T('{'))
        {
            strControl = strControl.Mid(1);
        }
        iLength = strControl.GetLength();
        if(strControl[iLength - 1] == _T('}'))
        {
            strControl = strControl.Left(iLength - 1);
        }
    }

    int iStart = strControl.Find(_T(','));
    if(iStart == -1)
    {
        strOPCServer = strControl;
        if(pszOPCServer)
        {
            if((UINT)strOPCServer.GetLength() < SizeOPCServer)
            {
                _tcscpy(pszOPCServer, strOPCServer);
            }
        }
        return S_FALSE;
    }

    int iEnd = strControl.Find(_T(','), iStart+1);
    strOPCServer = strControl.Left(iStart);
    strOPCServer.TrimLeft();
    strOPCServer.TrimRight();

    if(iEnd > iStart)
    {
        strCustomConnectGuid = strControl.Mid(iStart + 1, iEnd - iStart - 1);
        strCustomConnectGuid.TrimLeft();
        strCustomConnectGuid.TrimRight();

        strRest = strControl.Mid(iEnd + 1);
        strRest.TrimLeft();
        strRest.TrimRight();

        // now copy results to function arguments
        if(pszOPCServer)
        {
            if((UINT)strOPCServer.GetLength() < SizeOPCServer)
            {
                _tcscpy(pszOPCServer, strOPCServer);
            }
            else
            {
                hr = E_FAIL;
            }
        }
        if(pszCustomConnectGuid)
        {
            if((UINT)strCustomConnectGuid.GetLength() < SizeCustomConnectGuid)
            {
                _tcscpy(pszCustomConnectGuid, strCustomConnectGuid);
            }
            else
            {
                hr = E_FAIL;
            }
        }

        HRESULT hrTmp = UTIL_ParseTargetControlString(strRest, pszOPCInstName, pszCommChannel, pszConnectPar, pszProtocol, pszProtocolPar, pszFirmwareVersion);
        if(hr == S_OK)
        {
            hr = hrTmp;
        }
        return hr;
    }
    return E_FAIL;
}



//------------------------------------------------------------------*
/**
 * UTIL_ParseTargetControlString()
 *
 *  Parse target portion of a control string used for communication purposes
 *  see also UTIL_ParseControlString() and UTIL_BuildControlString()
 *  Any parameter may be NULL if not required
 *  ****************** caution *******************
 *  * minimum buffer sizes are noted in          *
 *  * square brackets after each parameter !!    *
 *  **********************************************
 *
 * @param           pszTargetControlString: target control string to be parsed (input)
 * @param           pszOPCInstName: OPC-Server instance name [UTIL_MAX_OPC_INST_NAME_LEN]
 * @param           pszCommChannel: communication channel [UTIL_MAX_COMM_CHANNEL_LEN]
 * @param           pszConnectPar: connect parameter string [UTIL_MAX_CONNECT_PAR_LEN]
 * @param           pszProtocol: protocol string [UTIL_MAX_PROTOCOL_LEN]
 * @param           pszProtocolPar: protocol parameter string [UTIL_MAX_PROTOCOL_PAR_LEN]
 * @param           pszFirmwareVersion: firmware version string [UTIL_MAX_FIRMWARE_VERSION_LEN]
 * @return          S_OK: ok
 *                  E_FAIL: syntax error or missing fields
 *                  E_INVALIDARG: required argument was invalid (NULL)
 * @exception       -
 * @see             UTIL_ParseControlString(), UTIL_BuildControlString()
*/
UTILIF HRESULT UTIL_ParseTargetControlString(
    IN LPCTSTR pszTargetControlString,
    LPTSTR pszOPCInstName,
    LPTSTR pszCommChannel, 
    LPTSTR pszConnectPar,
    LPTSTR pszProtocol,
    LPTSTR pszProtocolPar,
    LPTSTR pszFirmwareVersion)
{
    HRESULT hr = S_OK;
    CString strRest(pszTargetControlString);
    CString strOPCInstName;
    CString strConnectPar;
    CString strCommChannel;
    CString strProtocol;
    CString strProtocolPar;
    CString strFirmwareVersion;

    size_t SizeOPCInstName = UTIL_MAX_OPC_INST_NAME_LEN;
    size_t SizeCommChannel = UTIL_MAX_COMM_CHANNEL_LEN;
    size_t SizeConnectPar = UTIL_MAX_CONNECT_PAR_LEN;
    size_t SizeProtocol = UTIL_MAX_PROTOCOL_LEN;
    size_t SizeProtocolPar = UTIL_MAX_PROTOCOL_PAR_LEN;
    size_t SizeFirmwareVersion = UTIL_MAX_FIRMWARE_VERSION_LEN;

    // if no comma found -> old version:
    // connect parameter = strRest (IP-address)
    // OPC Instance name = connect parameter
    int iCommaPos = strRest.Find(_T(','));
    if(iCommaPos == -1)
    {
        strConnectPar = strRest;
        strOPCInstName = strConnectPar;
    }
    else
    {
        strOPCInstName = strRest.Left(iCommaPos);
        strRest = strRest.Mid(iCommaPos + 1);

        if(EvaluateBracketedTerm(strRest, strCommChannel, strConnectPar))
        {
            strConnectPar.TrimLeft();
            strConnectPar.TrimRight();
        }
        else 
        {
            return S_FALSE;
        }
        if(!strRest.IsEmpty())
        {
            int     iFound; // help variable

            iFound = strRest.Find(_T(','));
            strRest = strRest.Mid(iFound + 1);   // remove comma
            strRest.TrimLeft();
            if(EvaluateBracketedTerm(strRest, strProtocol, strProtocolPar))
            {
                iFound = strRest.Find(_T(','));
                strRest = strRest.Mid(iFound + 1);   // remove comma
                strRest.TrimLeft();
                CString strTmp;
                if(EvaluateBracketedTerm(strRest, strTmp, strFirmwareVersion))
                {
                    if(!strTmp.CompareNoCase(UTIL_VERSION_STRING) == 0)
                    {
                        hr = E_FAIL;
                    }
                }
            }
        }
    }

    if(pszOPCInstName)
    {
        if((UINT)strOPCInstName.GetLength() < SizeOPCInstName)
        {
            _tcscpy(pszOPCInstName, strOPCInstName);
        }
        else
        {
            hr = E_FAIL;
        }
    }
    if(pszCommChannel)
    {
        if((UINT)strCommChannel.GetLength() < SizeCommChannel)
        {
            _tcscpy(pszCommChannel, strCommChannel);
        }
        else
        {
            hr = E_FAIL;
        }
    }
    if(pszConnectPar)
    {
        if((UINT)strConnectPar.GetLength() < SizeConnectPar)
        {
            _tcscpy(pszConnectPar, strConnectPar);
        }
        else
        {
            hr = E_FAIL;
        }
    }
    if(pszProtocol)
    {
        if((UINT)strProtocol.GetLength() < SizeProtocol)
        {
            _tcscpy(pszProtocol, strProtocol);
        }
        else
        {
            hr = E_FAIL;
        }
    }
    if(pszProtocolPar)
    {
        if((UINT)strProtocolPar.GetLength() < SizeProtocolPar)
        {
            _tcscpy(pszProtocolPar, strProtocolPar);
        }
        else
        {
            hr = E_FAIL;
        }
    }
    if(pszFirmwareVersion)
    {
        if((UINT)strFirmwareVersion.GetLength() < SizeFirmwareVersion)
        {
            _tcscpy(pszFirmwareVersion, strFirmwareVersion);
        }
        else
        {
            hr = E_FAIL;
        }
    }
    return hr;
}

//------------------------------------------------------------------*
/**
 * encrypt string
 *
 *  ensure that buffer size of pszEncrypted is at least 2*strlen(pszToEncrypt) + 16!
 *
 * @param           pszToEncrypt: string to be encrypted (must be null terminated!)
 * @param           pszEncrypted: encrypted hex version of string (buffer size see above)
 * @param           pszKey: 16 character encryption key (if NULL the UTIL standard key is used)
 * @return          HRESULT
 * @exception       -
 * @see             UTIL_DecryptString()
*/
UTILIF HRESULT UTIL_EncryptString(IN LPCTSTR pszToEncrypt, OUT LPTSTR pszEncrypted, IN LPCTSTR pszKey)
{
    int         i;
    int         iSize;
    int         iAllocSize;
    LPBYTE      byMem;
    ICE_KEY*    pIceKey;
    BYTE        abKey[UTIL_CRYPTKEYSIZE+1];
    HRESULT     hr;

    // get length of input string
    iSize = strlen(pszToEncrypt);

    if(iSize == 0)
    {
        pszEncrypted[0] = 0;
        return S_OK;
    }

    iAllocSize = ((iSize-1)/8 + 1) * 8;

    byMem = (LPBYTE) malloc(iAllocSize);
    if (NULL == byMem)
    {
        return E_OUTOFMEMORY;
    }

    ::memset((void*) byMem, 0, iAllocSize);
    ::memcpy((void*) byMem, (void*) pszToEncrypt, iSize);

    // copy key
    if(pszKey != 0)
    {
        int iKeySize = strlen(pszKey);
        if(iKeySize > UTIL_CRYPTKEYSIZE)
        {
            iKeySize = UTIL_CRYPTKEYSIZE;
        }
        ::memset((void*) abKey, 0, UTIL_CRYPTKEYSIZE+1);
        ::memcpy((void*) abKey, (void*)pszKey, iKeySize);
    }
    else
    {
        ::memcpy((void*) abKey, (void*)staStdUTILCryptKey, UTIL_CRYPTKEYSIZE);
    }

    pIceKey = ice_key_create(1);
    if (NULL == pIceKey)
    {
        free(byMem);
        return E_OUTOFMEMORY;
    }

    ice_key_set(pIceKey, abKey);
    for (i=0; i < iSize; i+=8)
    {
        ice_key_encrypt(pIceKey, byMem + i, byMem + i);
    }

    ice_key_destroy(pIceKey);

    hr = UTIL_ByteArrayToHexString(byMem, iAllocSize, pszEncrypted);
    
    free(byMem);

    return hr;
}

//------------------------------------------------------------------*
/**
 * decrypt string
 *
 *  ensure that pszDecrypted is at least as big as strlen(pszToDecrypt)/2 + 1
 *
 * @param           pszToDecrypt: hex string to be decrypted (must be null terminated!)
 * @param           pszDecrypted: decrypted string (buffer size big enough, see above!)
 * @param           pszKey: 16 character encryption key (if NULL the UTIL standard key is used)
 * @return          HRESULT
 * @exception       -
 * @see             UTIL_EncryptString()
*/
UTILIF HRESULT UTIL_DecryptString(IN LPCTSTR pszToDecrypt, OUT LPTSTR pszDecrypted, IN LPCTSTR pszKey)
{
    int         i;
    int         iSize;
    int         iAllocSize;
    LPBYTE      byMem;
    ICE_KEY*    pIceKey;
    BYTE        abKey[UTIL_CRYPTKEYSIZE+1];
    HRESULT     hr;

    if(strlen(pszToDecrypt) %2 != 0)
    {
        return E_FAIL;
    }

    // get length of input string
    iSize = strlen(pszToDecrypt) / 2;

    if(iSize == 0)
    {
        pszDecrypted[0] = 0;
        return S_OK;
    }

    iAllocSize = ((iSize-1)/8 + 1) * 8;

    byMem = (LPBYTE) malloc(iAllocSize);
    if (NULL == byMem)
    {
        return E_OUTOFMEMORY;
    }

    ::memset((void*) byMem, 0, iAllocSize);

    hr = UTIL_HexStringToByteArray(pszToDecrypt, byMem);

    // copy key
    if(pszKey != 0)
    {
        int iKeySize = strlen(pszKey);
        if(iKeySize > UTIL_CRYPTKEYSIZE)
        {
            iKeySize = UTIL_CRYPTKEYSIZE;
        }
        ::memset((void*) abKey, 0, UTIL_CRYPTKEYSIZE+1);
        ::memcpy((void*) abKey, (void*)pszKey, iKeySize);
    }
    else
    {
        ::memcpy((void*) abKey, (void*)staStdUTILCryptKey, UTIL_CRYPTKEYSIZE);
    }

    pIceKey = ice_key_create(1);
    if (NULL == pIceKey)
    {
        free(byMem);
        return E_OUTOFMEMORY;
    }

    ice_key_set(pIceKey, abKey);
    for (i=0; i < iSize; i+=8)
    {
        ice_key_decrypt(pIceKey, byMem + i, byMem + i);
    }

    ice_key_destroy(pIceKey);
    memcpy(pszDecrypted, byMem, iAllocSize);
    pszDecrypted[iAllocSize] = 0;
    free(byMem);
    return S_OK;
}


//------------------------------------------------------------------*
/**
 * calculate fingerprint for string
 *
 *  ensure that buffer size of pszOutput is at least 33
 *
 * @param           pszInput: string for which fingerprint is to be calculated
 * @param           pszOutput: 32 character fingerprint string + trailing 0
 * @return          S_OK
 * @exception       -
 * @see             -
*/
UTILIF HRESULT UTIL_GetStringFingerprint(IN LPCTSTR pszInput, OUT LPTSTR pszOutput)
{
    MD5_CTX         mdContext;
    int             iSize = strlen(pszInput);
    LPBYTE          lpByte = (LPBYTE) malloc(UTIL_FINGERPRINTSIZE);
    HRESULT         hr;

    if(lpByte == NULL)
    {
        return E_OUTOFMEMORY;
    }

    MD5Init(&mdContext);
    MD5Update(&mdContext, (unsigned char*)pszInput, iSize);
    MD5Final (&mdContext);
    ::memcpy((void*) lpByte, (void*)mdContext.digest, UTIL_FINGERPRINTSIZE);

    hr = UTIL_ByteArrayToHexString(lpByte, UTIL_FINGERPRINTSIZE, pszOutput);

    ::free(lpByte);

    return hr;
}


//------------------------------------------------------------------*
/**
 * convert byte array to hexadecimal string.
 *
 *  ensure that buffer size of pszHexString is at least 2*lSize
 *
 * @param           lpByte: byte array
 * @param           lSize: size of byte array
 * @param           pszHexString: hexadecimal string (buffer size see above)
 * @return          S_OK
 * @exception       -
 * @see             UTIL_HexStringToByteArray()
*/
UTILIF HRESULT UTIL_ByteArrayToHexString(IN LPBYTE lpByte, IN LONG lSize, OUT LPTSTR pszHexString)
{
    CString strHex;
    CString strTmp;
    BYTE*   pByte = lpByte;
    for(int i=0; i<lSize; ++i)
    {
        strTmp.Format("%02X", *pByte);
        ++pByte;
        strHex += strTmp;
    }
    strcpy(pszHexString, strHex);
    return S_OK;
}


//------------------------------------------------------------------*
/**
 * convert hexadecimal string to byte array.
 *
 *  ensure that size of lpByte is at least strlen(pszHexString) / 2 + 1
 *
 * @param           pszHexString: hexadecimal string
 * @param           lpByte: byte array (size see above)
 * @return          S_OK: ok
 *                  E_FAIL: strlen(pszHexString) is odd or non hex digit encountered
 * @exception       -
 * @see             UTIL_HexStringToByteArray()
*/
UTILIF HRESULT UTIL_HexStringToByteArray(IN LPCTSTR pszHexString, OUT LPBYTE lpByte)
{
    CString strTmp;
    CString strHex(pszHexString);
    CString strPrefix(_T("0x"));
    BYTE*   pByte = lpByte;
    int     iTmp;

    int     i = 0;

    // hex string length must be even
    if(strlen(pszHexString) %2 != 0)
    {
        return E_FAIL;
    }

    while(pszHexString[i] != 0)
    {
        strTmp = strPrefix + strHex.Mid(i, 2);
        if(!ISHEXDIGIT(strTmp[2]) || !ISHEXDIGIT(strTmp[3]))
        {
            return E_FAIL;
        }
        sscanf(strTmp, "%X", &iTmp);
        *pByte = iTmp;
        i += 2;
        ++pByte;
    }
    return S_OK;
}

//------------------------------------------------------------------*
/**
 * UTIL_XML_EscapeSpecialCharacters
 *
 *  replace special XML characters by their escape sequence
 *  special characters are '&', '<', '>', ''', '"', TAB, NEWLINE and RETURN
 *
 * @param           lpszString: input string
 * @param           lpszOut: string with escaped characters
 * @param           SizeChars: size of lpszOut
 *                             to be shure that buffer is big enough
 *                             SizeChars has to be 6*strlen(lpszOut) + 1
 * @return          successful?
 * @exception       -
 * @see             -
*/
UTILIF BOOL UTIL_XML_EscapeSpecialCharacters(LPCTSTR lpszString, LPTSTR lpszOut, size_t SizeChars)
{
    char*   pChar = (char*)lpszString;
    CString strResult;
    BOOL    bResult = TRUE;

    while(*pChar != '\0')
    {
        switch(*pChar)
        {
        case '&':
            strResult += "&amp;";
            break;
        case '<':
            strResult += "&lt;";
            break;
        case '>':
            strResult += "&gt;";
            break;
        case '\'':
            strResult += "&apos;";
            break;
        case '"':
            strResult += "&quot;";
            break;
        case '\t':
            strResult += "&#09;";
            break;
        case '\n':
            strResult += "&#10;";
            break;
        case '\r':
            strResult += "&#13;";
            break;
        case ' ':
            strResult += "&#32;";
            break;
        default:
            strResult += *pChar;
        }
        pChar++;
    }
    _tcsncpy(lpszOut, strResult, SizeChars);
    if(strResult.GetLength() >= SizeChars)
    {
        bResult = FALSE;
        lpszOut[SizeChars-1] = 0;
    }

    return bResult;

}


//------------------------------------------------------------------*
/**
 * UTIL_String2UTF8
 *
 *  transforms an ASCII string into UTF-8 format
 *  the string is split in parts and each part is
 *  translated into unicode and from unicode to UTF-8
 *  if one translation step fails or the output buffer
 *  size is too small the return value is FALSE.
 *
 *  Attention:
 *    UTF-8 encodes one character with up to 3 characters
 *    escape sequence for one character may be up to 6 characters
 *
 * @param           lpszStringIn: input string
 * @param           lpszOut: output string (for size see just below)
 * @param           SizeChars: size of lpszOut
 *                             to be shure that buffer is big enough
 *                             SizeChars has to be 6*strlen(lpszOut) + 1
 * @return          successful?
 * @exception       -
 * @see             -
*/
UTILIF BOOL UTIL_String2UTF8(LPCTSTR lpszString, LPTSTR lpszOut, size_t SizeChars)
{
   	static WCHAR    szUnicodeBuffer[BUFFER_LEN+1];
    static CHAR     szBuffer[BUFFER_LEN*3+3];

    CString     strResult;

    UINT        uiLen;
    UINT        uiCharCount = 0;
    UINT        uiTmp;
    BOOL        bResult = TRUE;
    UINT        uiStep = BUFFER_LEN;
    CString     strWork;
    

    if(!UTIL_XML_EscapeSpecialCharacters(lpszString, lpszOut, SizeChars))
    {
        bResult = FALSE;
    }

    strWork = lpszOut;
    uiLen = strWork.GetLength();

    while(uiCharCount < uiLen)
    {
        uiTmp = (uiLen > uiCharCount + uiStep) ? uiStep : uiLen - uiCharCount;
        strcpy(szBuffer, strWork.Mid(uiCharCount, uiTmp));
	    if(MultiByteToWideChar(CP_ACP, 0, szBuffer, -1, szUnicodeBuffer, sizeof(szUnicodeBuffer)))
        {
	        WideCharToMultiByte(CP_UTF8, 0, szUnicodeBuffer, -1, szBuffer, sizeof(szBuffer), 0, 0);
            strResult += szBuffer;
        }
        else
        {
            DWORD dw;
            dw = GetLastError();
            bResult = FALSE;
            break;
        }
        uiCharCount += uiStep;
    }

    _tcsncpy(lpszOut, strResult, SizeChars);
    if(strResult.GetLength() >= SizeChars)
    {
        bResult = FALSE;
        lpszOut[SizeChars-1] = 0;
    }

    return bResult;
}


/*
 *----------------------------------------------------------------------------*
 *  $History: utilif.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 5  *****************
 * User: Sis          Date: 1.10.03    Time: 14:56
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * CString not allowed in header file (2)
 * 
 * *****************  Version 4  *****************
 * User: Sis          Date: 1.10.03    Time: 9:48
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * bug fix: may not use CString in header
 * 
 * *****************  Version 3  *****************
 * User: Sis          Date: 29.09.03   Time: 18:12
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * bug fix d-76: Euro sign problem in GR-Editor
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 29.09.03   Time: 14:08
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * bug fix d-2109: UTF-8 problems
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 16  *****************
 * User: Sis          Date: 6.05.03    Time: 14:23
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * bug fix d-2025
 * 
 * *****************  Version 15  *****************
 * User: Sis          Date: 7.08.02    Time: 13:52
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * crypto + fingerprint
 * 
 * *****************  Version 14  *****************
 * User: Sis          Date: 14.05.02   Time: 15:32
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * bug fix
 * 
 * *****************  Version 13  *****************
 * User: Sis          Date: 13.05.02   Time: 15:44
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * control string: added parameter pszOPCInstName
 * 
 * *****************  Version 12  *****************
 * User: Sis          Date: 6.05.02    Time: 16:27
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * changed control string functions
 * 
 * *****************  Version 11  *****************
 * User: Sis          Date: 3.05.02    Time: 11:52
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * new parameter in UTIL_BuildControlString(), UTIL_ParseControlString()
 * 
 * *****************  Version 10  *****************
 * User: Sis          Date: 26.04.02   Time: 16:37
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * control string functionality
 * 
 * *****************  Version 9  *****************
 * User: Sis          Date: 22.10.01   Time: 17:06
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * UTIL_GetMasterPassword()
 * 
 * *****************  Version 8  *****************
 * User: Sis          Date: 30.07.01   Time: 17:02
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * implemented UTIL_GetStdProjectPath()
 * 
 * *****************  Version 7  *****************
 * User: Su           Date: 10.04.01   Time: 12:35
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * UTIL_FormatMessage,UTIL_DisplaySysError bug fix, Header replaced.
 *==
 *----------------------------------------------------------------------------*
*/
