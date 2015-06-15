/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/trace/trace.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: trace.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/trace/trace.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           Trace
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
 *  15.03.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include "traceif.h"

//----  Local Defines:   -----------------------------------------------------*
// determine number of elements in an array (not bytes)
#define _countof(array) (sizeof(array)/sizeof(array[0]))

//----  Static Initializations:   --------------------------------------------*
#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef UNUSED_ALWAYS
#define UNUSED_ALWAYS(x) x
#endif

static BOOL             s_bInitialized = FALSE;
static enum TraceMode   s_traceMode = fileContTrace;
static WORD             s_wLevel    = TRC_FULL;
static UINT             s_uRingSize = 1000;
static UINT             s_uFileSize = 500;
static TCHAR*           s_pszTraceFile = NULL;
static TCHAR*           s_pszOldTrcFile = NULL;
static FILE*            s_hfTrc = NULL;
static WORD             s_wCurrDay = 0;

static const TCHAR      s_szExtOldTrace[] = _T(".otc");
static const int        s_nMaxDumpWidth = 16;   // max number of bytes in hex dump line

static CRITICAL_SECTION s_csTrcFile;
                            // exclusive access to trace file
static BOOL             s_bNewLine = TRUE;

//----  Local prototypes:          -------------------------------------------*
static void             printTrc(LPCTSTR pszLine);
static void             hexDump(BYTE* pby, int nBytes, int nShowBytes);
static LPCTSTR          GetTrcLevelString(WORD wLevel);
static LPCTSTR          GetDayOfWeek(WORD wDay);


//-------------------------- GLOBAL FUNCTION ---------------------------------*
int
    TrcInitialize(void)
/*>>>>
initialize trace module; mandatory first call!

Result
  E_TRC_OK
  E_TRC_NO_RESOURCE
<<<<*/
{
    if (s_bInitialized) {
        // already initialized!
        return E_TRC_OK;
    }
    if (s_hfTrc != NULL) {
        fclose(s_hfTrc);
        s_hfTrc = NULL;
    }
    if (s_pszTraceFile != NULL) {
        delete [] s_pszTraceFile;
        s_pszTraceFile = NULL;
    }
    if (s_pszOldTrcFile != NULL) {
        delete [] s_pszOldTrcFile;
        s_pszOldTrcFile = NULL;
    }

    InitializeCriticalSection(&s_csTrcFile);

    s_wCurrDay = 0;
    s_bInitialized = TRUE;
    s_bNewLine = TRUE;

    int ret = 0;
    ret = TrcSetMode(fileContTrace);
    assert(ret == E_TRC_OK);
    ret = TrcSetLevel(TRC_FULL);
    assert(ret == E_TRC_OK);
//    VERIFY(TrcSetRingSize(1000) == E_TRC_OK);
    ret = TrcSetFileSize(500);
    assert(ret == E_TRC_OK);
    ret = TrcSetFilename(_T("trace.log"));
    assert(ret == E_TRC_OK);
    
//    TrcPrint(TRC_WARNING, _T("**** Starting trace!!!!!!!!\n"));

    return E_TRC_OK;
}

//-------------------------- GLOBAL FUNCTION ---------------------------------*
int
    TrcTerminate(void)
/*>>>>
shutdown trace module

Result
  E_TRC_OK
  E_TRC_NOT_INITIALIZED
<<<<*/
{
//    TrcPrint(TRC_WARNING, _T("**** Closing trace!!!!!!!!\n"));
    if(!s_bInitialized) {
        return E_TRC_OK;
    }

    if (s_hfTrc != NULL) {
        fclose(s_hfTrc);
        s_hfTrc = NULL;
    }
    if (s_pszTraceFile != NULL) {
        delete [] s_pszTraceFile;
        s_pszTraceFile = NULL;
    }
    if (s_pszOldTrcFile != NULL) {
        delete [] s_pszOldTrcFile;
        s_pszOldTrcFile = NULL;
    }

    DeleteCriticalSection(&s_csTrcFile);
    s_bInitialized = FALSE;
    return E_TRC_OK;
}

//-------------------------- GLOBAL FUNCTION ---------------------------------*
int
    TrcSetMode(enum TraceMode newMode /*= fileContTrace*/)
