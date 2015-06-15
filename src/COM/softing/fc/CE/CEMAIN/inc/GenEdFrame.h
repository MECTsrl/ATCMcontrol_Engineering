
#ifndef __GENEDFRAME_H_
#define __GENEDFRAME_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CGenEdDoc;
class CGenEdView;
class CGenEdFrame;
class CGenEdCntrItem;
class CGenEdCont;
class CDocumentSettings;

#include "GenEdRC.h"
#include "TargetOnlBase.h"
#include "TargetOnl.h"
#include "GenEdTabWnd.h"
#include "fccallback.h"
#include "SBLLexTree.h"


typedef enum E_ActivateAction
{
    eActivateNone,
    eActivateClass,
    eActivateInstance
};


enum GenEdFrameAction { GenEdFrameActivated, GenEdFrameDeactivated, GenEdFrameDestroyed };

FC_CALLBACK_DEF2 (GenEdFrameActionCB, GenEdFrameAction, CGenEdFrame *)


class CGenEdFrame : public SECMDIChildWnd
{
    DECLARE_DYNCREATE (CGenEdFrame)
protected:
    CGenEdFrame ();           // protected constructor used by dynamic creation

// tab info class (used as structure)
public:
//    class CInstTabInfo
//    {
//    public:
//        CString strId;          // identification string
//        CString strInstPath;    // instance path
//        CString strIdPath;      // id path in inst tree
//    };
//
//    typedef CMap<CString, LPCTSTR, CInstTabData, CInstTabData&> T_TabInfo;

// public methods
public:
    virtual void ActivateFrame (int nCmdShow=-1);

    void ActivateServer();
    void DeactivateServer();
    void SetFocusToServer();

    void RegisterActionCB (FC_CALLBACK_BASE (GenEdFrameActionCB) *pActionCB);
    void UnregisterActionCB (FC_CALLBACK_BASE (GenEdFrameActionCB) *pActionCB);

    void SetContainer(CGenEdCont* pContainer);
    int  GetActiveTab();

    bool IsTabbed();

    void ActivateClassTab();

protected:
    CGenEdCont*             m_pContainer;
    CGenEdTabWnd*           m_pGenEdTabWnd;
    CWnd*                   m_pGenEdWnd;
    CCreateContext          m_tCreateContext;   // used to create tab wnds
    CStringArray            m_arrInst;
    int                     m_iSelectedTab;
    int                     m_iContextTab;
    int                     m_iTabExpandLevel;
    bool                    m_bTabbed;
    bool                    m_bInsertInitialTabs;
    bool                    m_bHaveAddTab;
    CSBLLexTree<bool>       m_UntabbedInstances;
    CImageList              m_ImageList;
    DWORD                   m_dwClientCookie;
//    T_TabInfo               m_tTabInfo;
    CString                 m_strOldTitle;
    BOOL                    m_bAutoMode;

    FC_CALLBACK_HOLDER (GenEdFrameActionCB) m_ActionCBs;

    FC_CALLBACK (TargetOnlConnectStateCB, CGenEdFrame) m_ConnectStateCB;
    FC_CALLBACK (TargetOnlExclusiveSessionCB, CGenEdFrame) m_ExclusiveSessionCB;

// Operations
public:
    void GetServerViews(CArray<CGenEdView *, CGenEdView *>& ratServerViews);
    BOOL UpdateTabs(E_ActivateAction eAction = eActivateClass);
    void OnAddTarget(CTargetOnlBase *pTarget);
    void OnRemoveTarget(CTargetOnlBase *pTarget);

    HRESULT ShowInstance (const CString& crstrInstPath, bool bWithTask);

    void CheckAssignmentChange ();

    CGenEdView* FindView();

    void LoadWindowPosition(const CString& crstrFilePath);

    BOOL SaveInstanceInfo(CDocumentSettings* pDocSettings = NULL, BOOL bForce = FALSE);
    BOOL LoadInstanceInfo(bool bActivateTab = true, CDocumentSettings* pDocSettings = NULL);

    virtual void OnUpdateFrameTitle (BOOL bAddToTitle);

    CString GetToolTipTextForTab(int iTab);
    CGenEdDoc *GetCurrentEditorDoc ();
    CGenEdView *GetCurrentEditorView(int iIndex);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGenEdFrame)
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    protected:
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CGenEdFrame ();
    virtual void SelectInstances (int iInsertBefore);
    virtual void InstanceSelected (int iIndex);

