#ifndef __WATCHWND_H_
#define __WATCHWND_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SizedControlBar.h"
#include "fccallback.h"
#include "CEStateMan.h"
#include "Settings.h"


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*
class CWatchTabWindow;
class CWatchControlWrapper;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CWatchWindow : public CSizedControlBar  //SECControlBar
{

private:
    enum ETabType
    {
        eAutomatic,     // 0: automatic pane "Watch X"
        eImportExport,  // 1: name of imported / exported file
        eManual,        // 2: manually set
        eForce          // 3. force type
    };

    typedef CList<ETabType*, ETabType*> TTabInfoList;

    TTabInfoList    m_tTabInfo;

    // SIS: used for settings
    FC_CALLBACK(CEStateChangeCB, CWatchWindow) m_StateChangeCB;

    void DeleteTabInfo();

public:
    CWatchWindow ();
    virtual ~CWatchWindow ();

    void SetProjectPath (CString strProjectPath);

    int AddCustomPane(const CString& crstrName = _T(""));
    void RemoveCustomPane (int iCustomPane);
    void ShowCustomPane (int iCustomPane);
    HRESULT LoadCustomPane (int iCustomPane = -1, CString strPath = _T(""));
    HRESULT SaveCustomPane (int iCustomPane = -1, CString strPath = _T(""));
    HRESULT ResubscribeCustomPane (int iCustomPane);

    void AddForcedPane ();
    void RemoveForcedPane ();
    void ShowForcedPane ();

	// SHIFT_F10 14.12.04 SIS >>
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// SHIFT_F10 14.12.04 SIS <<

protected:
    CWatchControlWrapper *GetCustomPaneWrapper (int iCustomPane);
    CString BuildCustomPaneCaption (int iIndex);

    // SIS, 11.04.01: remove tab including destroy view
    void RemoveTab(int iIndex);

    void FillSettings(CWatchSettings& rtWatchSettings);
    void LoadSettings(CWatchSettings& rtWatchSettings);

    ETabType GetTabType(int iIndex);
    void SetTabType(int iIndex, ETabType eType);

    CString GetTabLabel(int iIndex);
    void SetTabLabel(int iIndex, const CString& crstrLabel, BOOL bUser = FALSE);

    CString QueryWatchFile(bool bSave, const CString& crstrInitFile = _T(""));

    SECTab* AddTab(const CString& crstrLabel, ETabType eType);
    SECTab* InsertTab(int iIndex, const CString& crstrLabel, ETabType eType);

    void OnWatchTabDblClck(WPARAM wparam, LPARAM lparam);
    void OnWatchTabRename(WPARAM wparam, LPARAM lparam);

    BOOL ActivateTab(int iTab);

protected:

    CWatchTabWindow *m_pTabWindow;

    CString m_strProjectPath;

    int m_iContextMenuTab;

    int m_iCustomPaneCount;

    int m_iForcedPaneTab;

    HACCEL	m_hAccelerators;    // SHIFT_F10 14.12.04 SIS

public:
    DECLARE_DYNCREATE(CWatchWindow)

protected:
    //{{AFX_VIRTUAL(CWatchWindow)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CWatchWindow)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd (CDC *pDC);
    afx_msg void OnContextMenu (CWnd *pWnd, CPoint point);
    afx_msg void OnAddPane ();
    afx_msg void OnRemovePane ();
    afx_msg void OnImport();
    afx_msg void OnExport();
    afx_msg void OnRenameTab();
    afx_msg LRESULT OnWatchTabContextMenu (WPARAM wparam, LPARAM lparam);
   	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSetFocus (CWnd *pOldWnd);	// SHIFT_F10 15.12.04 SIS
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void OnStateChange (CEStateNotify stateNotify, unsigned int uiStage);

};


//{{AFX_INSERT_LOCATION}}


#endif // __WATCHWND_H_