/*>>>>
set trace mode

I   newMode:

Result
  E_TRC_OK
  E_TRC_NOT_INITIALIZED
  E_TRC_BAD_MODE
  E_TRC_NOT_IMPLEMENTED
<<<<*/
{
    if (!s_bInitialized) {
        return E_TRC_NOT_INITIALIZED;
    }

    int ret = E_TRC_OK;

    switch (newMode) {
    case noTrace:
    case fileContTrace:
        s_traceMode = newMode;
        ret = E_TRC_OK;
        break;
    case ringTrace:
    case fileStartupTrace:
    default:
        ret = E_TRC_NOT_IMPLEMENTED;
        break;
    }

    return ret;
}

//------------------------------------------------------------------*
/**
 * Returns actual trace mode.
 * @param			enum TraceMode& actualMode 
 * @return			int -E_TRC_OK: function call succeeded<br>
 *                      -E_TRC_NOT_INITIALIZED trace lib not initialized.
*/
int TrcGetMode(enum TraceMode& actualMode)
{
    if (!s_bInitialized) {
        actualMode = noTrace;
        return E_TRC_NOT_INITIALIZED;
    }

    actualMode = s_traceMode;
    return E_TRC_OK;
}


//-------------------------- GLOBAL FUNCTION ---------------------------------*
int
    TrcSetLevel(WORD wLevel /*= TRC_FULL*/)
/*>>>>
enable/disable trace levels

I   wLevel:     bitfield with enabled levels

Result
  E_TRC_OK
  E_TRC_NOT_INITIALIZED
  E_TRC_BAD_LEVEL
  E_TRC_NOT_IMPLEMENTED
<<<<*/
{
    if (!s_bInitialized) {
        return E_TRC_NOT_INITIALIZED;
    }

    /* EF better this way ? the user may define own levels and in the registry 
       you simple say 0xffff instead of 0x2ff all developers I do so...
    if (   ((wLevel & TRC_ALL) == 0)
       || ((wLevel & ~TRC_ALL) != 0))
    {
       return E_TRC_BAD_LEVEL;
    }
    */

    s_wLevel = wLevel;
    return E_TRC_OK;
}

//-------------------------- GLOBAL FUNCTION ---------------------------------*
int
    TrcSetRingSize(UINT uRingSize /*= 1000*/)
/*>>>>
set max. number of *entries* in ring                            

I   uRingSize::

Result
  E_TRC_OK
  E_TRC_NOT_INITIALIZED
  E_TRC_NO_RESOURCE
  E_TRC_NOT_IMPLEMENTED
<<<<*/
{
    if (!s_bInitialized) {
        return E_TRC_NOT_INITIALIZED;
    }

    s_uRingSize = uRingSize;
    return E_TRC_NOT_IMPLEMENTED;
}

//-------------------------- GLOBAL FUNCTION ---------------------------------*
int
    TrcSetFileSize(UINT uFileSize /*= 500*/)
/*>>>>
set max. size of trace file in KB 

I   newMode:

Result
  E_TRC_OK
  E_TRC_NOT_INITIALIZED
  E_TRC_NO_RESOURCE
  E_TRC_NOT_IMPLEMENTED
<<<<*/
{
    if (!s_bInitialized) {
        return E_TRC_NOT_INITIALIZED;
    }

    s_uFileSize = uFileSize;
    return E_TRC_OK;
}

//-------------------------- GLOBAL FUNCTION ---------------------------------*
int
    TrcFlushRingToFile(LPCTSTR pszFile)
/*>>>>
flush contents of ringtrace to file

I   newMode:

Result
  E_TRC_OK
  E_TRC_NOT_INITIALIZED
  E_TRC_NO_RESOURCE
  E_TRC_NOT_IMPLEMENTED
<<<<*/
{
    if (!s_bInitialized) {
        return E_TRC_NOT_INITIALIZED;
    }

    UNUSED_ALWAYS(pszFile);
    return E_TRC_NOT_IMPLEMENTED;
}

//-------------------------- GLOBAL FUNCTION ---------------------------------*
int
    TrcSetFilename(LPCTSTR pszFile)
