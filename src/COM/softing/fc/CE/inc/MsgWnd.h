
#ifndef __MSGWND_H_
#define __MSGWND_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

#include "MsgDoc.h"
#include "MsgFrame.h"
#include "MsgThrd.h"
#include "fccallback.h"
#include "CEStateMan.h"
#include "TargetOnl.h"
#include "SizedDialogBar.h"

// local defines
#define MSGWND_FRAME  7   // frame size for message framewindow

#define CEMSG_ALARMCOLOR    ( RGB(255,0,0) )
#define CEMSG_STANDARDCOLOR ( RGB(0,0,0) )


// MsgWnd.h : header file
//

class CMsgRegex;
class CMsgData;
class CImplMsgSink;

/////////////////////////////////////////////////////////////////////////////
// CMsgWnd window


class CMsgWnd : public CSizedDialogBar
{
// Construction
public:
    CMsgWnd();
    DECLARE_DYNCREATE(CMsgWnd)

// Attributes
public:
    
protected:
    CMsgFrame*    m_pMsgFrame;
    CMsgRegex*    m_pRegex;
    CImplMsgSink* m_pMsgSink;
    CMap<CString,LPCTSTR, DWORD, DWORD>   m_SourceNameCookieMap;

    FC_CALLBACK (TargetOnlSessionCB, CMsgWnd) m_SessionCB;
    FC_CALLBACK (TargetOnlExclusiveSessionCB, CMsgWnd) m_ExclusiveSessionCB;
    FC_CALLBACK (TargetOnlConnectStateCB, CMsgWnd) m_ConnectStateCB;
    FC_CALLBACK (CEStateChangeCB, CMsgWnd) m_StateChangeCB;
   
    HACCEL m_hAccelerators;
private:
    CImageList  m_ImageList;

// Operations
public:
    CMsgFrame * GetMsgFrame(void);
    BOOL PreTranslateMessage (MSG *pMsg);

    void ShowCompileWindow();
    void ShowDownloadWindow();
    void ShowFileFindWindow();
    void ShowSystemWindow();

    void ClearCompileWindow(bool bShow=false);
    void ClearDownloadWindow(bool bShow=false);
    void ClearFileFindWindow(bool bShow=false);
    void ClearSystemWindow(bool bShow=false);
    void ClearAllWindows();

    void ShowTargetWindow(const CString& strTargetName);
    LRESULT OnMsgThrdNotify(WPARAM WParam, LPCTSTR szMsgText);
    LRESULT OnMsgThrdReady(WPARAM wParam, LPARAM lParam);
    void StartBuild(CString &ProjectName, CString &strOption);
    void StartBuildLibrary(CString &ProjectName, CString &libPath, CString &libName, 
                           CString &strOption, CString& strTargetExt);
    void StartCompileFile(CString &ProjectName, CString &strOption, CString &FileName);
    void AddMessage(LPCTSTR pszMachine, WORD wScrFacility, LPCTSTR pszDescr);

    void ResetAlarmInSelectedView();
    BOOL GetAlarmState();
    void ResetAlarm(int index);

    CMsgData * FindControlMsgData(CTargetOnlBase *pTargetBase, int &);
    CMsgData * FindControlMsgData(const CString &strTargetName, int &);

    void ConnectTarget(const CString& strTargetName, BOOL bActivate = TRUE);
    void DisconnectTarget(const CString& strTargetName, BOOL bActivate = TRUE);
    void ConnectTargetExclusive(const CString& strTargetName, BOOL bActivate = TRUE);
    void DisconnectTargetExclusive(const CString& strTargetName, BOOL bActivate = TRUE);

    void StartControl(const CString& strTargetName, BOOL bActivate = TRUE);
    void StopControl(const CString& strTargetName, BOOL bActivate = TRUE);
    void StartAllTasks(const CString& strTargetName, BOOL bActivate = TRUE);
    void StopAllTasks(const CString& strTargetName, BOOL bActivate = TRUE);
    void ColdStartReset(const CString& strTargetName, BOOL bActivate = TRUE);
    void WarmStartReset(const CString& strTargetName, BOOL bActivate = TRUE);
    void InitializeClear(const CString& strTargetName, BOOL bActivate = TRUE);

    void UpdateTargetConnectState(CTargetOnlBase* pTargetOnl);

    void ShowNextError();
    void ShowPreviousError();


protected:
    int StartMessaging(LPCTSTR lpszCmd, UINT uiIndex);
    int StopMessaging(UINT uiIndex);
    BOOL CheckMessage(LPCTSTR szMsgText);
    void SetAlarm(int index);
    
    void GetSessionImage(CSC_SESSION_STATE VarSessionState, CSC_SESSION_STATE ExclusiveSessionState, int& iImage);
    void GetConnectImage(E_TargetOnlConnectState eState, int& iImage);
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

private:
    void OnStateChange (CEStateNotify stateNotify, unsigned int uiStage);
    void OnAddTarget (CTargetOnlBase *pTarget);
    void OnRemoveTarget (CTargetOnlBase *pTarget);
    void OnRenameTarget (CTargetOnlBase *pTarget, const CString &strOldTargetName);
    void OnAddressChanged(CTargetOnlBase * pTarget);
    HRESULT OpenMessageSession(CTargetOnlBase* pTarget,CMsgData & MsgData);
    void CloseMessageSession(CMsgData & MsgData);
    void OnExclusiveSessionCB (CTargetOnl *pTarget, CSC_SESSION_STATE sessionState, HRESULT hrError);
    void OnConnectStateCB (CTargetOnlBase *pTarget);

    void OnSessionCB (CTargetOnlBase *pTarget, CSC_SESSION_STATE sessionState, HRESULT hrError);
    
    void OnAdviseDownloader(const CString & );
    void OnUnadviseDownloader(const CString & );

    int ConnectMessageSession(CTargetOnlBase* ,CMsgData *,int);
    int DisconnectMessageSession(CTargetOnlBase* ,CMsgData *,int);
    CMsgData* GetMsgDataByViewer(MSG_VIEWER_TYPE viewer, int &);

    void StopMessagingViewer(MSG_VIEWER_TYPE viewer);
    void StartMessagingViewer(MSG_VIEWER_TYPE viewer);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMsWnd)
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_VIRTUAL

// Implementation

public:
    HRESULT RegisterMsgSource(LPUNKNOWN lpUnkMsgSource, DWORD* lpdwCookie);
    HRESULT UnregisterMsgSource(LPUNKNOWN lpUnkMsgSource, DWORD dwCookie);
    
    virtual ~CMsgWnd();

	// Generated message map functions
protected:
    
private:
    
    //{{AFX_MSG(CMsgWnd)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    afx_msg BOOL OnToolTipNfy (UINT id, NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MSGWND_H_

