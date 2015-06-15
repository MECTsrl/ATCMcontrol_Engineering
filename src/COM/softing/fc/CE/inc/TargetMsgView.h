#if !defined(AFX_PROBEFORMVIEW_H__3BBF0024_CF87_43F4_9B48_2AD4DC145C7D__INCLUDED_)
#define AFX_PROBEFORMVIEW_H__3BBF0024_CF87_43F4_9B48_2AD4DC145C7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProbeFormView.h : header file
//

//----  Aggregate Includes:   --------------------------------------*
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "msgrc.h"
#include "msgwnd.h"
//----  Forward Class Definitions:   -------------------------------*
struct MsgTargetDialogItemInfo;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


/////////////////////////////////////////////////////////////////////////////
// CMsgEdit window

class CMsgEdit : public CEdit
{
// Construction
public:
	CMsgEdit();

// Attributes
public:

protected:
    CMsgRegex * m_pRegex;
    CBrush * m_phbr;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMsgEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMsgEdit)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CTargetMsgView form view

class CTargetMsgView : public CFormView
{
protected:
	CTargetMsgView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTargetMsgView)

// Form Data
public:
	//{{AFX_DATA(CTargetMsgView)
	enum { IDD = IDD_MSGTARGET_DLGBAR };
	CMsgEdit	m_Edit;
	//}}AFX_DATA

// Attributes
public:
    CImageList          m_ImageList;
protected:
    //CMsgEdit*	        m_pEdit;
protected:
    CString             m_strTargetName;        // name of target
    CBrush*             m_phbr;
    CString             m_strTargetAddr;
    CSC_SESSION_STATE   m_SessionState;
    CSC_SESSION_STATE   m_ExclusiveSessionState;
    E_TargetOnlConnectState m_eConnectState;
    bool                m_bResourceCleared;
    int                 m_iResourceState;
//    CImageList          m_ConnImageList;
	bool                m_bSubscribedForState;
	CSubscription*      m_pStateSubscription;
	CSubscription*      m_pClearedSubscription;
    CImageList          m_ConnImageList;
    BOOL                m_bInitialized;         // used to set font in OnSize()

// Operations
public:
	virtual void OnTargetAddChangeNotify (LPCTSTR pszNewTargetAddr);
    void SetTargetAddress(const TCHAR* pszTargetAddr);

    void OnSizeDialogItems (int iLeft, int iTop, int iWidth, int iHeight);
    int  SizeItems (unsigned int uiCount, MsgTargetDialogItemInfo *pItemInfos);
    void AppendText(LPCTSTR pszMsgText);

	virtual void OnSessionNotify (CSC_SESSION_STATE sessionState);
	virtual void OnExclusiveSessionNotify (CSC_SESSION_STATE sessionState);

    void OnConnectStateNotify(E_TargetOnlConnectState eConnectState);

    void SetTargetName(const CString& crstrTargetName);
    void InitTarget();

    void SetStateText(const CString& crstrStateText);

protected:
    CTargetOnlBase* GetTargetOnlObject();
    HRESULT SubscribeForState();
    HRESULT UnsubscribeForState();
    void OnResourceStateCB (CSubscription *pSubscription);
	
    FC_CALLBACK (SubscriptionModifiedCB, CTargetMsgView) m_ResourceStateCB;

    void SetResourceCleared (bool bCleared);
    void SetResourceState (int iState);
    void SetSessionState (CSC_SESSION_STATE sessionState);
	void SetExclusiveSessionState (CSC_SESSION_STATE sessionState);
    void SetConnectState(E_TargetOnlConnectState eConnectState);
    void GetConnectImage(E_TargetOnlConnectState eState, int& iImage);
private:
    void UpdateStateText();
//	void DrawSessionState (CPaintDC *pDC);
	void DrawConnectState (CPaintDC *pDC);
	void DrawResourceState (CPaintDC *pDC);

    BOOL GetUserConnectInfo(CString& rstrUserConnectInfo);

    CMsgWnd* GetMsgView(bool bMustLive=true);
    CMsgEdit* GetEditCtrl();
    void UpdateEditText(bool bFocus = false);
    CString OnGetToolTipText (CWnd *pWnd);

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTargetMsgView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:

	virtual ~CTargetMsgView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTargetMsgView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnConnect();
	afx_msg void OnAcknowledge();
	afx_msg void OnAlltasksstart();
	afx_msg void OnAlltasksstop();
	afx_msg void OnResWarmstart();
	afx_msg void OnResColdstart();
	afx_msg void OnResclear();
	afx_msg void OnConnectExclusive();
	afx_msg void OnDisconnectExclusive();
	afx_msg void OnResStart();
	afx_msg void OnResStop();
	afx_msg void OnDisconnect();
	afx_msg void OnUpdateConnect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDisconnect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWarmstart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColdstart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateResStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateResStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAllTasksStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAllTasksStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExclusiveConnect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExclusiveDisconnect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateResClear(CCmdUI* pCmdUI);
    afx_msg void OnUpdateAck(CCmdUI* pCmdUI);
	afx_msg void OnPaint();
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnEditCopy ();
    afx_msg void OnEditClear ();
    afx_msg void OnContextMenuCommand();
    afx_msg BOOL OnToolTipNfy (UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    friend class CMsgWnd;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROBEFORMVIEW_H__3BBF0024_CF87_43F4_9B48_2AD4DC145C7D__INCLUDED_)