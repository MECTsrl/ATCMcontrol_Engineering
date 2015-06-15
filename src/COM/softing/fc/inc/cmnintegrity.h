/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/cmnintegrity.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: cmnintegrity.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/cmnintegrity.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                INC
 *
 * =COMPONENT           INC
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
 *== Utility functions to check memory integrity
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.02.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#ifndef __CMNINTEGRITY_H_
#define __CMNINTEGRITY_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "refobj.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

//------------------------------------------------------------------*
/**
 * @def DEFINE_INTEGRITY
 * Macros for memory integrity checks using _Crt* memory check
 * routines of the runtime library.
 *
 * <b>HOW TO USE</b><br>
 * ==========
 *
 * Define macro ENABLE_MEMORY_CHECK as desired in your stdafx.h or
 * some other global include file and include cmnintegrity.h.
 *
 * ENABLE_MEMORY_CHECK == 0     no memory checking
 * ENABLE_MEMORY_CHECK == 1     memory check on entry and on exit of
 *                              an integrity domain (see below)
 * ENABLE_MEMORY_CHECK == 2     memory check on entry and on exit of
 *                              an integrity domain and on memory
 *                              allocations and deallocations.
 *
 * Example:
 * --------
 *
 * #ifdef _DEBUG<br>
 * #  define ENABLE_MEMORY_CHECK 2<br>
 * #else<br>
 * #  define ENABLE_MEMORY_CHECK 0<br>
 * #endif<br>
 * #include "cmnintegrity.h"<br>
 *
 *
 * Write DEFINE_INTEGRITY at global declaration level of your
 * stdafx.cpp or some other cpp file of your project.
 *
 * Example:
 * --------
 *
 * DEFINE_INTEGRITY<br>
 *
 *
 * Write INIT_INTEGRITY in your modules global initialization
 *
 * Example:
 * --------
 *
 * CYourModuleApp::CYourModuleApp ()<br>
 * {<br>
 *     INIT_INTEGRITY<br>
 * }<br>
 *
 *
 * Each time you open a C++ compound statement (e.g. a method)
 * that you'd like to check for integrity write INTEGRITY_DOMAIN
 * at the compounds very beginning. Integrity is then checked
 * on entry and on exit of the compound statement. If you do not
 * write it at the beginning the integrity domain extends from
 * the point it is written to the end of the compound statement.
 *
 * Example:
 * --------
 *
 * void CYourModuleClass::DoSomething ()<br>
 * {<br>
 *     INTEGRITY_DOMAIN<br>
 *     .
 *     .
 *     .<br>
 * }<br>
 *
 *
 * Whenever you feel checking the integrity inside the checked
 * compound statement is useful you may write CHECK_INTEGRITY
 * everywhere inside a integrity domain where appearance of a
 * statement is syntactically correct.
 *
 * Example:
 * --------
 *
 * void CYourModuleClass::DoMore ()<br>
 * {<br>
 *     INTEGRITY_DOMAIN<br>
 *     .
 *     .
 *     .<br>
 *     CHECK_INTEGRITY<br>
 *     .
 *     .
 *     .<br>
 * }<br>
*/

#if ENABLE_MEMORY_CHECK == 0

/** @def INIT_INTEGRITY 
 *  Macros for memory integrity checks using _Crt* memory check
 *  routines of the runtime library.
 *  @see DEFINE_INTEGRITY
*/
#  define INIT_INTEGRITY 
#  define DEFINE_INTEGRITY

/** @def INTEGRITY_DOMAIN 
 *  Macros for memory integrity checks using _Crt* memory check
 *  routines of the runtime library.
 *  @see DEFINE_INTEGRITY
*/
#  define INTEGRITY_DOMAIN

/** @def CHECK_INTEGRITY 
 *  Macros for memory integrity checks using _Crt* memory check
 *  routines of the runtime library.
 *  @see DEFINE_INTEGRITY
*/
#  define CHECK_INTEGRITY
#elif ENABLE_MEMORY_CHECK == 1

/** @def INIT_INTEGRITY 
 *  Macros for memory integrity checks using _Crt* memory check
 *  routines of the runtime library.
 *  @see DEFINE_INTEGRITY
*/
#  define INIT_INTEGRITY {  CIntegrityDomain::sm_bCheckAll = false; \
                            CIntegrityDomain::uiMemoryIntegrityDomains = 0; \
                            CIntegrityDomain::iSavedMemoryCheckFlags = 0; \
                            CIntegrityDomian::iCurrentMemoryCheckFlags = 0;}
#  define DEFINE_INTEGRITY  bool CIntegrityDomain::sm_bCheckAll; \
                            CExclusiveSection CIntegrityDomain::m_Guard; \
                            unsigned int CIntegrityDomain::uiMemoryIntegrityDomains; \
                            int CIntegrityDomain::iSavedMemoryCheckFlags; \
                            int CIntegrityDomain::iCurrentMemoryCheckFlags;

