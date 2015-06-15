#if !defined(AFX_CEWATCHDLG_H__1D876E63_F264_11D2_A011_00A024363DFC__INCLUDED_)
#define AFX_CEWATCHDLG_H__1D876E63_F264_11D2_A011_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CeWatchDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCeWatchDlg dialog
#include "CeWatchList.h"
#include "ButtonIcon.h"
#include "XMLHelper/XMLNode.h"
#include "XMLHelper/XMLDocument.h"
#include "CEComboBox.h"

class CCeWatchBackEnd;
class CCeWatchElement;

class CCeWatchDlg : public CDialog
{
// Construction
public:
	CCeWatchDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCeWatchDlg();

// Dialog Data
	//{{AFX_DATA(CCeWatchDlg)
	enum { IDD = IDD_MAINDLG };
	CStatic	m_SelVarNameCtrl;
	CCeWatchList	m_VarList;
	CCEComboBox	m_HistoryListCtrl;
	CButtonIcon	m_DelButton;
	CButtonIcon	m_DelAllButton;
	CButton	m_WriteButton;
	CButtonIcon	m_InsertButton;
	CString	m_strSelVarName;
	//}}AFX_DATA

// Attributes
public:
	void  SetProjectPath(LPCTSTR file);
	inline const CString& GetProjectPath() const { return m_ProjectPath; }

// Operations
public:
	BOOL Store(LPCTSTR file);
	BOOL Load(LPCTSTR file);
	void ForceResubscribe();

    BOOL SaveToNode(CXMLNode& rtNode, CXMLDocument& rtDocument);
    BOOL LoadFromNode(CXMLNode& rtNode);

    void Deactivate();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCeWatchDlg)
	public:
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCeWatchDlg)
	virtual LRESULT OnVarListChanged(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnVarChanged(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnWrite();
	afx_msg void OnRemoveAllVariables();
	afx_msg void OnRemoveSelectedVariables();
	afx_msg void OnAddVariableFromList();
	afx_msg void OnEditchangeCbHistory();
	afx_msg void OnSelchangeCbHistory();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnClose();
	afx_msg void OnSetFocusCbHistory();
	afx_msg void OnKillFocusCbHistory();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    afx_msg void OnCopyItems();
    afx_msg void OnCutItems();
    afx_msg void OnPasteItems();
	afx_msg void OnSetFocus(CWnd* pOldWnd);		// SHIFT_F10 14.12.04 SIS
	// 14.12.05 SIS >>
    afx_msg void OnIncRange();
    afx_msg void OnDecRange();
	// 14.12.05 SIS <<
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private :


	void WriteValue();
	bool CheckWrite(LPCTSTR pNewValue=NULL); 
	void UpdateWriteButton(LPCTSTR pNewValue=NULL);
	void UpdateDelButton();
	void UpdateDelAllButton();
	void UpdateInsertButton();
	void UpdateButtons();
	void UpdateHistoryList();
    bool CopyItems ();

private:
	CToolTipCtrl	 m_ToolTipCtrl;
	CCeWatchElement* m_pWriteVar; 
	CCeWatchBackEnd* m_pBackEnd;
	CString	         m_ProjectPath;

	CWnd*			m_ToolWndArray[5];
	bool			m_bHistoryFocus;

    HACCEL m_hAccelerators;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEWATCHDLG_H__1D876E63_F264_11D2_A011_00A024363DFC__INCLUDED_)
