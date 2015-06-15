/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/fcpump.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fcpump.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/fcpump.cpp $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           UTIL
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
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




//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*



#include "stdafx.h"
#define UTIL_IMP
#include "fcpump.h"


bool CFCPump::m_bClassRegistered;
CMap<HWND, HWND, CFCPump *, CFCPump *> CFCPump::m_PumpByHWND;


CFCPump::CFCPump ()
: m_hWnd (NULL),
  m_bTriggered (false),
  m_uiNextDelayCookie (1),
  m_uiDelayCookie (0)
{
}


CFCPump::~CFCPump ()
{
    Destroy ();
}


void CFCPump::RegisterCB (FC_CALLBACK_BASE (FCPumpCB) *pPumpCB)
{
    POSITION pPosition;

    pPosition = m_PumpCBs.Find (pPumpCB);
    ASSERT (pPosition == NULL);
    if (pPosition != NULL)
    {
        return;
    };

    m_PumpCBs.AddTail (pPumpCB);
}


void CFCPump::UnregisterCB (FC_CALLBACK_BASE (FCPumpCB) *pPumpCB)
{
    POSITION pPosition;

    pPosition = m_PumpCBs.Find (pPumpCB);
    ASSERT (pPosition != NULL);
    if (pPosition == NULL)
    {
        return;
    };

    m_PumpCBs.RemoveAt (pPosition);
}


HRESULT CFCPump::Create (LPCTSTR pszName)
{
    HRESULT hr;

    m_pszName = _tcsdup (pszName);
    if (m_pszName == NULL)
    {
        return (E_OUTOFMEMORY);
    };

    hr = RegisterPumpWindowClass ();
    if (FAILED (hr))
    {
        return (hr);
    };

    m_hWnd = ::CreateWindow (FCPUMPWINDOWCLASS, "pump window", WS_DISABLED,
                             -1, -1, 0, 0, NULL, NULL, ::GetModuleHandle (NULL), NULL);

    if (m_hWnd == NULL)
    {
        return (HRESULT_FROM_WIN32 (::GetLastError ()));
    };

    m_PumpByHWND.SetAt (m_hWnd, this);

    return (S_OK);
}


void CFCPump::Destroy ()
{
    if (m_hWnd != NULL)
    {
        m_PumpByHWND.RemoveKey (m_hWnd);

        ::DestroyWindow (m_hWnd);
        m_hWnd = NULL;
    };

    if (m_pszName != NULL)
    {
        free ((void *)m_pszName);
        m_pszName = NULL;
    };
}


HRESULT CFCPump::Trigger ()
{
    if (m_hWnd == NULL)
    {
        return (E_FAIL);
    };

    if (!m_bTriggered)
    {
        m_bTriggered = true;
        ::PostMessage (m_hWnd, WM_FCPUMP, 0, 0);
    };

    return (S_OK);
}


void CFCPump::OnPump ()
{
    m_bTriggered = false;
    m_PumpCBs.Execute ();
}


HRESULT CFCPump::Delay (UINT uiDelay, UINT &uiCookie)
{
    if (m_hWnd == NULL)
    {
        return (E_FAIL);
    };

    uiCookie = ::SetTimer (m_hWnd, m_uiNextDelayCookie++, uiDelay, NULL);

    return (S_OK);
}


void CFCPump::OnTimer (UINT uiCookie)
{
    UINT uiSavedCookie=m_uiDelayCookie;

    if (m_hWnd != NULL)
    {
        ::KillTimer (m_hWnd, uiCookie);
    };

    m_uiDelayCookie = uiCookie;
    m_PumpCBs.Execute ();
    m_uiDelayCookie = uiSavedCookie;
}


UINT CFCPump::GetDelayCookie ()
{
    return (m_uiDelayCookie);
}


HRESULT CFCPump::RegisterPumpWindowClass ()
{
    WNDCLASS wndclass;
    ATOM wAtom;
    DWORD dwError;

    if (m_bClassRegistered)
    {
        return (S_OK);
    };

    wndclass.style = 0;
    wndclass.lpfnWndProc = CFCPump::WindowProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = ::GetModuleHandle (NULL);
    wndclass.hIcon = NULL;
    wndclass.hCursor = NULL;
    wndclass.hbrBackground = NULL;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = FCPUMPWINDOWCLASS;

    wAtom = RegisterClass (&wndclass);
    if (wAtom != 0)
    {
        m_bClassRegistered = true;
        return (S_OK);
    };

    dwError = ::GetLastError ();
    if (dwError == ERROR_CLASS_ALREADY_EXISTS)
    {
        m_bClassRegistered = true;
        return (S_OK);
    };

    return (HRESULT_FROM_WIN32 (dwError));
}


LRESULT CALLBACK CFCPump::WindowProc (HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam)
{
    CFCPump *pPump;

    if (!m_PumpByHWND.Lookup (hWnd, pPump))
        return (::DefWindowProc (hWnd, uiMessage, wParam, lParam));

    if (uiMessage == WM_FCPUMP)
    {
        pPump->OnPump ();
        return (0);
    };

    if (uiMessage == WM_TIMER)
    {
        pPump->OnTimer (wParam);
        return (0);
    };

    return (::DefWindowProc (hWnd, uiMessage, wParam, lParam));
}


/*
 *----------------------------------------------------------------------------*
 *  $History: fcpump.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 2  *****************
 * User: Rf           Date: 17.04.01   Time: 16:02
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 1  *****************
 * User: Rf           Date: 17.04.01   Time: 15:28
 * Created in $/4Control/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 5.04.01    Time: 19:37
 * Updated in $/4Control/COM/softing/fc/CE/CEUTIL
 * 
 * *****************  Version 1  *****************
 * User: Sis          Date: 22.01.01   Time: 15:30
 * Created in $/4Control/COM/softing/fc/CE/CEUTIL
 * 
 *----------------------------------------------------------------------------*
*/
