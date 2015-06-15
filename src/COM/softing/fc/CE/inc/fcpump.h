#ifndef __FCPUMP_H_
#define __FCPUMP_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


#ifndef __FCPUMP_H
#define __FCPUMP_H

#include "windef.h"
#include "fccallback.h"

FC_CALLBACK_DEF (FCPumpCB)


#define FCPUMPWINDOWCLASS _T ("ATCM Pump Window")
#define WM_FCPUMP WM_USER


class CFCPump
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
