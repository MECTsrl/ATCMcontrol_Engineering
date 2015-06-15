#ifndef __NAVIGATOR_H_
#define __NAVIGATOR_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileTreeCtrl.h"
#include "ClassTreeCtrl.h"
#include "HWTreeCtrl.h"
#include "InstTreeCtrl.h"
#include "NavTreeControl.h"
#include "NavDef.h"
#include "resource.h"
#include "CEProjMan.h"
#include "CEStateMan.h"
#include "SizedDialogBar.h"
#include "CEComboBox.h"

#include "TargetOnlBase.h"
#include "TargetOnl.h"

extern CComModule _Module;
#include "ProjManEventsImpl.h"


class CSubscriptionManager;
class CPrintSettings;


/////////////////////////////////////////////////////////////////////////////
// CNavigator dialog


//------------------------------------------------------------------*
/**
 *  class CNavigator
 *
 *  navigator class
 *  contains file, class, instance and target view
 *
 *  @see    CNavTreeCtrl
 */
class CNavigator : public CSizedDialogBar
{
private:
    DECLARE_DYNCREATE(CNavigator)

    FC_CALLBACK (TargetOnlSessionCB, CNavigator) m_SessionCB;
    FC_CALLBACK (TargetOnlExclusiveSessionCB, CNavigator) m_DebugSessionCB;
    FC_CALLBACK (TargetOnlConnectStateCB, CNavigator) m_ConnectStateCB;
    FC_CALLBACK(CEStateChangeCB, CNavigator) m_StateChangeCB;

// Construction
public:
	CNavigator(CWnd* pParent = NULL);   // standard constructor
    virtual ~CNavigator();

// Dialog Data
	//{{AFX_DATA(CNavigator)
	enum { IDD = IDD_NAVIGATOR };
	CTabCtrl	    m_TabCtrl;
	CButton	        m_FltFrameCtrl;
	CCEComboBox	    m_FltNameCtrl;
	CCEComboBox	    m_FltCatCtrl;
	CFileTreeCtrl	m_tFlTreeCtrl;
	CClassTreeCtrl	m_tClTreeCtrl;
	CHWTreeCtrl	    m_tHWTreeCtrl;
	CInstTreeCtrl	m_tInstTreeCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNavigator)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnSize(UINT nType, int cx, int cy);
	//}}AFX_VIRTUAL

// Implementation
public:
    // loading / unloading project
    //void LoadProject(LPCTSTR lpszPathName);
    //void SaveProject(LPCTSTR lpszPathName);

    void InitTree(NAV_TreeMode eNavTree, CXMLDocument* pXMLDoc);
    void BuildTree(NAV_TreeMode eNavTree);
    void UpdateTreeNode(NAV_TreeMode eNavTree, const CString& crstrIdPath);
    void UpdateAttributes(NAV_TreeMode eNavTree, const CString& crstrIdPath);
    void UpdateDelete(NAV_TreeMode eNavTree, const CString& crstrIdPath);
    void UpdateInsert(NAV_TreeMode eNavTree, const CString& crstrIdPath);
//    void UpdateSourceState(NAV_TreeMode eNavTree, const CString& crstrIdPath);
    void InvalidateTree(NAV_TreeMode eNavTree);
    void OnFileAdded(const CString& crstrFilePath, enum E_FV_FileCategory eFileCat);
    void OnFileRemoved(const CString& crstrFilePath, enum E_FV_FileCategory eFileCat);
    BOOL IsVisible();
    void InvalidateFileView();

    void AddTargetInstance(const CString& crstrId, const CString& crstrIdPath);
    void RemoveTargetInstance(const CString& crstrId);
    BOOL OnIdle(LONG lCount);

    // context menu functionality used by project list view
    void AddContextMenuForNode(CMenu* pMenu, const CString& crstrIdPath);
    void DeleteNodesByIdPath(CStringArray& rastrIdPath);
    void OpenNode(const CString& crstrIdPath);
    BOOL IsKeyMouseActionAllowed(const CString& crstrIdPath, int iKeyMouseEvent);
    void RenameNode(const CString& crstrIdPath, const CString& crstrNewId);
    BOOL ShowNode(const CString& crstrIdPath);
    void DoContextMenuCommandForNode(const CString& crstrIdPath, UINT nID);

    // file view functionality used by project list view
    BOOL CopyFileToProject(const CString& crstrFilePathName, E_FV_FileCategory eCatTo);
    void EnableFileCheck(BOOL bFileCheck = TRUE);
    void CheckFiles(BOOL bSilent = FALSE);

    void Parse();

    void AddSourceFiles(CStringArray& rastrFiles, const CString& crstrForeignProjPath);

    BOOL PrintTreeView(NAV_TreeMode eTree, CPrintSettings* pPrintSettings, CString& rstrFilePathName);

