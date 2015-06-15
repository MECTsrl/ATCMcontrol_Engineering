/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/traceif.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: traceif.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/traceif.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           Trace
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
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
#ifndef __TRACEIF_H_
#define __TRACEIF_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   ------------------------------------------------*

//----  Forward Class Definitions:   -----------------------------------------*

//----  Defines:    ----------------------------------------------------------*
//********      Trace levels:
#define TRC_FATAL               0x0001      // fatal error, highest trace level
#define TRC_ERROR               0x0002      // serious error: bad parameter etc.
#define TRC_WARNING             0x0004      // internal warning
#define TRC_BUS_ERROR           0x0008      // error on communication bus
#define TRC_BUS_DIAGNOSE        0x0010      // diagnostic message on bus
#define TRC_USER_OPERATION      0x0020      // log important user interaction/operation
#define TRC_INTERFACE           0x0040      // log action/callback on user interface
#define TRC_COMM_EVENT          0x0080      // log communication layer event
#define TRC_READ_VALUE          0x0100      // log read operation with value
#define TRC_WRITE_VALUE         0x0200      // log write operation with value

// some commonly used level combinations:
#define TRC_BASIC               (TRC_FATAL | TRC_ERROR     | TRC_WARNING)
#define TRC_COMMUNICATION       (TRC_BASIC | TRC_BUS_ERROR | TRC_BUS_DIAGNOSE | TRC_COMM_EVENT)
#define TRC_FULL                (TRC_COMMUNICATION | TRC_USER_OPERATION | TRC_INTERFACE)
#define TRC_ALL                 (TRC_FULL | TRC_READ_VALUE | TRC_WRITE_VALUE)

//********      Trace error codes:
#define E_TRC_OK                0           // no error, last call was ok
#define E_TRC_NOT_INITIALIZED   1           // trace is not initialized
#define E_TRC_BAD_MODE          2           // unknown/bad trace mode
#define E_TRC_BAD_LEVEL         3           // unknown/bad trace level
#define E_TRC_NO_RESOURCE       4           // last call requested too many resources
#define E_TRC_NOT_IMPLEMENTED   5           // function not implemented

//----  Typedefs:   ----------------------------------------------------------*
enum TraceMode {
    noTrace = 0,        // trace is completely disabled
    ringTrace,          // trace to ringbuffer in memory, no file trace
    fileStartupTrace,   // trace to file until maxTraceSize is reached,
                        //  old trace file is deleted
    fileContTrace,      // trace to file, swap to second file 
                        //  if maxTraceSize is reached
};

//----  Globals:    ----------------------------------------------------------*

//----  Prototypes:    -------------------------------------------------------*
int                     TrcInitialize(void);
                            // initialize trace module; mandatory first call!
int                     TrcTerminate(void);
                            // shutdown trace module
int                     TrcSetMode(enum TraceMode newMode);
                            // set trace mode
int                     TrcGetMode(enum TraceMode& actualMode);
                            // get actual trace mode.
int                     TrcSetLevel(WORD wLevel);
                            // enable/disable trace levels
int                     TrcSetRingSize(UINT uRingSize);
                            // set max. number of *entries* in ring                            
int                     TrcSetFileSize(UINT uFileSize);
                            // set max. size of trace file in KB 
int                     TrcSetFilename(LPCTSTR pszFile);
                            // set basename of trace file
int                     TrcFlushRingToFile(LPCTSTR pszFile);
                            // flush contents of ringtrace to file

int                     TrcPrint(WORD wLevel, LPCTSTR lpszFormat, ...);
                            // print trace message to output device
int                     TrcHexDump(WORD wLevel, BYTE* pby, int nBytes, int nShowBytes = -1);
                            // print hex dump of <pby>
int                     TrcWin32Error(WORD wLevel, LPCTSTR pszText);
                            // trace Win32 system error messages

//------------------------------------------------------------------*
/**
 * DefaultInitTrace initializes tracing for the calling module. This
 * function models default behaviour which can be overwritten by 
 * the module. Copy the code of InitTrace and adapt it in 
 * your module as desired 
 * @param hMod - Module handle of the current module.
 * @param pszBasePath - Base path in the registry for registration of the 
 *                      trace settings.
 * @param pszModuleId - Module identifier
 * @return hr = E_POINTER, if pszBasePath or pszModuleId == NULL.<br>
 *  hr = S_OK for success.
 * @see             
*/
HRESULT  InitTrace(HMODULE hMod, LPCTSTR pszBasePath, LPCTSTR pszModuleId);

/////////////////////////////////////////////////////////////////////////////
#endif // __TRACEIF_H_

/*
 *----------------------------------------------------------------------------*
 *  $History: traceif.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 21.03.01   Time: 16:20
 * Created in $/4Control/COM/softing/fc/inc
 * Renamed from trace.h to traceif.h to avoid conflicts with standard
 * headers
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 15.03.01   Time: 15:00
 * Created in $/4Control/COM/softing/fc/inc
 *==
 *----------------------------------------------------------------------------*
*/
