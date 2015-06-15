/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/OnlServError.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: OnlServError.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/OnlServError.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "onlserverror.h"
#include "csc_onlineerr.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

/*
    1.) Define HRESULT error code corresponding to VM error code first
    within CSC_OnlineErr.mc
    2.) Include the VM resp. target error code plus the mapping to HRESULT here
*/
static struct {
    /** VM error code*/
    IEC_UINT uError;
    /** HRESULT code within CSC_OnlineErr.h/.mc */
    HRESULT hr;
}   ErrorDescTbl[] = {
	{(IEC_UINT)-1,E_CSC_REQUEST_TIMEOUT}
};

static bool FormatMsgArg(HINSTANCE hResHandle,LPTSTR pszOut, size_t sizeBuf, HRESULT hr,va_list pArg);

static HRESULT ErrorCodeToHResult(IEC_UINT uErrNo)
{
    HRESULT hr = S_OK;
    int nEntries  = FC_ARRAY_LEN(ErrorDescTbl);

    for(int i = 0;i<nEntries;i++) {
        if(ErrorDescTbl[i].uError == uErrNo)
            hr = ErrorDescTbl[i].hr;
    }

    if(hr == S_OK) {//not found
        hr = E_CSC_REQUEST_TIMEOUT;//set to unknown error code
    }

    return hr;
}


//------------------------------------------------------------------*
/**
 * Maps VM specific error code retrieved from communication layers
 * transfer command to a message string. The VM error code will be
 * obtained directly from the receive buffer of the transfer command.
 * The error code will be mapped to a HRESULT code according to 
 * the ErrorDescTbl table defined above. The returned message BSTR
 * can be handed directly to the function CControlMgt::SendMsgToEng()
 * in order to place the message in the message queue transferred to
 * 4CE. The returned BSTR shall be freed by calling ::SysFreeString()
 *
 * <B>NOTE: To free the returned BSTR use macro FC_FREE_BSTR for con-
 * venience.</B>
 *
 * @param			BSTR * psMessage - returned BSTR containing the 
 *                  message. The returned BSTR shall be freed by calling 
 *                  ::SysFreeString(). Shall not be NULL.
 * @param			LPBYTE lpData - pointer to the data buffer returned
 *                  with the transfer command. Shall not be NULL.
 * @param			UINT cbData - length of data buffer
 * @param			...- variable list of additional arguments.
 * @return			bool - true - error code can be mapped to message
 *                  false - otherwise
 * @exception		CMemoryException.	
 * @see			    ErrorDescTbl, ErrorCodeToHResult()
*/
bool GetCommErrorMessage(BSTR * psMessage, LPBYTE lpData, UINT cbData,...)
{
    HRESULT hr;
    HINSTANCE hResHandle;
    IEC_UINT uErrNo = (IEC_UINT)-1;
    CString strOut;
    va_list arg_ptr;
    LPTSTR pszOut = NULL;

    assert(psMessage);
    assert(lpData);

    *psMessage = NULL;
    
    if (cbData>=sizeof(IEC_UINT) && lpData != NULL)
        uErrNo = TRANSLATE_SHORT( *(IEC_UINT*)lpData);
    

    hr = ErrorCodeToHResult(uErrNo);

    hResHandle = _Module.GetResourceInstance();
    assert(hResHandle);
    pszOut = strOut.GetBuffer(_MAX_PATH);
    assert(pszOut);

    va_start(arg_ptr,cbData);      
    bool bRes = FormatMsgArg(hResHandle,pszOut,_MAX_PATH,hr,arg_ptr);    
    va_end(arg_ptr);
    strOut.ReleaseBuffer();

    if(bRes) {
        *psMessage = strOut.AllocSysString();
    }

    return bRes;
}

//------------------------------------------------------------------*
/**
 * Maps VM specific error code to a message string. The error code 
 * will be mapped to a HRESULT code according to 
 * the ErrorDescTbl table defined above. The returned message BSTR
 * can be handed directly to the function CControlMgt::SendMsgToEng()
 * in order to place the message in the message queue transferred to
 * 4CE. The returned BSTR shall be freed by calling ::SysFreeString()
 *
 * <B>NOTE: To free the returned BSTR use macro FC_FREE_BSTR for con-
 * venience.</B>
 *
 * @param			BSTR * psMessage - returned BSTR containing the 
 *                  message. The returned BSTR shall be freed by calling 
 *                  ::SysFreeString(). Shall not be NULL.
 * @param			IEC_UINT uErrNo - VM specific error code.
 * @param			... - variable list of additional arguments.
 * @return			bool - true - error code can be mapped to message
 *                  false - otherwise
 * @exception		CMemoryException.	
 * @see			    ErrorDescTbl, ErrorCodeToHResult()
*/
bool GetCommErrorMessage(BSTR * psMessage, IEC_UINT uErrNo,...)
{
    HRESULT hr;
    HINSTANCE hResHandle;
    CString strOut;
    va_list arg_ptr;
    LPTSTR pszOut = NULL;

    assert(psMessage);
    *psMessage = NULL;
    
    hr = ErrorCodeToHResult(uErrNo);

    hResHandle = _Module.GetResourceInstance();
    assert(hResHandle);
    pszOut = strOut.GetBuffer(_MAX_PATH);
    assert(pszOut);

    va_start(arg_ptr,uErrNo);      
    bool bRes = FormatMsgArg(hResHandle,pszOut,_MAX_PATH,hr,arg_ptr);    
    va_end(arg_ptr);
    strOut.ReleaseBuffer();

    if(bRes) {
        *psMessage = strOut.AllocSysString();
    }

    return bRes;
}


//TODO : static helper - export this from fc_util and delete it, use fc_util instead
static bool FormatMsgArg(HINSTANCE hResHandle,LPTSTR pszOut, size_t sizeBuf, HRESULT hr,va_list pArg)
{
    DWORD dwRes;
    LPVOID lpMsgBuf = NULL;
    LANGID langId,deflangId;

    langId = MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT);
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

/* ---------------------------------------------------------------------------- */
