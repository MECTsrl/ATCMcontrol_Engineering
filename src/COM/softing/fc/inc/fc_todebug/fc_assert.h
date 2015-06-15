/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_todebug/fc_assert.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_assert.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_todebug/fc_assert.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                fc
 *
 * =COMPONENT           fc_todebug
 *
 * =CURRENT             $Date: 28.02.07 19:01 $
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
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  02.01.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/

#ifndef FC_ASSERT_H_
#define FC_ASSERT_H_

#include <tchar.h>


#ifndef FC_DBAPI
#define FC_DBAPI __declspec(dllimport)
#endif
#define FC_DBDECL _stdcall


#ifdef __cplusplus
extern "C" {
#endif


//------------------------------------------------------------------*
/**
 * FC_DebugPrint(A|W) writes the formatted text into the debug output
 * file and also prints it with Win32 OutputDebugstring.
 *                  
 *
 * @param           lpszFormat  format string, same as for wsprintf
 * @return          void
 * @exception       -
 * @see             Example debug.ini how to customise the output file(s)
*/
FC_DBAPI void FC_DBDECL FC_DebugPrintA(const char*  lpszFormat, ...);
FC_DBAPI void FC_DBDECL FC_DebugPrintW(const wchar_t* lpszFormat, ...);
#ifdef UNICODE
 #define FC_DebugPrint FC_DebugPrintW
#else
 #define FC_DebugPrint FC_DebugPrintA
#endif


// assertions are never UNICODE
//------------------------------------------------------------------*
/**
 * FC_Assert displayes an assertion messagebox and 
 * also prints the assertion with Win32 OutputDebugstring and
 * writes the assertion into the debug log.
 *
 * @param           lpFile   from __FILE__
 * @param           Line     from __LINE__
 * @param           lpString text to be displayed
 * @return          void
 * @exception       -
*/
FC_DBAPI void FC_DBDECL FC_Assert(const char* lpFile, int Line, const char* lpString);

//------------------------------------------------------------------*
/**
 * FC_PrintGetLastError if GetLastError() is !=0 write the last error 
 * with FC_DebugPrint.
 *
 * @param           lpFile   from __FILE__
 * @param           Line     from __LINE__
 * @return          void
 * @exception       -
*/
FC_DBAPI void FC_DBDECL FC_PrintGetLastError(const char* lpszFile, int Line);

//------------------------------------------------------------------*
/**
 * FC_AssertGetLastError if GetLastError() is !=0 write the last error 
 * with FC_DebugPrint and also call FC_Assert it.
 *
 * @param           lpFile   from __FILE__
 * @param           Line     from __LINE__
 * @return          void
 * @exception       -
*/
FC_DBAPI void FC_DBDECL FC_AssertGetLastError(
    const char*   pszFile, 
    int           iLine,
    const TCHAR*  pszHint
);

//------------------------------------------------------------------*
/**
 * FC_AssertSysErr same as FC_AssertGetLastError but dwerr is a parameter
 *
 * @param           lpFile   from __FILE__
 * @param           Line     from __LINE__
 * @param           dwerr    from GetLastError(), no output if dwerr==ERROR_SUCCESS
 * @return          void
 * @exception       -
*/
FC_DBAPI void FC_DBDECL FC_AssertSysErr(
    const char*    pszFile, 
    int            iLine, 
    unsigned long  dwerr,
    const TCHAR*   pszHint
);

//------------------------------------------------------------------*
/**
 * FC_PrintSysErr same as FC_PrintGetLastError but dwerr is a parameter
 *
 * @param           lpFile   from __FILE__
 * @param           Line     from __LINE__
 * @param           dwerr    from GetLastError(), no output if dwerr==ERROR_SUCCESS
 * @param           pszHint  additional text as a debug hint, may be NULL
 * @return          void
 * @exception       -
*/
FC_DBAPI void FC_DBDECL FC_PrintSysErr(
    const char*    pszFile, 
    int            iLine, 
    unsigned long  dwerr,
    const TCHAR*   pszHint
);

#ifdef __cplusplus
}
#endif

//assertions are on even if no debug, but no message box appears
//only OutputDebugString
#ifdef FC__FILE__
 #define   assert(exp) if(exp); else FC_Assert(FC__FILE__,__LINE__,#exp)
 #define fcassert(exp) if(exp); else FC_Assert(FC__FILE__,__LINE__,#exp)
