/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fcpump.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fcpump.h $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/fcpump.h $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           UTIL
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *           $Revision: 1 $
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
 *  14.12.2000  SIS      File copied from V1.3
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __FCPUMP_H_
#define __FCPUMP_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// ATTENTION ATTENTION ATTENTION
//
// This class has to be part of a DLL. You must not move it to a library.
// Reason: we declare a static map that maps from HWNDs to CFCPump instances
// and a static window procedure that has access to its corresponding map.
// If you move this class to a library, each module using this library in-
// stantiates its own window procedure/map pair. However only one of these
// window procedures will be called by Windows (usually the first registered
// wins). The other map's entries cannot be mapped in this window procedure
// (it does not even know about this map) and CFCPump just ignores messages
// for HWNDs in this map.

//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


#ifndef __FCPUMP_H
#define __FCPUMP_H

#if	defined	(UTIL_IMP)
#define	UTILIF __declspec(dllexport)
#else
#define	UTILIF __declspec(dllimport)
#endif

#include "windef.h"
#include "fccallback.h"

FC_CALLBACK_DEF (FCPumpCB)


#define FCPUMPWINDOWCLASS _T ("4C Pump Window")
#define WM_FCPUMP WM_USER


class UTILIF CFCPump
{
public:
    CFCPump ();
    ~CFCPump ();

    void RegisterCB (FC_CALLBACK_BASE (FCPumpCB) *pPumpCB);
    void UnregisterCB (FC_CALLBACK_BASE (FCPumpCB) *pPumpCB);

    HRESULT Create (LPCTSTR pszName);
    void Destroy ();

    HRESULT Trigger ();

    HRESULT Delay (UINT uiDelay, UINT &uiCookie);
    UINT GetDelayCookie ();

protected:
    LPCTSTR                       m_pszName;
    HWND                          m_hWnd;
    FC_CALLBACK_HOLDER (FCPumpCB) m_PumpCBs;
    bool                          m_bTriggered;
    UINT                          m_uiDelayCookie;
    UINT                          m_uiNextDelayCookie;

    virtual void OnPump ();
    virtual void OnTimer (UINT uiCookie);

private:
    static bool m_bClassRegistered;
    static CMap<HWND, HWND, CFCPump *, CFCPump *> m_PumpByHWND;
    static HRESULT RegisterPumpWindowClass ();
    static LRESULT CALLBACK WindowProc (HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam);
};

#endif

#endif // __FCPUMP_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: fcpump.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 2  *****************
 * User: Rf           Date: 17.04.01   Time: 16:02
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Sis          Date: 14.12.00   Time: 16:53
 * Created in $/4Control/COM/softing/fc/inc
 * Copied from V1.3 and replaced header
 *==
 *----------------------------------------------------------------------------*
*/