/** @def INTEGRITY_DOMAIN 
 *  Macros for memory integrity checks using _Crt* memory check
 *  routines of the runtime library.
 *  @see DEFINE_INTEGRITY
*/
#  define INTEGRITY_DOMAIN CIntegrityDomain _ckintegrity_;

/** @def CHECK_INTEGRITY 
 *  Macros for memory integrity checks using _Crt* memory check
 *  routines of the runtime library.
 *  @see DEFINE_INTEGRITY
*/
#  define CHECK_INTEGRITY { CIntegrityDomain::Check (); }
#else

/** @def INIT_INTEGRITY 
 *  Macros for memory integrity checks using _Crt* memory check
 *  routines of the runtime library.
 *  @see DEFINE_INTEGRITY
*/
#  define INIT_INTEGRITY {  CIntegrityDomain::sm_bCheckAll = true; \
                            CIntegrityDomain::uiMemoryIntegrityDomains = 0; \
                            CIntegrityDomain::iSavedMemoryCheckFlags = 0; \
                            CIntegrityDomain::iCurrentMemoryCheckFlags = 0;}
#  define DEFINE_INTEGRITY  bool CIntegrityDomain::sm_bCheckAll; \
                            CExclusiveSection CIntegrityDomain::m_Guard; \
                            unsigned int CIntegrityDomain::uiMemoryIntegrityDomains; \
                            int CIntegrityDomain::iSavedMemoryCheckFlags; \
                            int CIntegrityDomain::iCurrentMemoryCheckFlags;

/** @def INTEGRITY_DOMAIN 
 *  Macros for memory integrity checks using _Crt* memory check
 *  routines of the runtime library.
 *  @see DEFINE_INTEGRITY
*/
#  define INTEGRITY_DOMAIN CIntegrityDomain _ckintegrity_;

/** @def CHECK_INTEGRITY 
 *  Macros for memory integrity checks using _Crt* memory check
 *  routines of the runtime library.
 *  @see DEFINE_INTEGRITY
*/
#  define CHECK_INTEGRITY { CIntegrityDomain::Check (); }
#endif


//------------------------------------------------------------------*
/**
 *  class CIntegrityDomain - used by macros DEFINE_INTEGRITY,INIT_INTEGRITY,
 *  INTEGRITY_DOMAIN,CHECK_INTEGRITY to perform integrity checks of memory.
 *  Uses the Crt* memory check routines of the runtime library. Private class -
 *  should only be used by the macros mentioned above.
 *
 *  @see    DEFINE_INTEGRITY,INIT_INTEGRITY,INTEGRITY_DOMAIN,CHECK_INTEGRITY
*/
class CIntegrityDomain
{
    static CExclusiveSection m_Guard;
public:
    CIntegrityDomain ()
    {
        BeginMemoryIntegrityDomain (sm_bCheckAll);
        CheckMemoryIntegrity ();
    }

    ~CIntegrityDomain ()
    {
        CheckMemoryIntegrity ();
        EndMemoryIntegrityDomain ();
    }

    static void Check ()
    {
        CheckMemoryIntegrity ();
    }

    static bool sm_bCheckAll;
    static unsigned int uiMemoryIntegrityDomains;
    static int iSavedMemoryCheckFlags;
    static int iCurrentMemoryCheckFlags;
private:
    inline void BeginMemoryIntegrityDomain (bool bCheckAll);
    inline void EndMemoryIntegrityDomain ();
    inline static void CheckMemoryIntegrity();
};


void CIntegrityDomain::BeginMemoryIntegrityDomain (bool bCheckAll)
{
    
    m_Guard.enter();

    if (uiMemoryIntegrityDomains == 0)
    {
        iSavedMemoryCheckFlags = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG);
    };
    ++uiMemoryIntegrityDomains;

    iCurrentMemoryCheckFlags |= bCheckAll ? _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF : _CRTDBG_ALLOC_MEM_DF;

    _CrtSetDbgFlag (iCurrentMemoryCheckFlags);

    m_Guard.leave();
}


void CIntegrityDomain::EndMemoryIntegrityDomain ()
{
    m_Guard.enter();

    if (--uiMemoryIntegrityDomains == 0)
    {
        iCurrentMemoryCheckFlags = iSavedMemoryCheckFlags;

        _CrtSetDbgFlag (iSavedMemoryCheckFlags);
    };

    m_Guard.leave();
}


void CIntegrityDomain::CheckMemoryIntegrity ()
{
    m_Guard.enter();
    _CrtCheckMemory ();
    m_Guard.leave();
}

/*
 *----------------------------------------------------------------------------*
 *  $History: cmnintegrity.h $
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
 * User: Su           Date: 7.02.01    Time: 11:14
 * Created in $/4Control/COM/softing/fc/inc
 * Initial rev. - Thanks to RF of TOF tool team for providing this.
 *==
 *----------------------------------------------------------------------------*
*/

#endif // __CMNINTEGRITY_H_