#else
 #define   assert(exp) if(exp); else FC_Assert(__FILE__,__LINE__,#exp)
 #define fcassert(exp) if(exp); else FC_Assert(__FILE__,__LINE__,#exp)
#endif

#define FC_ASSERT_SYSERR(dw, pszHint) if(dw) FC_AssertSysErr(__FILE__,__LINE__,dw,pszHint)
#define FC_ASSERT_LASTERR(pszHint)    FC_AssertGetLastError(__FILE__,__LINE__,pszHint)

#ifdef DEBUG
  /* debug macros ------------------------------------------------------------*/
  #define FC_PRINTERROR() FC_PrintGetLastError(__FILE__, __LINE__)
  #define FC_DEBUGPRINT(lpString)     FC_DebugPrint(lpString)
  #define FC_DEBUGPRINT1(lpString,p1)     FC_DebugPrint(lpString,p1)
  #define FC_DEBUGPRINT2(lpString,p1,p2)     FC_DebugPrint(lpString,p1,p2)
  #define FC_DEBUGPRINT3(lpString,p1,p2,p3)     FC_DebugPrint(lpString,p1,p2,p3)
  #define FC_DEBUGPRINT4(lpString,p1,p2,p3,p4)     FC_DebugPrint(lpString,p1,p2,p3,p4)
  #define FC_DEBUGPRINT5(lpString,p1,p2,p3,p4,p5)     FC_DebugPrint(lpString,p1,p2,p3,p4,p5)
  #define FC_DEBUGPRINT6(lpString,p1,p2,p3,p4,p5,p6)     FC_DebugPrint(lpString,p1,p2,p3,p4,p5,p6)
  #define FC_DEBUGPRINT7(lpString,p1,p2,p3,p4,p5,p6,p7)     FC_DebugPrint(lpString,p1,p2,p3,p4,p5,p6,p7)
  #define FC_DEBUGPRINT8(lpString,p1,p2,p3,p4,p5,p6,p7,p8)     FC_DebugPrint(lpString,p1,p2,p3,p4,p5,p6,p7,p8)
  #define FC_DEBUGPRINT9(lpString,p1,p2,p3,p4,p5,p6,p7,p8,p9)     FC_DebugPrint(lpString,p1,p2,p3,p4,p5,p6,p7,p8,p9)
  #define FC_DEBUGPRINT10(lpString,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10)     FC_DebugPrint(lpString,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10)
#else
  /* no debug macros ---------------------------------------------------------*/
  #define FC_PRINTERROR()
  #define FC_DEBUGPRINT(lpString)
  #define FC_DEBUGPRINT1(lpString,p1)
  #define FC_DEBUGPRINT2(lpString,p1,p2)
  #define FC_DEBUGPRINT3(lpString,p1,p2,p3)
  #define FC_DEBUGPRINT4(lpString,p1,p2,p3,p4)
  #define FC_DEBUGPRINT5(lpString,p1,p2,p3,p4,p5)
  #define FC_DEBUGPRINT6(lpString,p1,p2,p3,p4,p5,p6)
  #define FC_DEBUGPRINT7(lpString,p1,p2,p3,p4,p5,p6,p7)
  #define FC_DEBUGPRINT8(lpString,p1,p2,p3,p4,p5,p6,p7,p8)
  #define FC_DEBUGPRINT9(lpString,p1,p2,p3,p4,p5,p6,p7,p8,p9)
  #define FC_DEBUGPRINT10(lpString,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10)
#endif


#endif //FC_ASSERT_H_
/*
 *----------------------------------------------------------------------------*
 *  $History: fc_assert.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc/fc_todebug
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc/fc_todebug
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 5.01.01    Time: 10:09
 * Updated in $/4Control/COM/softing/fc/inc/fc_todebug
 * renamed global defines FC_API -> FC_DBAPI
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 3.01.01    Time: 11:04
 * Updated in $/4Control/COM/softing/fc/inc/fc_todebug
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 2.01.01    Time: 18:15
 * Created in $/4Control/COM/softing/fc/inc/fc_todebug
 * taken and modified from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
