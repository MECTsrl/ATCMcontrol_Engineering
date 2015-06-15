/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/trace/traceex.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: traceex.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/trace/traceex.cpp $
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
#include <atlbase.h>
#include <assert.h>
#include "traceif.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


HRESULT InitTrace(HMODULE hMod, LPCTSTR pszBasePath, LPCTSTR pszModuleId)
{
    TCHAR szTrcFile[_MAX_PATH + sizeof(TCHAR)];
    enum TraceMode mode = noTrace;
    WORD wLevel = TRC_FULL;
    UINT uFileSize = 500;   // 500 KB file size

    if(!pszBasePath) {
        assert(pszBasePath != NULL);
        return E_POINTER;
    }

    if(!pszModuleId) {
        assert(pszModuleId != NULL);
        return E_POINTER;
    }

    TrcInitialize();

    CRegKey keyApp;
    CRegKey keyTrace;

    _tcscpy(szTrcFile,pszBasePath);
    _tcscat(szTrcFile,pszModuleId);

    LONG lRes = keyApp.Create(HKEY_LOCAL_MACHINE, szTrcFile, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE);
    if (lRes == ERROR_SUCCESS)
    {
        lRes = keyTrace.Create(keyApp, _T("Trace"), REG_NONE, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE);
        if (lRes == ERROR_SUCCESS)
        {
            DWORD   dwMode = (DWORD) noTrace;
            keyTrace.QueryValue(dwMode, _T("Mode"));
            switch (dwMode) {
            case fileContTrace: // trace to file, swap to second file 
                mode = fileContTrace;
                break;
            case noTrace:       // trace is completely disabled
            case ringTrace:     // trace to ringbuffer in memory, no file trace
            case fileStartupTrace:  // trace to file until maxTraceSize is reached,
            default:
                mode = noTrace;
                break;
            }
            DWORD   dwLevel = TRC_FULL;
            keyTrace.QueryValue(dwLevel, _T("Level"));
            wLevel = (WORD) dwLevel;
            DWORD dwFileSize = uFileSize;
            keyTrace.QueryValue(dwFileSize, _T("MaxFileSize"));
            uFileSize = dwFileSize;
        }
    }
    else {
        TrcTerminate();
        return HRESULT_FROM_WIN32(lRes);
    }
    
    keyTrace.SetValue(uFileSize, _T("MaxFileSize"));
    keyTrace.SetValue((DWORD) mode, _T("Mode"));
    keyTrace.SetValue((DWORD) wLevel, _T("Level"));
    keyTrace.SetValue(_T("0: no trace\n")
                      _T("3: continuous trace with file swapping\n"),
                      _T("ModeComment"));
    keyTrace.SetValue(
            _T("0x0001: TRC_FATAL = fatal error, highest trace level\n")
            _T("0x0002: TRC_ERROR = serious error\n")
            _T("0x0004: TRC_WARNING = internal warning\n")
            _T("0x0008: TRC_BUS_ERROR = error on communication bus\n")
            _T("0x0010: TRC_BUS_DIAGNOSE = diagnostic message on bus\n")
            _T("0x0020: TRC_USER_OPERATION = log important user interaction/operation\n")
            _T("0x0040: TRC_INTERFACE = log action/callback on user interface\n")
            _T("0x0080: TRC_COMM_EVENT = log communication layer event\n")
            _T("0x0100: TRC_READ_VALUE = log read operation with value\n")
            _T("0x0200: TRC_WRITE_VALUE = log write operation with value\n"),
            _T("LevelComment"));

    // handle case that user turned off tracing
    if (mode == noTrace)
    {
        //TrcPrint(TRC_WARNING, _T("**** Trace is turned off!\n"));
        TrcTerminate();
        return S_OK;
    }
    
    ::TrcSetMode(mode);
    ::TrcSetLevel(wLevel);
    ::TrcSetFileSize(uFileSize);
    
    ::GetModuleFileName(hMod, szTrcFile, _MAX_PATH);
    LPTSTR  psz = _tcsrchr(szTrcFile, _T('\\'));
    if (psz != NULL) {
        *psz = _T('\0');
    }

    _tcscat(szTrcFile, _T("\\"));
    _tcscat(szTrcFile,pszModuleId);
    _tcscat(szTrcFile,_T(".trc"));

    ::TrcSetFilename(szTrcFile);
    TrcPrint(TRC_WARNING, _T("**** Starting trace with TrcLevel 0x%04x !\n"), wLevel);
    return S_OK;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: traceex.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/trace
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/trace
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 21.03.01   Time: 16:22
 * Updated in $/4Control/COM/softing/fc/CMN/trace
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 19.03.01   Time: 10:04
 * Updated in $/4Control/COM/softing/fc/CMN/trace
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 15.03.01   Time: 15:00
 * Created in $/4Control/COM/softing/fc/CMN/trace
 *==
 *----------------------------------------------------------------------------*
*/
