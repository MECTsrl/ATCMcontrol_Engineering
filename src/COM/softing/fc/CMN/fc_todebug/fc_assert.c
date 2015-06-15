/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_todebug/fc_assert.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_assert.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_todebug/fc_assert.c $
 *
 * =PROJECT	            CAK1020  ATCMControl V2.0
 *
 * =SWKE                fc
 *
 * =COMPONENT           fc_todebug
 *
 * =CURRENT             $Date: 28.02.07 19:00 $
 *                      $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *== The implementation of assert and debug prints.
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  02.01.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/



#include <tchar.h>
#include <windows.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define FC_DBAPI __declspec(dllexport)
#include "fc_todebug\fc_assert.h"
#include "fc_mem_l.h"

//Number of elements of an array
#define ARRAY_LEN(var) (sizeof(var)/sizeof((var)[0]))
//static helper function:

static void StringTrimRight(TCHAR* psz)
{
	TCHAR* p     = psz;
	TCHAR* pLast = NULL;

	while(*p)
	{
		if(_istspace(*p))
        {
			if(pLast==NULL)
				pLast = p;
        }
		else
        {
			pLast = NULL;
        }

		p = _tcsinc(p);
	}

	if(pLast)
        *pLast = 0;
}


static const TCHAR* StringFmtSysErr(
    DWORD    dwSystemErr, 
    TCHAR    szBuffer[/*nBuffer*/], 
    unsigned nBuffer
)
{
    DWORD dw;
    
    assert(nBuffer>64);
    if(nBuffer==0)
        return szBuffer;

    szBuffer[0] = 0;

    dw = FormatMessage( 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        dwSystemErr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),// Default language
        szBuffer,
        nBuffer-2,
        NULL 
    );
    if(dw!=0)
    {
        //no error:
        szBuffer[nBuffer-1] = 0;//be safe...

        //some sys error have an unnice newline at the end:
        StringTrimRight(szBuffer);
    }
    return szBuffer;
} 





//defines:
//int FC_TcharToMultiByte(LPCTSTR lpT, char lpM[/*cch*/], int cch);
//int FC_MultiByteToTchar(const char* lpM, TCHAR lpT[/*cch*/], int cch);
#ifdef UNICODE //???????? should be removed
 #define FC_TcharToMultiByte(lpT,lpM,cch) FC_WideCharToMultiByte(lpT,lpM,cch)
 #define FC_MultiByteToTchar(lpM,lpT,cch) FC_MultiByteToWideChar(lpM,lpT,cch)
#else
 #define FC_TcharToMultiByte(lpT,lpM,cch) lstrcpyn(lpM,lpT,cch)
 #define FC_MultiByteToTchar(lpM,lpT,cch) lstrcpyn(lpT,lpM,cch)
#endif



#define FC_EVENTLOG_SOURCE_NAME _T("4Control")
#define FC_EVENT_ASSERT         1

#define FC_MAXASSERTLOGFILE_LEN  (1*1024L*1024L)

#define FC_DEBUGINI_FILE        _T("debug.ini")
#define FC_MODULE_SECTION       _T("trace")
#define FC_DEFAULT_ENTRY        _T("default")

#define FC_FILENAME_SECTION     _T("file")
#define FC_FILENAME_ENTRY       _T("default")

#define FC_MAX_CACHE_SIZE       20
#define FC_MAX_MODULE_SIZE      16

//types:
typedef struct
{
    int  bUsed;   // second chance
    int  bDebugOn;
    TCHAR szModule[FC_MAX_MODULE_SIZE];
} FC_CACHE_ENTRY;



// static functions
static void FC_InitTraceFile(void);

#if defined(WIN32) && !defined(NDEBUG)
static BOOL bTlsOk;
#endif


#ifdef WIN32
#define THREAD_LOCAL  __declspec(thread)
#else
#define THREAD_LOCAL
#endif

