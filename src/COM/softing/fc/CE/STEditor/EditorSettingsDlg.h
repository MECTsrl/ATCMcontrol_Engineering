// EditorSettingsDlg.h : header of the CEditorSettingsDlg class
//

#ifndef _EDITORSETTINGSDLG_H_
#define _EDITORSETTINGSDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditorSettingsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditorSettingsDlg dialog

class CEditorSettingsDlg : public CDialog
{
// Construction
public:
	CEditorSettingsDlg(CWnd* pParent = NULL);   // standard constructor
    ~CEditorSettingsDlg();

    void SetDefaultEditSettings(SECEditSettings& rtDefaultSettings);

// Dialog Data
	//{{AFX_DATA(CEditorSettingsDlg)
	enum { IDD = IDD_EDITOR_SETTINGS };
	BOOL	m_bKeepTabs;
	int		m_iTabSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditorSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditorSettingsDlg)
	afx_msg void OnEsDeftextcolBtn();
	afx_msg void OnEsQuotecolBtn();
	afx_msg void OnEsCommentcolBtn();
	afx_msg void OnEsBgcolBtn();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnEsKeywordcolBtn();
	afx_msg void OnEsDefault();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    void CreateBgBrush();
    void CreateTextFont();
    BOOL SelectColor(COLORREF& crCol);
    void SetValues(BOOL bSave = TRUE);

protected:
    SECEditSettings     m_tDefaultSettings;
    SECEditSettings     m_tSettings;

    HBRUSH              m_hBgBrush;
    HFONT               m_hFont;

    COLORREF            m_crBgColor;
    COLORREF            m_crDefTextColor;
    COLORREF            m_crKeywordColor;
    COLORREF            m_crQuoteColor;
    COLORREF            m_crCommentColor;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _EDITORSETTINGSDLG_H_