protected:

	// Generated message map functions
	//{{AFX_MSG(CNavigator)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSelchangeTabNavigator(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelendokFilterName();
	afx_msg void OnEditchangeFilterName();
	afx_msg void OnSelchangeFilterCat();
	afx_msg void OnItemexpandingFlTREE(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusFilterName();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnBeginlabeleditFlTREE(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditClTREE(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditClTREE(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditFlTREE(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditHwtree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditHwtree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditInsttree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditInsttree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragClTREE(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragFlTREE(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemexpandingHwtree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandingInsttree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandingClTREE(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBegindragHwtree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragInsttree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownFlTREE(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNavCopy();
	afx_msg void OnNavCut();
	afx_msg void OnNavPaste();
	afx_msg void OnCollapseAll();
	afx_msg void OnExpandAll();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnNavContextMenu();	// SHIFT_F10 14.12.04 SIS
	//}}AFX_MSG
public:
    afx_msg void OnContextMenuTreeCommand(UINT nID);
	afx_msg void OnDropFiles(HDROP hDropInfo);

	DECLARE_MESSAGE_MAP()

protected:
    void LoadProfile();
    void SaveProfile();
    BOOL Init();
    BOOL InitControls();
    void InitTab();
    void SetFilterData(BOOL bNamesChanged = FALSE);
    void GetFilterData();

    void ResizeWindow(const CRect& rt_new_rect);
    void ShowActualTree(BOOL bChange = FALSE);
    void InsertNewFilter(const CString& rstrFilter);
	
//    virtual void OnExtendContextMenu(CMenu* pMenu);

    HRESULT AdviseProjEventHandler();
    
    void UnadviseProjEventHandler();
    
    void OnStateChange (CEStateNotify stateNotify, unsigned int uiStage);

    // return pointer to currently active tree control
    CNavTreeCtrl* GetActiveTreeCtrl();
    // return pointer to specified tree control
    CNavTreeCtrl* GetTreeCtrl(NAV_TreeMode eTreeMode);

    // get standard docking height (CSizedDialogBar)
    int GetStandardDockingHeight();

    void UnregisterSessionCBs();
    void OnSessionCB (CTargetOnlBase* pTarget, CSC_SESSION_STATE sessionState, HRESULT hrError);
    void OnDebugSessionCB (CTargetOnl* pTarget, CSC_SESSION_STATE sessionState, HRESULT hrError);

    void OnConnectChange(CTargetOnlBase *pTarget);

    //  Variable subsciption management of navigator
//    void Subscribe();
//    void CleanupSubscription();
//    bool LookupSubscription(const CString& crstrKey, CSubscrElement& Container);
//    bool LookupRequest(const CString& crstrKey,CSubscrReq & Request);

//    bool LookupTreeItem(CTreeCursor & Cursor,HTREEITEM hItem);
    DWORD GetStateAttribute (CSubscription *pStateSubscription, CSubscription *pClearedSubscription);

    CNavTreeCtrl* GetTreeForIdPath(const CString& crstrIdPath, NAV_TreeMode& reTreeMode);

protected:
    BOOL            m_bInitialized;     // navigator initialized?
    BOOL            m_bProjectLoaded;   // project loaded?
    CRect           m_WindowRect;       // current window size for resizing purposes
    NAV_TreeMode	m_ActualTreeMode;   // no of currently visible tree
    UINT            m_IDCheckEvent;     // ID of timer event

    CImageList      m_ctlImage;         // bitmap for images of the Tab control

protected:
    CComObject<CProjManEventsImpl> *m_pProjEventHandler;
	DWORD                           m_dwProjEventHandlerCookie;
	DWORD                           m_dwProjInstEventHandlerCookie;
//    CComQIPtr<ICEProjectManager>    m_pProjMan;
    HACCEL                          m_hAccelerators;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __NAVIGATOR_H_

