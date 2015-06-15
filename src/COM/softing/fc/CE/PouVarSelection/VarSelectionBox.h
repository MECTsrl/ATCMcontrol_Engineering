#if !defined(AFX_VARSELECTIONBOX_H__16A606E5_BC80_4AC8_9F49_9C0C1D6BB0DC__INCLUDED_)
#define AFX_VARSELECTIONBOX_H__16A606E5_BC80_4AC8_9F49_9C0C1D6BB0DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VarSelectionBox.h : header file
//

#include "Resource.h"

class CPouVarSelCtrl;

/////////////////////////////////////////////////////////////////////////////
// CVarSelectionBox dialog

class CVarSelectionBox : public CDialog
{
// Construction
public:
	CVarSelectionBox(CWnd* pParent = NULL);   // standard constructor

  
    BOOL Init(CPouVarSelCtrl* pPouVarSelCtrl,
              LONG xPos, LONG yPos,
              const CString& strLine,
              const LONG iColumn,
              const CString& strTypeHint,
              const CString& strPouName);

    CString GetSelection(LONG& iColStart, LONG& iColEnd);

// Dialog Data
	//{{AFX_DATA(CVarSelectionBox)
	enum { IDD = IDD_VAR_SELECT };
	CComboBox	m_ComboVarSel;	// 13.12.06 SIS
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVarSelectionBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    BOOL FindText(const CString& strLine, const LONG iColumn);
    void FillVarList(SAFEARRAY* pSArr);
    void SelectListEntry();

	// Generated message map functions
	//{{AFX_MSG(CVarSelectionBox)
	virtual void OnCancel();
    virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnDblclkVarList();
	afx_msg void OnChangeVarSelEdit();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    CString m_strOrigText;
    LONG    m_iStartCol;
    LONG    m_iEndCol;
    CString m_strEditorText;
    CString m_strTypeHint;
    CString m_strSelected;
    TCHAR   m_chLastChar;
    BOOL    m_bNewVar;
    CList<CString, CString&> m_listVars;
    LONG    m_xPos;
    LONG    m_yPos;
    CWnd*   m_pParent;
    CPouVarSelCtrl* m_pPouVarSelCtrl;
    BOOL    m_bComplexSelection;
    CString m_strPouName;
    BOOL    m_bEnding;
    BOOL    m_bMouseOutOfBox;
    UINT    m_timer;
    CString m_strComplexVarPart;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VARSELECTIONBOX_H__16A606E5_BC80_4AC8_9F49_9C0C1D6BB0DC__INCLUDED_)
