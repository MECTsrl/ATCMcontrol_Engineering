
#if !defined(AFX_INSTRANDLG_H__C3B21244_5126_11D2_810B_444553540000__INCLUDED_)
#define AFX_INSTRANDLG_H__C3B21244_5126_11D2_810B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// InsTranDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInsTranDlg dialog

class CInsTranDlg : public CDialog
{
// Construction
public:
	CInsTranDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
// pointer to be used for filling in the list boxes.
// the pointer are given valid values in the view.	
	CStringList *m_pNameList;
	//{{AFX_DATA(CInsTranDlg)
	enum { IDD = IDD_GR_PROP_TRAN };
	CString	m_strName;
	BOOL	m_bSetDefaultSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsTranDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CComboBox& NameBox()
		{return *(CComboBox*)GetDlgItem(IDC_GR_TRAN_NAME); }

	// Generated message map functions
	//{{AFX_MSG(CInsTranDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_INSTRANDLG_H__C3B21244_5126_11D2_810B_444553540000__INCLUDED_)