public:
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
protected:
//{{AFX_MSG(CGenEdFrame)
    afx_msg void OnNcActivate (BOOL bActive);
    afx_msg void OnNcDestroy();
    afx_msg void OnSysCommand(UINT nID, LONG lParam);
    afx_msg void OnAddInstance();
    afx_msg void OnRemoveInstance();
    afx_msg void OnRemoveAll();
    afx_msg void OnUpdateRemoveAll(CCmdUI* pCmdUI);
    afx_msg void OnUpdateRemoveInstance(CCmdUI* pCmdUI);
    afx_msg void OnExpandTabs();
    afx_msg void OnCollapseTabs();
    afx_msg void OnUpdateExpandTabs(CCmdUI* pCmdUI);
    afx_msg void OnUpdateCollapseTabs(CCmdUI* pCmdUI);
    afx_msg void OnUpdateAddInstance(CCmdUI* pCmdUI);
    afx_msg void OnRefreshAll();
    afx_msg void OnPreviousTab();
    afx_msg void OnNextTab();
    afx_msg void OnPreviousServer();
    afx_msg void OnNextServer();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
    afx_msg void OnInstTabContextMenu (WPARAM tab, LPARAM pos);
    afx_msg void OnRecalculateLayout (WPARAM, LPARAM);
    afx_msg LRESULT OnSetFocusToActiveChildFrame (WPARAM, LPARAM);
    afx_msg void OnMDIActivate (BOOL bActivate, CWnd *pActivateWnd, CWnd *pDeactivateWnd);
    afx_msg LRESULT OnTabSelect(WPARAM WParam, LPARAM LParam );
    afx_msg LRESULT OnUpdateTitle(WPARAM wParam, LPARAM lParam);
    


    DECLARE_MESSAGE_MAP ()

private:
    void BeginLayout ();
    void EndLayout ();
    void AddInstanceTab (int iInsertBefore, const CString &strInstanceName, bool bUpdate = true);
    int GetTabCount();
    void UpdateAddTab();
    void UpdateTabNames ();
    CString GetInstanceTabName (int iIndex);
    CString GetInstanceTabFullName (int iIndex);
    HRESULT GetInstances ();
    void RemoveTabbedInstances ();
    int GetMaxExpandLevel ();
    CTargetOnlBase *MachineFromInstance (const CString& crstrInstPath);
//    void GetSessionImage(CSC_SESSION_STATE VarSessionState, CSC_SESSION_STATE ExclusiveSessionState, int & iImage);
    void OnConnectStateCB (CTargetOnlBase *pTarget);
    void OnExclusiveSessionCB (CTargetOnl *pTarget, CSC_SESSION_STATE sessionState, HRESULT hrError);
    int FindTabbedInstance (const CString& crstrInstPath, bool bWithTask);
    CString FindUntabbedInstance (const CString& crstrInstPath, bool bWithTask);
    void SplitInstance (const CString& crstrInstPath, CString &strInstanceNoTask, CString &strTask);
    void SwitchToAnyServer ();
    bool GetServerIndex (CGenEdCntrItem *pServer, int &iIndex);

    BOOL InsertTab(CRuntimeClass* pViewClass,
                    int nIndex,
                    LPCTSTR lpszLabel,
                    CCreateContext* pContext =NULL,
                    UINT nID = -1);

    void RemoveTab(int iIndex);

    void GetConnectImage(E_TargetOnlConnectState eState, int& iImage);

    CTargetOnlBase* GetMachineFromInstance (const CString& crstrInstPath);

    void AddDefaultTabs();

    CDocumentSettings* CGenEdFrame::GetDocumentSettings();

};

/////////////////////////////////////////////////////////////////////////////
// CAddInstanceDlg dialog

class CAddInstanceDlg : public CDialog
{
// Construction
public:
    CAddInstanceDlg (CSBLLexTree<bool> *pInstances, CWnd *pParent=NULL);   // standard constructor

    CSBLLexTree<bool>*          m_pInstances;
    CArray<CString, CString>    m_SelectedInstances;

// Dialog Data
    //{{AFX_DATA(CAddInstanceDlg)
    enum { IDD = IDD_GENED_ADDINSTANCE };
    CListCtrl	m_InstanceList;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAddInstanceDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CAddInstanceDlg)
    afx_msg void OnDoubleClickInstances(NMHDR* pNMHDR, LRESULT* pResult);
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // __GENEDFRAME_H_


