
#ifndef __DLEXT_H_
#define __DLEXT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#ifdef DEBUG
#define DUMP_STATIONSTATE(str,state) FC_DEBUGPRINT2(_T("CEDL> %s - State: %s\n"),(str),GetStateDescription((state)))
#else
#define DUMP_STATIONSTATE(str,state)
#endif
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
typedef struct {
    CEDL_STAT State;
    LPCTSTR szDescr; 
}CEDL_STAT_DESC;

/*error description struct for other error sources 
	than the CEDL. CEDL error messages will be managed with
	CEDLError.mc. Errors coming from the CSC_Online will be
	managed with this struct.
*/
typedef struct {
	HRESULT hr;
	int iIDS;
	LPCTSTR szDescr;
}CEDL_ERR_DESC;

//contains helper functions and classes for the component
void DisplayDaoException(CDaoException* e);

HRESULT CompareFiles(LPCTSTR pszFile1,LPCTSTR pszFile2);
HRESULT CompareFilesForOnlineChange(LPCTSTR pszFile1, LPCTSTR pszFile2);
HRESULT WaitObjectMsg(DWORD dwCount,HANDLE *pHandles,DWORD *pWaitResult,DWORD dwTimeOut);
LPCTSTR GetStateDescription(CEDL_STAT State);
BOOL GetErrorDescription(HRESULT hr,LPTSTR pszBuffer,size_t sizeBuffer);

void ClearFile(LPCTSTR pszFile);

void FormatMsg(LPTSTR pszOut,HRESULT hr,...);
void FormatMsgArg(LPTSTR pszOut,HRESULT hr,va_list pArg);
HRESULT ComCatch(_com_error & err);
HRESULT ComErrorInfo(const CLSID& clsid, REFIID riid, IErrorInfo ** ppErrInfo, LPCTSTR pszDesc);
void ComThrow(const CLSID& clsid, REFIID riid,HRESULT hr,...) throw();

int GetMaxStringId(LPCTSTR,int nArrIds[], size_t sizeArray);
BOOL ShowHelp(HWND hWnd,DWORD dwData);

#endif // __DLEXT_H_

