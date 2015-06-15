#if !defined(AFX_DLUPDATEDLG_H__0616E3B4_1576_11D2_8853_006097E1BE6E__INCLUDED_)
#define AFX_DLUPDATEDLG_H__0616E3B4_1576_11D2_8853_006097E1BE6E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   --------------------------------------*
#include "resource.h"
//----  Forward Class Definitions:   -------------------------------*
class CStation;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
typedef enum {
    CEDL_DO_NOTHING,
    CEDL_DO_DOWNLOAD,
    CEDL_DO_DELETE
} CEDL_DO_ACTIONS;

/////////////////////////////////////////////////////////////////////////////
// CDLUpdateDlg dialog

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

class CDLUpdateDlg : public CDialog {
// Construction
public:
	CDLUpdateDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CDLUpdateDlg)
	enum { IDD = IDD_SELUPDATE };
	CProgressCtrl	m_DlProgressCtrl;
	CButton	m_DownloadBtn;
	CListCtrl	m_DomainListCtrl;
	CString	m_strControlName;
	CString	m_strDomainName;
	//}}AFX_DATA

    void SetStation(CStation* pStation) {m_pStation = pStation;};
    void SetEventPtr(CImpDLEvents* pDLEvents) {m_pDLEvents = pDLEvents;};
    BOOL FillListFromDB();
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLUpdateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	BOOL SkipDomain(CString & strDomain);
    void UpdateState(void);
// Implementation
protected:
	CStation* m_pStation;
    CImpDLEvents* m_pDLEvents;
	CImageList m_ImageList;
	HCURSOR m_hSaveCursor;
	HWND	m_hwndCapture; 

    int GetImageIdFromFlag(CEDL_DOM_FLAG Flag);
    void ChangeAllSelectedItems(CEDL_DO_ACTIONS Flag);
    void DisplayColumnHeadings(UINT iStringId);
    void EraseList(void);
	BOOL AddColumn(
		LPCTSTR strItem, int nItem,int iID, int nSubItem = -1,
		int nMask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
		int nFmt = LVCFMT_LEFT);
    BOOL AddItem(int nItem,int nSubItem,LPCTSTR strItem,int nImageIndex = -1);
    void ClearProgress(void);
	virtual void OnCancel();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	void OnUpdateAbort();

	// Generated message map functions
	//{{AFX_MSG(CDLUpdateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDompopupDel();
	afx_msg void OnDompopupDown();
	afx_msg void OnDompopupNothing();
	afx_msg void OnColumnclickUpdateList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateDownload();
	afx_msg void OnOK();
	afx_msg void OnUpdateSelectall();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLUPDATEDLG_H__0616E3B4_1576_11D2_8853_006097E1BE6E__INCLUDED_)