/*>>>>
set basename of trace file

I   pszFile:    base path/name of trace file

Result
  E_TRC_OK
  E_TRC_NOT_INITIALIZED
  E_TRC_NO_RESOURCE
<<<<*/
{
    if (!s_bInitialized) {
        return E_TRC_NOT_INITIALIZED;
    }
    assert(pszFile != NULL);
    if (pszFile == NULL) {
        return E_TRC_NO_RESOURCE;
    }

    EnterCriticalSection(&s_csTrcFile);
    if (s_hfTrc != NULL) {
        fclose(s_hfTrc);
        s_hfTrc = NULL;
    }
    if (s_pszTraceFile != NULL) {
        delete [] s_pszTraceFile;
        s_pszTraceFile = NULL;
    }
    if (s_pszOldTrcFile != NULL) {
        delete [] s_pszOldTrcFile;
        s_pszOldTrcFile = NULL;
    }
    s_pszTraceFile = new TCHAR[_tcslen(pszFile) + sizeof(TCHAR)];
    assert(s_pszTraceFile != NULL);
    if (s_pszTraceFile == NULL) {
        LeaveCriticalSection(&s_csTrcFile);
        return E_TRC_NO_RESOURCE;
    }
    _tcscpy(s_pszTraceFile, pszFile);
    
    int iLen = _tcslen(s_pszTraceFile) + _tcslen(s_szExtOldTrace) + 2 * sizeof(TCHAR);
    s_pszOldTrcFile = new TCHAR[iLen];
    if (s_pszOldTrcFile == NULL) {
        LeaveCriticalSection(&s_csTrcFile);
        return E_TRC_NO_RESOURCE;
    }
    
    _tcscpy(s_pszOldTrcFile, s_pszTraceFile);
    LPTSTR   pszExt = _tcsrchr(s_pszOldTrcFile, '.');
    if (pszExt != NULL) {
        // replace original extension:
        *pszExt = _T('\0');
    }
    _tcscat(s_pszOldTrcFile, s_szExtOldTrace);
    LeaveCriticalSection(&s_csTrcFile);

    return E_TRC_OK;
}

//-------------------------- GLOBAL FUNCTION ---------------------------------*
int
    TrcPrint(WORD wLevel, LPCTSTR pszFormat, ...)
/*>>>>
print trace line with current time

I   wLevel:     trace level
I   pszFormat:  format string for wsprintf()

Result
  E_TRC_OK
  E_TRC_NOT_INITIALIZED
  E_TRC_BAD_LEVEL
  E_TRC_NO_RESOURCE
<<<<*/
{
    if (!s_bInitialized) {
        return E_TRC_NOT_INITIALIZED;
    }

    if (s_traceMode == noTrace) {
        // trace mode is disabled
        return E_TRC_OK;
    }

    if ((wLevel & s_wLevel) == 0) {
        // this level is not activated
        return E_TRC_OK;
    }

    if (pszFormat == NULL) {
        return E_TRC_NO_RESOURCE;
    }

    // print time and level...
    SYSTEMTIME  tm;
    GetLocalTime(&tm);

    TCHAR szBuffer[512];

    EnterCriticalSection(&s_csTrcFile);
    if (s_bNewLine)
    {
        // insert date and time if start of new line:
        if (s_wCurrDay != tm.wDay) {
            printTrc(_T("--------------------------------------------------------------\n"));
            _stprintf(szBuffer, _T("Current date: %s %04d-%02d-%02d\n"),
                      GetDayOfWeek(tm.wDayOfWeek),
                      (int) tm.wYear, (int) tm.wMonth, (int) tm.wDay);
            printTrc(szBuffer);
            s_wCurrDay = tm.wDay;
        }
        _stprintf(szBuffer, _T("%02d:%02d:%02d.%03d(%03d)|"),
                  (int) tm.wHour, (int) tm.wMinute,
                  (int) tm.wSecond, (int) tm.wMilliseconds, ::GetCurrentThreadId());
        printTrc(szBuffer);
        printTrc(GetTrcLevelString(wLevel));
        printTrc(_T("|"));
        s_bNewLine = FALSE;
    }
    // ... and actual message:
    va_list args;
    va_start(args, pszFormat);

    int nBuf;
    nBuf = _vstprintf(szBuffer, pszFormat, args);
    assert(nBuf < _countof(szBuffer));
    va_end(args);

    printTrc(szBuffer);
    int len = _tcslen(szBuffer);
    if (len > 0) {
        if (   (szBuffer[len - 1] == _T('\n'))
            || (szBuffer[len - 1] == _T('\r')))
        {
            s_bNewLine = TRUE;
        }
    }

    // check if max file size was reached:
    if ((s_hfTrc != NULL) && s_bNewLine && (s_uFileSize > 0)) {
        if ((UINT)(ftell(s_hfTrc) / 1000) > s_uFileSize) {
            fclose(s_hfTrc);
            _tremove(s_pszOldTrcFile);
            _trename(s_pszTraceFile, s_pszOldTrcFile);
            s_hfTrc = NULL;
            s_wCurrDay = 0; // force date line as first line in new file
        }
    }
    LeaveCriticalSection(&s_csTrcFile);
    return E_TRC_OK;
}