#ifndef NDEBUG
static int              bEraseFile;
static CHAR             cszOutputFileName[MAX_PATH];
#ifdef WIN32
static CRITICAL_SECTION csOutputFileInUse;
#endif

static THREAD_LOCAL int nLastRecentlyUsed = 0;
static THREAD_LOCAL int bDebugOn          = TRUE;
static THREAD_LOCAL FC_CACHE_ENTRY        theCache[FC_MAX_CACHE_SIZE];


BOOL APIENTRY DllMain( HANDLE hModule, 
                        DWORD ul_reason_for_call, 
                        LPVOID lpReserved )
{
  switch( ul_reason_for_call ) {
    case DLL_PROCESS_ATTACH:
      DisableThreadLibraryCalls(hModule);
      FC_InitTraceFile();
      FC_InitMemorySystem();
      break;
    case DLL_PROCESS_DETACH:
      FC_ExitMemorySystem();
      break;
    }
  return TRUE;
}


/****************************************************************************/
static void FC_InitTraceFile(void)
{
  TCHAR            tszOutputFileName[MAX_PATH];
  TCHAR            tszModuleFileName[MAX_PATH];

  __try {
    int i;

    bTlsOk = nLastRecentlyUsed>=0 && nLastRecentlyUsed<FC_MAX_CACHE_SIZE;
    bTlsOk &= bDebugOn == TRUE || bDebugOn == FALSE;
    for (i=0; i<FC_MAX_CACHE_SIZE; i++)
      {
        bTlsOk &= theCache[i].bUsed == TRUE || theCache[i].bUsed == FALSE;
        bTlsOk &= theCache[i].bDebugOn == TRUE || theCache[i].bDebugOn == FALSE;
        bTlsOk &= (lstrlen(theCache[i].szModule)<sizeof(theCache[i].szModule));
      }

  } __except(GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION
                ? EXCEPTION_EXECUTE_HANDLER
                : EXCEPTION_CONTINUE_SEARCH)
  {
    bTlsOk = FALSE;
  }

  if (GetModuleFileName(NULL,tszModuleFileName,ARRAY_LEN(tszModuleFileName)))
    {
      LPTSTR lpExt;
      LPTSTR lpModule = _tcsrchr(tszModuleFileName, _T('\\'));
      if (lpModule)
        {
          lpModule++;
          lpExt = _tcschr(lpModule, _T('.'));
          if (lpExt)
            *lpExt = _T('\0');
        }
      else
        lpModule = tszModuleFileName;
      GetPrivateProfileString(FC_FILENAME_SECTION, lpModule, _T(""),
                              tszOutputFileName, ARRAY_LEN(tszOutputFileName),
                              FC_DEBUGINI_FILE);
    }
  else
    tszOutputFileName[0] = _T('\0');

  if (tszOutputFileName[0] == _T('\0'))
    GetPrivateProfileString(FC_FILENAME_SECTION, FC_FILENAME_ENTRY, _T(""),
                           tszOutputFileName, ARRAY_LEN(tszOutputFileName),
                           FC_DEBUGINI_FILE);
  
FC_TcharToMultiByte( tszOutputFileName, cszOutputFileName,ARRAY_LEN(cszOutputFileName));
if (cszOutputFileName[0])
  {
    bEraseFile = TRUE;
    InitializeCriticalSection(&csOutputFileInUse);
  }

FC_DEBUGPRINT3(_T("FC_ASSERT> Module=\"%s\", Output=\"%s\", bTlsOk=%d\n"),
                    tszModuleFileName, tszOutputFileName, bTlsOk);
}
#endif


