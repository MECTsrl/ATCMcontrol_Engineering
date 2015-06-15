#if !defined(AFX_FCINSERTTABLEDLG_H__CE424EC0_CE60_11D2_B4B3_001767C30000__INCLUDED_)
#define AFX_FCINSERTTABLEDLG_H__CE424EC0_CE60_11D2_B4B3_001767C30000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FcInsertTableDlg.h : header file
//
#include "FcColorComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CFcDlgInsertTable dialog

class CFcDlgInsertTable : public CDialog
{
// Construction
public:
	CFcDlgInsertTable(CFcHtmlEdView* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFcDlgInsertTable)
	enum { IDD = IDD_INSERT_TABLE };
	CComboBox	m_CellAlignCbCtrl;
	CComboBox	m_AlignCbCtrl;
	CFcColorComboBox	m_CbCtrlColorCellBorder;
	CFcColorComboBox	m_CbCtrlColorCellBg;
	CFcColorComboBox	m_CbCtrlColorBorder;
	CFcColorComboBox	m_CbCtrlColorBg;
	CSpinButtonCtrl	m_SpinCtrlBorderSize;
	CSpinButtonCtrl	m_SpinCtrlSpacing;
	CSpinButtonCtrl	m_SpinCtrlPadding;
	CSpinButtonCtrl	m_SpinCtrlWidth;
	CSpinButtonCtrl	m_SpinCtrlRows;
	CSpinButtonCtrl	m_SpinCtrlColumns;
	int		m_nColumns;
	int		m_nRows;
	int		m_nWidth;
	int		m_WidthUnit;
	int		m_Align;
	int		m_nPadding;
	int		m_nSpacing;
	int		m_CellAlign;
	CString	m_CellBgColor;
	CString	m_BgImage;
	CString	m_BgColor;
	CString	m_ColorBorder;
	CString	m_CellColorBorder;
	BOOL	m_bWrapText;
	int		m_nBorderSize;
	CString	m_Title;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFcDlgInsertTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFcDlgInsertTable)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnTableCellColorBorderButton();
	afx_msg void OnTableCellBgColorButton();
	afx_msg void OnTableImageBgButton();
	afx_msg void OnTableColorBorderButton();
	afx_msg void OnTableBgColorButton();
	afx_msg void OnDeltaposTableColumnsSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposTableRowsSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposTableWidthSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposTableCellSpacingSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposTableCellPaddingSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposTableBorderSizeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void DoSpin(int delta, int& value, int mini, int maxi, LRESULT* pResult);
	CString GetTableAttributes();
	CString GetCellAttributes();
	void SelectCustomColor(CString& ColEntry);
	CString GetColorEntry(const CString& str);

private:	
	CFcHtmlEdView* m_pParent;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FCINSERTTABLEDLG_H__CE424EC0_CE60_11D2_B4B3_001767C30000__INCLUDED_)