//----------------------------- FUNCTION -------------------------------------*
int
    TrcHexDump(WORD wLevel, BYTE* pby, int nBytes, int nShowBytes /*= -1*/)
/*>>>>
print hex dump of <pby>

I   wLevel:     trace level
I   pby:        ptr to byte array to dump
I   nBytes:     number of bytes in byte array
I   nShowBytes: show up to <nShowBytes> bytes;
                if -1, use default max length <s_nMaxDumpWidth>

Result
  E_TRC_OK
  E_TRC_NOT_INITIALIZED
  E_TRC_BAD_LEVEL
  E_TRC_NO_RESOURCE
<<<<*/
{
    if (!s_bInitialized) {
        return E_TRC_NOT_INITIALIZED;
    }

    if (s_traceMode == noTrace) {
        // trace mode is disabled
        return E_TRC_OK;
    }

    if ((wLevel & s_wLevel) == 0) {
        // this level is not activated
        return E_TRC_OK;
    }

    if (pby == NULL) {
        return E_TRC_NO_RESOURCE;
    }

    EnterCriticalSection(&s_csTrcFile);
    TrcPrint(wLevel, _T("HexDump: "));
    if (nShowBytes < 0) {
        nShowBytes = s_nMaxDumpWidth;
    }
    hexDump(pby, nBytes, nShowBytes);
    LeaveCriticalSection(&s_csTrcFile);
    return E_TRC_OK;
}

//----------------------------- FUNCTION -------------------------------------*
int
    TrcWin32Error(WORD wLevel, LPCTSTR pszText)
/*>>>>
trace Win32 system error messages

I   wLevel:     trace level
I   pszText     additional text to display

Result
  E_TRC_OK
  E_TRC_NOT_INITIALIZED
  E_TRC_BAD_LEVEL
  E_TRC_NO_RESOURCE
<<<<*/
{
    if (!s_bInitialized) {
        return E_TRC_NOT_INITIALIZED;
    }

    if (s_traceMode == noTrace) {
        // trace mode is disabled
        return E_TRC_OK;
    }

    if ((wLevel & s_wLevel) == 0) {
        // this level is not activated
        return E_TRC_OK;
    }

    TCHAR msg[MAX_PATH];
    EnterCriticalSection(&s_csTrcFile);
    DWORD   dwErr = ::GetLastError();
    // show error code...
    if (pszText == NULL) {
        pszText = _T("");
    }
    TrcPrint(wLevel, _T("%s Win32 Error: code = 0x%08lX\n"), pszText, dwErr);
    s_bNewLine = FALSE;
    if ((dwErr & 0x20000000) == 0) {
        // system error, get message text:
        msg[0] = _T('\0');
        ::FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL, dwErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
            msg, MAX_PATH, NULL); 
        TrcPrint(wLevel, _T("****  %s"), msg);
    }
    else {
        TrcPrint(wLevel, _T("****  Custom Error Code!!"));
    }
    if (!s_bNewLine) {
        TrcPrint(wLevel, _T("\n"));
    }
    LeaveCriticalSection(&s_csTrcFile);

    return E_TRC_OK;
}

//----------------------------- FUNCTION -------------------------------------*
/*static*/ void
    printTrc(LPCTSTR pszLine)