/****************************************************************************/
static int FC_IsDebugOn(LPCTSTR lpsz)
{
#ifdef NDEBUG
  return TRUE;
#else
  int i;
  int bDefault;
  TCHAR szModule[FC_MAX_MODULE_SIZE];

#ifdef WIN32
  if (!bTlsOk)
    return TRUE;
#endif

  for (i=0; i<FC_MAX_MODULE_SIZE && lpsz[i]; i++)
    if (lpsz[i]<=_T(' ') || lpsz[i]==_T('>'))
      break;

  if (i==0 || i>=FC_MAX_MODULE_SIZE || lpsz[i] != _T('>')) // no module id!
    return bDebugOn;

  lstrcpyn(szModule, lpsz, i+1);

  if (!lstrcmpi(szModule, _T("ASSERT")))
    return TRUE;

  i = nLastRecentlyUsed;
  do {
    if (!lstrcmpi(szModule, theCache[nLastRecentlyUsed].szModule))
      {
        theCache[nLastRecentlyUsed].bUsed = TRUE;
        return (bDebugOn = theCache[nLastRecentlyUsed].bDebugOn);
      }
    nLastRecentlyUsed++;
    if (nLastRecentlyUsed==FC_MAX_CACHE_SIZE)
      nLastRecentlyUsed = 0;
  } while (i != nLastRecentlyUsed);

  // none found, look for a unused entry

  do {
    if (!theCache[nLastRecentlyUsed].bUsed)
        break;

    theCache[nLastRecentlyUsed].bUsed = FALSE;  // second chance
    nLastRecentlyUsed++;
    if (nLastRecentlyUsed==FC_MAX_CACHE_SIZE)
      nLastRecentlyUsed = 0;
  } while (1);

  bDefault = GetPrivateProfileInt(FC_MODULE_SECTION, FC_DEFAULT_ENTRY, TRUE, FC_DEBUGINI_FILE);
  theCache[nLastRecentlyUsed].bDebugOn =
             GetPrivateProfileInt(FC_MODULE_SECTION, szModule, bDefault, FC_DEBUGINI_FILE);

  lstrcpy(theCache[nLastRecentlyUsed].szModule, szModule);
  theCache[nLastRecentlyUsed].bUsed = TRUE;
  return (bDebugOn = theCache[nLastRecentlyUsed].bDebugOn);
#endif
}


/****************************************************************************/
FC_DBAPI void FC_DBDECL FC_DebugPrintA(
/****************************************************************************/
    IN LPCSTR lpszFormat,
    IN ...)
/****************************************************************************/
{
  va_list ap;
  CHAR cBuff[1024];

  va_start(ap, lpszFormat);
  wvsprintfA(cBuff, lpszFormat, ap);
  va_end(ap);

  if (FC_IsDebugOn(cBuff))
  {
    OutputDebugStringA(cBuff);

#ifndef NDEBUG
    if (cszOutputFileName[0])
    {
    FILE *fout;
    EnterCriticalSection(&csOutputFileInUse);

    fout = fopen(cszOutputFileName, bEraseFile ? "w" : "a");
    if (fout != NULL)
      {
      bEraseFile = FALSE;
      fseek(fout,0L,SEEK_END);
      if (ftell(fout)<FC_MAXASSERTLOGFILE_LEN) // only write this if there is less than 1MB log-file
        fwrite(cBuff, strlen(cBuff), 1, fout);
      fclose(fout);
      }

    LeaveCriticalSection(&csOutputFileInUse);
    }
#endif

  }

}


/****************************************************************************/
FC_DBAPI void FC_DBDECL FC_DebugPrintW(
/****************************************************************************/
    IN LPCWSTR lpszFormat,
    IN ...)
/****************************************************************************/
{
  va_list ap;
  WCHAR wBuff[1024];

  va_start(ap, lpszFormat);
  wvsprintfW(wBuff, lpszFormat, ap);
  va_end(ap);

  FC_DebugPrintA("%ls", wBuff);
}


/****************************************************************************/
FC_DBAPI void FC_DBDECL FC_Assert(
/****************************************************************************/
/* FC_Assert wird nicht UNICODE */
    LPCSTR lpFile,
    int Line,
    LPCSTR  lpString)
