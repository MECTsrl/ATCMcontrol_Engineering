/////////////////////////////////////////////////////////////////////////////
// TrayIcon.h : Declaration of SECTrayIcon
//
// Stingray Software Extension Classes
// Copyright (C) 1997 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//

#ifndef __SEC_TRAYICON_H
#define __SEC_TRAYICON_H

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32

//
// SEC Extension DLL
// Initialize declaration context
//
#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL

// Default Tray Icon notification message
#define WM_SEC_TRAYICON_NOTIFY	WM_USER+1025


//@doc SECTrayIcon
//@mfunc UINT | SECTrayIcon | GetNotifyID | Returns the current notification ID for this object.
//@rdesc The notification ID.
//@xref<c SECTrayIcon>


//@doc SECTrayIcon
//@mfunc BOOL | SECTrayIcon | IsPlaying | Call this function to query if animation is in progress. 
//@rdesc Returns TRUE if animation is in progress; FALSE otherwise.
//@xref<c SECTrayIcon>

	

//@doc SECTrayIcon 
//@class Windows 95-style System Tray icon functionality. The SECTrayIcon class provides your application with an easy to use mechanism
// to add your own custom icons to the system tray, providing neat UI feedback such as tooltip text, context menu support,
// and animated icons.
//@comm The TRAYICON sample in the \SAMPLES\TOOLKIT\STANDARD\UIEXT\TRAYICON directory demonstrates the SECTrayIcon class.
//@BASE public|CObject
class SECTrayIcon : public CObject
{

	DECLARE_DYNAMIC( SECTrayIcon )

public:
    // construction/destruction
    SECTrayIcon();
    virtual ~SECTrayIcon();

	//@access Creation/Destruction

	//@cmember
	/* Create the tray icon.*/
    virtual void Create(CWnd* pwnd, UINT nNotifyID,
						UINT nMsg=WM_SEC_TRAYICON_NOTIFY, 
						HINSTANCE hinst = ::AfxGetResourceHandle());

	//@cmember
	/* Create the tray icon.*/
    virtual void Create(CWnd* pwnd, 
						UINT nMsg=WM_SEC_TRAYICON_NOTIFY, 
						HINSTANCE hinst = ::AfxGetResourceHandle());

	//@cmember
	/* Destroy the tray icon.*/
    virtual void Destroy();

	//@access Attributes

	//@cmember
	/* Set the tooltip text.*/
    virtual void SetTip(const CString& sTip);

	//@cmember
	/* Set the associated tray icon.*/
    virtual void SetIcon(UINT nIconID);

	//@cmember
	/* Set the associated tray icon.*/
    virtual void SetIcon(LPCTSTR szIconID);

	//@cmember
	/* Set the associated tray icon.*/
    virtual void SetIcon(HICON hIcon);

	//@cmember
	/* Set the current tray icon state.*/
    virtual void  SetState(DWORD nState);

	//@cmember
	/* Get the current tray icon state.*/
    virtual DWORD GetState();

    //@cmember
    /* Add a tray icon state.*/
    virtual void AddState(DWORD nState, UINT nIconID, const CString& sTip, UINT nJiffy = 15);

    //@cmember
    /* Add a tray icon state.*/
    virtual void AddState(DWORD nState, LPCTSTR szIconID, const CString& sTip, UINT nJiffy = 15);

    //@cmember
    /* Add a tray icon state.*/
    virtual void AddState(DWORD nState, HICON hIcon, const CString& sTip, UINT nJiffy = 15);

    //@cmember
    /* Remove a tray icon state.*/
    virtual void RemoveState(DWORD nState);

	//@access Operations

	//@cmember
	/* Show/hide the tray icon.*/
    virtual void Show(BOOL bShow = TRUE);

    //@cmember
    /* Play an icon animation.*/
    virtual void Play(DWORD nBaseState, UINT nStates);

	//@cmember
	/* Stop an icon animation.*/
    virtual void Stop();

	//@cmember
	/* Queries whether animation is in progress.*/
	BOOL IsPlaying() { return (BOOL)m_nTimerID; }

    //@cmember
    /* Show tray icon context menu.*/
    static void ShowContextMenu(CWnd* pwnd, UINT nMenuID, UINT nSubMenu = 0);

    //@cmember
    /* Show tray icon context menu.*/
    static void ShowContextMenu(CWnd* pwnd, LPCTSTR szMenuID, UINT nSubMenu = 0);

	// Default notification id recall

	//@cmember
	/* Get next available id for tray icon notification.*/
	static UINT GetNextNotifyID();

	//@cmember
	/* Get the latest allocated nofitication id.*/
	static UINT GetLatestNotifyID();

	
	//@cmember
	/* Get the objects' current notification ID.*/
	UINT GetNotifyID() 	{ return m_nNotifyID; }

protected:
    static BOOL s_bAnimWndClassRegistered;
	static UINT s_nNextNotifyID;

    UINT        m_nNotifyID;
    HWND        m_hwnd;
    UINT        m_nMsg;
    DWORD       m_nCurrentState;
    BOOL        m_bShowing;
    HINSTANCE   m_hinst;
    UINT        m_nTimerID;
    HWND        m_hwndTimer;
    DWORD       m_nAnimBaseState;
    UINT        m_nAnimStates;
    UINT        m_nAnimOffset;

    struct STATEDATA
    {
        HICON   hicon;
        CString sTip;
        UINT    nJiffy;

        STATEDATA(HICON hicon, const CString& sTip, UINT nJiffy)
            : hicon(hicon), sTip(sTip), nJiffy(nJiffy) {}
    };

    CMapPtrToPtr    m_mapStateData;
    BOOL    GetStateData(DWORD nState, HICON* phicon, CString* psTip, UINT* pnJiffy);
    void    SetStateData(DWORD nState, HICON hicon, const CString& sTip, UINT nJiffy);

    void    OnTimer();

    // Shell_NotifyIcon() wrapper
    BOOL    ShellNotifyIcon(DWORD nMsg);

    friend LRESULT CALLBACK AnimWndProc(HWND, UINT, WPARAM, LPARAM);
};


//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif	// WIN32

#endif  // __SEC_TRAYICON_H