/*>>>>
print trace string to output device

I   pszLine:    line to output

Result
  void
<<<<*/
{
    assert(s_pszTraceFile != NULL);
    if (s_pszTraceFile == NULL) {
        return;
    }
    // check if open
    if (s_hfTrc == NULL) {
        s_hfTrc = _tfopen(s_pszTraceFile, _T("a+"));
        s_bNewLine = TRUE;
    }
    
    // now write if trace file is open:
    if (s_hfTrc != NULL) {
        _fputts(pszLine, s_hfTrc);
        fflush(s_hfTrc);
    }
}


//----------------------------- FUNCTION -------------------------------------*
/*static*/ void
    hexDump(BYTE* pby, int nBytes, int nShowBytes)
/*>>>>
print trace string to output device

I   pby:        ptr to byte array to dump
I   nBytes:     number of bytes in byte array
I   nShowBytes  show max. N bytes before truncating

Result
  void
<<<<*/
{
    int nRow = 0;
    TCHAR szBuffer[50];

    while (nBytes--)
    {
        if (nRow == 0)
        {
            wsprintf(szBuffer, _T("<%02X"), *pby++);
        }
        else {
            wsprintf(szBuffer, _T(" %02X"), *pby++);
        }
        printTrc(szBuffer);

        if (++nRow >= nShowBytes)
        {
            wsprintf(szBuffer, _T(" ...(trunc. by %d bytes)"), nBytes);
            printTrc(szBuffer);
//          nRow = 0;
            break;
        }
    }
    if (nRow != 0) {
        printTrc(_T(">\n"));
        s_bNewLine = TRUE;
    }
}

///////////////////////////////////////////////////////////////////////////////
//  Trace level texts:
static const TCHAR* const trcLevelTexts[] = {
    _T("FATAL "),           // TRC_FATAL            fatal error, highest trace level
    _T("ERROR "),           // TRC_ERROR            serious error: bad parameter etc.
    _T("WARN  "),           // TRC_WARNING          internal warning
    _T("BUSERR"),           // TRC_BUS_ERROR        error on communication bus
    _T("B_DIAG"),           // TRC_BUS_DIAGNOSE     diagnostic message on bus
    _T("USR_OP"),           // TRC_USER_OPERATION   log important user interaction/operation
    _T("IFACE "),           // TRC_INTERFACE        log action/callback on user interface
    _T("COMM  "),           // TRC_COMM_EVENT       log communication layer event
    _T("READ  "),           // TRC_READ_VALUE       log read operation with value
    _T("WRITE "),           // TRC_WRITE_VALUE      log write operation with value
    _T("<????>")
};

//----------------------------- FUNCTION -------------------------------------*
/*static*/ LPCTSTR
    GetTrcLevelString(WORD wLevel)
{
    int iLevel = -1;
    if (wLevel != 0) {
        while ((wLevel & 0x0001) == 0) {
            wLevel >>= 1;
            iLevel++;
        }
        iLevel++;
    }

    assert((iLevel >= 0) && (iLevel < _countof(trcLevelTexts)));
    if ((iLevel < 0) || (iLevel > _countof(trcLevelTexts))) {
        return _T("<BAD!>");
    }
    return trcLevelTexts[iLevel];
}

static const TCHAR* const dayOfWeekTexts[] = {
    _T("Sun"),
    _T("Mon"),
    _T("Tue"),
    _T("Wed"),
    _T("Thu"),
    _T("Fri"),
    _T("Sat")
};

//----------------------------- FUNCTION -------------------------------------*
/*static*/ LPCTSTR
    GetDayOfWeek(WORD wDay)
{

    assert((wDay >= 0) && (wDay < _countof(dayOfWeekTexts)));
    return dayOfWeekTexts[wDay];
}

/*
 *----------------------------------------------------------------------------*
 *  $History: trace.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/trace
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/trace
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 23.08.02   Time: 19:58
 * Updated in $/4Control/COM/softing/fc/CMN/trace
 * remove check  trace level, alllow this way custom levels
 * 
 * *****************  Version 3  *****************
 * User: Ed           Date: 6/06/01    Time: 11:25a
 * Updated in $/4Control/COM/softing/fc/CMN/trace
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 21.03.01   Time: 16:21
 * Updated in $/4Control/COM/softing/fc/CMN/trace
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 15.03.01   Time: 15:00
 * Created in $/4Control/COM/softing/fc/CMN/trace
 *==
 *----------------------------------------------------------------------------*
*/