/****************************************************************************/
{
    /* just in case: print to DBWin anyway (if an assertion happens nobody cares about odd mouse problems...) */
    FC_DebugPrint(_T("ASSERT> Assertion in File %hs (Line %d) : %hs\n"), lpFile, Line, lpString);

#ifdef DEBUG
{
    TCHAR szAssertStr[512];
    wsprintf (szAssertStr, 
      _T("The assertion \"%hs\"\nhas failed in file \"%hs\"\nat line %d."), 
      lpString, lpFile, Line);

    if (IDCANCEL == MessageBox(NULL, szAssertStr, _T("Assertion Failed!"),
             MB_OKCANCEL|MB_ICONEXCLAMATION|MB_SERVICE_NOTIFICATION))
    {
      __try{
        DebugBreak();
      }__except(UnhandledExceptionFilter(GetExceptionInformation()))
      {}
    }
}
#endif
}

/****************************************************************************/
FC_DBAPI void FC_DBDECL FC_PrintGetLastError(LPCSTR lpszFile, int Line)
/****************************************************************************/
{
    TCHAR szErrText[256];
    DWORD dwError = GetLastError();

    if(dwError!=ERROR_SUCCESS)
    {
        StringFmtSysErr(dwError, szErrText, ARRAY_LEN(szErrText));
        FC_DEBUGPRINT3(_T("FC_ASSERT> GetLastError() occurred at %hs line %d: %s\n"), 
             lpszFile, Line, szErrText);
    }
}


FC_DBAPI void FC_DBDECL FC_AssertGetLastError(
    const char*  pszFile, 
    int          iLine,
    const TCHAR* pszHint
)
{
    DWORD dwError = GetLastError();
    if(dwError!=ERROR_SUCCESS)
        FC_AssertSysErr(pszFile, iLine, dwError, pszHint);
}


FC_DBAPI void FC_DBDECL FC_PrintSysErr(
    const char*  pszFile, 
    int          iLine, 
    DWORD        dwerr,
    const TCHAR* pszHint
)
{
    TCHAR szErrText[256];
    
    if(dwerr!=ERROR_SUCCESS)
    {
        StringFmtSysErr(dwerr, szErrText, ARRAY_LEN(szErrText));
        FC_DebugPrint(_T("FC_ASSERT> bad return from system call")
                      _T(" at %hs line %d: dw=0x%x: %s  (Hint: '%s'\n"),
            pszFile, iLine, dwerr, szErrText, pszHint?pszHint:_T(""));
    }
}

FC_DBAPI void FC_DBDECL FC_AssertSysErr(
    const char*  pszFile, 
    int          iLine, 
    DWORD        dwerr,
    const TCHAR* pszHint
)
{
    if(dwerr==ERROR_SUCCESS)
        return;

    FC_PrintSysErr(pszFile, iLine, dwerr, pszHint);

#ifdef DEBUG
{
    TCHAR szAssertStr[1024];
    TCHAR szErrText[256];
    StringFmtSysErr(dwerr, szErrText, ARRAY_LEN(szErrText));

    wsprintf(szAssertStr, 
      _T("Bad return from system call:\n")
      _T("File '%hs' line %d\n")
      _T("dw=0x%x: %s\n")
      _T("Hint: '%s'"),
      pszFile, iLine, dwerr, szErrText, pszHint?pszHint:_T(""));

    if (IDCANCEL == MessageBox(NULL, szAssertStr, 
                _T("Assertion Failed!"),
         MB_OKCANCEL|MB_ICONEXCLAMATION|MB_SERVICE_NOTIFICATION))
    {
      __try{
        DebugBreak();
      }__except(UnhandledExceptionFilter(GetExceptionInformation()))
      {}
    }
}
#endif
}



/*
 *----------------------------------------------------------------------------*
 *  $History: fc_assert.c $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_todebug
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_todebug
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 5.01.01    Time: 10:10
 * Updated in $/4Control/COM/softing/fc/CMN/fc_todebug
 * renamed global defines FC_API -> FC_DBAPI
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 2.01.01    Time: 18:17
 * Created in $/4Control/COM/softing/fc/CMN/fc_todebug
 * taken and modified from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
